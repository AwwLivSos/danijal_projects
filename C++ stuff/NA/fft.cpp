#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <stdexcept>

const double PI = std::atan(1) * 4;

std::complex<double> W(double N) {
    return {std::cos(2 * PI / N), std::sin(2 * PI / N)};
}

void FFT(const std::vector<double> &input, std::vector<std::complex<double>> &output, int N, int s = 0, int d = 0, int t = 1) {
    if (N == 1) {
        output[d] = input[s];
        return;
    }

    FFT(input, output, N / 2, s, d, t * 2);
    FFT(input, output, N / 2, s + t, d + N / 2, t * 2);

    std::complex<double> WN = W(N);
    std::complex<double> W_power = 1;
    std::complex<double> W_step = std::pow(WN, -1);

    for (int k = 0; k < N / 2; ++k) {
        auto u = output[d + k];
        auto v = W_power * output[d + k + N / 2];
        output[d + k] = u + v;
        output[d + k + N / 2] = u - v;
        W_power *= W_step;
    }
}

void InvFFT(const std::vector<std::complex<double>> &input, std::vector<std::complex<double>> &output, int N, int s = 0, int d = 0, int t = 1) {
    if (N == 1) {
        output[d] = input[s];
        return;
    }

    InvFFT(input, output, N / 2, s, d, t * 2);
    InvFFT(input, output, N / 2, s + t, d + N / 2, t * 2);

    std::complex<double> WN = W(N);
    std::complex<double> W_power = 1;

    for (int k = 0; k < N / 2; ++k) {
        auto u = output[d + k];
        auto v = W_power * output[d + k + N / 2];
        output[d + k] = (u + v) / 2.0;
        output[d + k + N / 2] = (u - v) / 2.0;
        W_power *= WN;
    }
}

std::vector<double> LossyCompress(const std::vector<double> &data, int new_size) {
    if (new_size < 2 || new_size > data.size())
        throw std::range_error("Bad new size");
    if ((data.size() & (data.size() - 1)) != 0)
        throw std::range_error("Data size must be a power of two");

    int N = data.size();
    std::vector<double> reordered(N);

    for (int i = 0; i < N / 2; ++i) 
        reordered[i] = data[2 * i];
    for (int i = N / 2; i < N; ++i) 
        reordered[i] = data[2 * (N - i) - 1];

    std::vector<std::complex<double>> transformed(N);
    FFT(reordered, transformed, N);

    std::vector<double> compressed(new_size);
    std::complex<double> WN = W(2 * N);

    for (int i = 0; i < new_size - 1; ++i)
        compressed[i] = (std::pow(WN, -i / 2.0) * transformed[i]).real();

    compressed[new_size - 1] = N; 
    return compressed;
}

std::vector<double> LossyDecompress(const std::vector<double> &compressed) {
    int N = static_cast<int>(compressed.back());
    if ((N & (N - 1)) != 0 || compressed.size() > N)
        throw std::logic_error("Bad compressed sequence");

    std::vector<std::complex<double>> transformed(N);
    std::complex<double> WN = W(2 * N);

    transformed[0] = compressed[0];
    for (int i = 1; i < compressed.size() - 1; ++i)
        transformed[i] = 2.0 * std::pow(WN, i / 2.0) * compressed[i];

    std::vector<std::complex<double>> reordered(N);
    InvFFT(transformed, reordered, N);

    std::vector<double> decompressed(N);
    for (int i = 0; i < N; ++i)
        decompressed[i] = (i % 2 == 0) ? reordered[i / 2].real() : reordered[N - (i + 1) / 2].real();

    return decompressed;
}

void Testiraj() {
    std::vector<double> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    for (int size = 2; size <= 15; size++) {
        auto compressed = LossyCompress(data, size);
        auto decompressed = LossyDecompress(compressed);

        double error = 0;
        for (int i = 0; i < data.size(); ++i)
            error += std::abs(data[i] - decompressed[i]);

        std::cout << "Greška za " << size << ": " << error << "\n";
    }
}

int main(){
    Testiraj();
    return 0;
}