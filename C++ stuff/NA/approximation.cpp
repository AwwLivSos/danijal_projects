#include <stdexcept>
#include <vector>
#include <cmath>
#include <iostream>

class ChebyshevApproximation{
    std::vector<double> koef;
    int m, n;
    double min, max;
    ChebyshevApproximation(const std::vector<double> &koef, double min, double max) : m(koef.size() - 1), min(min), max(max), koef(koef) {}    //bonus konstruktor
public:
    template <typename FunType> 
        ChebyshevApproximation(FunType f, double xmin, double xmax, int n);
    void set_m(int m);
    void trunc(double eps);
    double operator()(double x) const;
    double derivative(double x) const;
    ChebyshevApproximation derivative() const;
    ChebyshevApproximation antiderivative() const;
    double integrate(double a, double b) const;
    double integrate() const;
};

template<typename FunType>
ChebyshevApproximation::ChebyshevApproximation(FunType f, double xmin, double xmax, int n) : min(xmin), max(xmax), n(n), m(n), koef(n+1){
    if(xmin>=xmax || n<1) throw std::domain_error("Bad parameters");

    const double PI = 3.14159265359;

    std::vector<double> w(n + 1), v(n + 1);
    for (int i = 0; i <= n; i++) {
        w[i] = PI * (2 * i + 1) / (2 * n + 2);
        v[i] = f((xmin + xmax + (xmax - xmin) * std::cos(w[i])) / 2);
    }

    for (int k = 0; k <= n; k++) {
        double s = 0;
        for (int i = 0; i <= n; i++) {
            s += v[i] * std::cos(k * w[i]);
        }
        koef[k] = 2 * s / (n + 1);
    }
}

void ChebyshevApproximation::set_m(int m){
    if(m> this->m || m<=1) throw std::domain_error("Bad order");
    this->m=m;
}

void ChebyshevApproximation::trunc(double eps){
    if (eps < 0) throw std::domain_error("Bad tolerance");

    for (int i = m; i >= 0; i--) {
        if (std::fabs(koef[i]) > eps) {
            m = i;
            return;
        }
    }
    throw std::domain_error("Bad tolerance");
}

double ChebyshevApproximation::derivative(double x) const{
    if(x<min || x>max) throw std::domain_error("Bad argument");
    double t = (2*x - min - max) / (max-min), 
    p=1,
    q=4*t,                  //horor
    s=koef[1]+q*koef[2],
    r{};
    for(int k=3; k<=m; k++){
        r=k*((2*t*q)/(k-1)- p/(k-2));
        s+=koef[k]*r;
        p=q;
        q=r;
    }
    return 2*s/(max-min);
}

ChebyshevApproximation ChebyshevApproximation::derivative() const{
    double mi = 4./(max-min);
    std::vector<double> v(m+1);

    v[m-1]=mi*m*koef[m];
    v[m-2]=mi*(m-1)*koef[m-1];

    for(int k=m-3; k>=0; k--) v[k] = v[k+2] + mi*(k+1)*koef[k+1];
    return ChebyshevApproximation(v,min,max);
}

ChebyshevApproximation ChebyshevApproximation::antiderivative() const{
    double mi = (max-min)/4;
    std::vector<double> v(m+1);

    v[0]=0;
    for (int k = 1; k<m; k++) v[k] = mi * (koef[k-1] - koef[k+1]) / k;

    v[m] = mi* koef[m - 1] / m;
    v.push_back(mi * koef[m] / (m + 1));
    return ChebyshevApproximation(v,min,max);
}

double ChebyshevApproximation::integrate(double a, double b) const{
    if (a < min || b > max || a > b) throw std::domain_error("Bad interval");
    return antiderivative()(b) - antiderivative()(a);
}

double ChebyshevApproximation::integrate() const{
    return integrate(min, max);
}

double ChebyshevApproximation::operator()(double x) const{
    if (x < min || x > max) throw std::domain_error("Bad argument");

    double t = (2 * x - min - max) / (max - min), 
    p = 1, q = t,                                 
    s = koef[0] / 2 + koef[1] * t, r;                   

    for (int k = 2; k <= m; k++) {
        r = 2 * t * q - p;
        s += koef[k] * r;
        p = q;
        q = r;
    }
    return s;
}

void test_sin_function() {
    ChebyshevApproximation approx([](double x) { return std::sin(x); }, 0, M_PI, 10);

    std::cout << "Test za sinus na intervalu [0, PI]:\n";

    std::cout << "Vrijednosti:\n";
    for (double x = 0; x <= M_PI; x += M_PI / 10) {
        double approx_val = approx(x);
        double exact_val = std::sin(x);
        std::cout << "x = " << x << ", priblizno = " << approx_val << ", tacno = " << exact_val << ", greska = " << std::fabs(approx_val - exact_val) << '\n';
    }

    std::cout << "\nIzvodi:\n";
    for (double x = 0; x <= M_PI; x += M_PI / 10) {
        double approx_deriv = approx.derivative(x);
        double exact_deriv = std::cos(x);
        std::cout << "x = " << x << ", priblizno = " << approx_deriv << ", tacno = " << exact_deriv << ", greska = " << std::fabs(approx_deriv - exact_deriv) << '\n';
    }

    std::cout << "\nIntegral:\n";
    double approx_integral = approx.integrate();
    double exact_integral = -std::cos(M_PI) + std::cos(0);
    std::cout << "Priblizan integral = " << approx_integral << ", tacan integral = " << exact_integral << ", greska = " << std::fabs(approx_integral - exact_integral) << '\n';
}

void test_exp_function() {
    ChebyshevApproximation approx([](double x) { return std::exp(x); }, 0, 1, 10);

    std::cout << "\nTest za eksponencijalnu funkciju na intervalu [0, 1]:\n";

    std::cout << "Vrijednosti:\n";
    for (double x = 0; x <= 1; x += 0.1) {
        double approx_val = approx(x);
        double exact_val = std::exp(x);
        std::cout << "x = " << x << ", priblizno = " << approx_val << ", tacno = " << exact_val << ", greska = " << std::fabs(approx_val - exact_val) << '\n';
    }

    std::cout << "\nIzvodi (naravno, trebali bi biti isti):\n";
    for (double x = 0; x <= 1; x += 0.1) {
        double approx_deriv = approx.derivative(x);
        double exact_deriv = std::exp(x);
        std::cout << "x = " << x << ", priblizno = " << approx_deriv << ", tacno = " << exact_deriv << ", greska = " << std::fabs(approx_deriv - exact_deriv) << '\n';
    }

    std::cout << "\nIntegral (isto kao za izvode):\n";
    double approx_integral = approx.integrate();
    double exact_integral = std::exp(1) - std::exp(0);
    std::cout << "Priblizan integral = " << approx_integral << ", tacan integral = " << exact_integral << ", greska = " << std::fabs(approx_integral - exact_integral) << '\n';
}

int main(){
    test_sin_function();
    test_exp_function();

    return 0;
}