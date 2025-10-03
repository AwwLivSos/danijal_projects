#include <cmath>
#include <limits>
#include <utility>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <functional>

const double PI = 4 * std::atan(1);
const double INF = std::numeric_limits<double>::infinity();

using VektorParova = std::vector<std::pair<double, double>>;

template <typename FunType>
double RK4Step(FunType f, double x0, double y0, double h) {
    double K1 = f(x0, y0);
    double K2 = f(x0 + h / 2, y0 + h * K1 / 2);
    double K3 = f(x0 + h / 2, y0 + h * K2 / 2);
    double K4 = f(x0 + h, y0 + h * K3);
    return y0 + h * (K1 + 2 * K2 + 2 * K3 + K4) / 6;
}

template <typename FunType>
VektorParova RK4Integrator(FunType f, double x0, double y0, double xmax, double h, double eps = 1e-8, bool adaptive = false) {
    if (eps <= 0) throw std::domain_error("Invalid parameters");

    VektorParova v;
    double x = x0, y = y0;

    if ((h > 0 && xmax < x0) || (h < 0 && xmax > x0)) {
        v.push_back({x, y});
        return v;
    }

    if (!adaptive) {
        while ((h > 0 && x < xmax) || (h < 0 && x > xmax)) {
            v.push_back({x, y});
            if ((h > 0 && x + h >= xmax) || (h < 0 && x + h <= xmax)) h = xmax - x;
            y = RK4Step(f, x, y, h);
            x += h;
        }
        v.push_back({x, y});
        return v;
    }

    int iteration_limit = 100;
    int iteration_count = 0;
    while ((h > 0 && x <= xmax) || (h < 0 && x >= xmax)) {
        double half_step_1 = RK4Step(f, x, y, h / 2);
        double half_step_2 = RK4Step(f, x + h / 2, half_step_1, h / 2);
        double full_step = RK4Step(f, x, y, h);

        double error_estimate = std::fabs((full_step - half_step_2) / h);

        if (error_estimate <= eps) {
            if ((h > 0 && x + h > xmax) || (h < 0 && x + h < xmax)) {
                h = xmax - x; 
                continue;
            }
            x += h;
            y = half_step_2;
            v.push_back({x, y});
        }

        h *= std::min(5.0, 0.9 * std::pow(eps / error_estimate, 0.25));

        if (++iteration_count > iteration_limit) {
            throw std::runtime_error("Adaptive step size integration exceeded iteration limit");
        }
    }

    return v;
}

template <typename FunType>
std::vector<double> RK4SystemStep(FunType f, double x, const std::vector<double>& y, double h) {
    size_t n = y.size();
    std::vector<double> K1(n), K2(n), K3(n), K4(n), y_temp(n);

    K1 = f(x, y);
    for (size_t i = 0; i < n; i++) y_temp[i] = y[i] + h * K1[i] / 2;
    K2 = f(x + h / 2, y_temp);
    for (size_t i = 0; i < n; i++) y_temp[i] = y[i] + h * K2[i] / 2;
    K3 = f(x + h / 2, y_temp);
    for (size_t i = 0; i < n; i++) y_temp[i] = y[i] + h * K3[i];
    K4 = f(x + h, y_temp);

    std::vector<double> y_next(n);
    for (size_t i = 0; i < n; i++) {
        y_next[i] = y[i] + h * (K1[i] + 2 * K2[i] + 2 * K3[i] + K4[i]) / 6;
    }

    return y_next;
}

std::vector<double> dodajVektor(const std::vector<double>& a, const std::vector<double>& b) {
    if (a.size() != b.size()) {
        throw std::range_error("Incompatible formats");
    }
    std::vector<double> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}

std::vector<double> skalirajVektor(const std::vector<double>& v, double scalar) {
    std::vector<double> result(v.size());
    for (size_t i = 0; i < v.size(); ++i) {
        result[i] = v[i] * scalar;
    }
    return result;
}

template <typename FunType>
std::vector<std::pair<double, std::vector<double>>> RK4SystemIntegrator(
    FunType f, double x0, const std::vector<double>& y0, double xmax, double h) 
{
    if (y0.empty()) {
        throw std::range_error("Incompatible formats");
    }

    std::vector<std::pair<double, std::vector<double>>> solutions;
    double x = x0;
    std::vector<double> y = y0;

    solutions.push_back({x, y});

    const double tolerance = std::abs(h) / 100;

    while ((h > 0 && x <= xmax + tolerance) || (h < 0 && x >= xmax - tolerance)) {
        std::vector<double> k1 = f(x, y);

        if (k1.size() != y.size()) {
            throw std::range_error("Incompatible formats");
        }

        std::vector<double> k2 = f(x + h / 2.0, dodajVektor(y, skalirajVektor(k1, h / 2.0)));
        std::vector<double> k3 = f(x + h / 2.0, dodajVektor(y, skalirajVektor(k2, h / 2.0)));
        std::vector<double> k4 = f(x + h, dodajVektor(y, skalirajVektor(k3, h)));

        y = dodajVektor(y, skalirajVektor(dodajVektor(dodajVektor(skalirajVektor(k1, 1.0), skalirajVektor(k2, 2.0)), dodajVektor(skalirajVektor(k3, 2.0), skalirajVektor(k4, 1.0))), h / 6.0));

        x += h;
        solutions.emplace_back(x, y);           //ubaga kad bude obicni push_back
    }

    return solutions;
}

template <typename FunType>
std::pair<double, bool> RombergIntegration(FunType f, double a, double b, double eps = 1e-8, int nmax = 1000000, int nmin = 50){
    if (eps < 0 || nmin < 0 || nmax < 0 || nmax < nmin) throw std::domain_error("Bad parameter");

    int podjele = 2;
    double korak = (b - a) / podjele;
    double sum = (f(a) + f(b)) / 2, prev_integral = sum;
    std::vector<double> integrali(nmax);

    for (int i = 1; podjele <= nmax; i++) {
        for (int j = 1; j <= podjele / 2; j++)
            sum += f(a + (2 * j - 1) * korak);

        integrali[i - 1] = korak * sum;
        double power = 4;
        for (int k = i - 1; k >= 1; k--) {
            integrali[k - 1] = (power * integrali[k] - integrali[k - 1]) / (power - 1);
            power *= 4;
        }

        if (std::fabs(integrali[0] - prev_integral) <= eps) return {integrali[0], true};
        prev_integral = integrali[0];
        korak /= 2;
        podjele *= 2;
    }

    return {prev_integral, 0};
}

template <typename FunType>
std::pair<double, bool> TanhSinhIntegration(FunType f, double a, double b, double eps = 1e-8, int nmax = 1000000, int nmin = 20, double range = 3.5){
    if (eps < 0 || nmin < 0 || nmax < 0 || nmax < nmin || range < 0) throw std::domain_error("Bad parameter");

    int podjele = 2;
    double korak = 2 * range / podjele;
    double midpoint = (b + a) / 2;
    double half_range = (b - a) / 2;
    double sum = 0, prev_integral = sum;

    while (podjele < nmax) {
        for (int i = 1; i <= podjele / 2; i++) {
            double t = -range + (2 * i - 1) * korak;
            double u = PI * std::sinh(t) / 2;
            double value = f(midpoint + half_range * std::tanh(u));
            if (std::isfinite(value)) sum += half_range * PI * std::cosh(t) * value / (2 * std::cosh(u) * std::cosh(u));
        }

        double integral = korak * sum;
        if (podjele >= nmin && std::fabs(integral - prev_integral) <= eps)
            return {integral, true};

        prev_integral = integral;
        podjele *= 2;
        korak /= 2;
    }

    return {prev_integral, 0};
}

template <typename FunType>
double AdaptiveAux(FunType func, double start, double end, double eps, double f1, double f2, double f3, int depth, bool &w) {
    if (!std::isfinite(f1)) f1 = 0;
    if (!std::isfinite(f2)) f2 = 0;
    if (!std::isfinite(f3)) f3 = 0;

    double midpoint = (start + end) / 2;
    double prvi_integral = (end - start) * (f1 + 4 * f3 + f2) / 6;
    double f4 = func((start + midpoint) / 2);
    double f5 = func((midpoint + end) / 2);
    double drugi_integral = (end - start) * (f1 + 4 * f4 + 2 * f3 + 4 * f5 + f2) / 12;

    if (std::fabs(prvi_integral - drugi_integral) <= eps) return drugi_integral; 

    if (depth <= 0) {
        w = false;
        return drugi_integral;
    }

    return AdaptiveAux(func, start, midpoint, eps, f1, f3, f4, depth - 1, w) + AdaptiveAux(func, midpoint, end, eps, f3, f2, f5, depth - 1, w);
}

template <typename FunType>
std::pair<double, bool> AdaptiveIntegration(FunType f, double a, double b, double eps = 1e-10, int maxdepth = 30, int nmin = 1){
    if (maxdepth < 0 || eps < 0 || nmin < 0) throw std::domain_error("Bad parameter");

    bool w = true;
    double result = 0;
    double korak = (b - a) / nmin;

    for (int i = 0; i < nmin; i++) {
        double sub_a = a + i * korak;
        double sub_b = sub_a + korak;
        result += AdaptiveAux(f, sub_a, sub_b, eps, f(sub_a), f(sub_b), f((sub_a + sub_b) / 2), maxdepth, w);
    }

    return {result, w};
}

void TestIntegration(const std::function<double(double)>& func, double a, double b, const std::string& name, double expected) {
    auto romberg = RombergIntegration(func, a, b);
    auto takahashi = TanhSinhIntegration(func, a, b);
    auto adaptive = AdaptiveIntegration(func, a, b);

    std::cout << "Testiranje " << name << " na intervalu [" << a << ", " << b << "]:\n";
    std::cout << "Romberg: " << romberg.first << ", konvergira: " << romberg.second << "\n";
    std::cout << "Takahashi: " << takahashi.first << ", konvergira: " << takahashi.second << "\n";
    std::cout << "Adaptivno: " << adaptive.first << ", konvergira: " << adaptive.second << "\n";
    std::cout << "Ocekivano: " << expected << "\n\n";
}

void Integriraj() {
    const double PI = std::atan(1) * 4;

    TestIntegration([](double x) { return std::sin(x); }, 0, PI, "sin(x)", 2);
    TestIntegration([](double x) { return 1 / std::sqrt(x); }, 0, 1, "1/sqrt(x)", 2);
    TestIntegration([](double x) { return std::exp(x); }, 0, 1, "exp(x)", std::exp(1) - 1);
    TestIntegration([](double x) { return x * x; }, 0, 1, "x^2", 1.0 / 3.0);
}


int main() {
    Integriraj();
    return 0;
}
