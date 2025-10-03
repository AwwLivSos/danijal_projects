//ja ne znam sto se ne moze na zamgeru kompajlirat, na ostalim mjestima je sasvim ok

#include <algorithm>
#include <cmath>
#include <complex>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

enum RegulaFalsiMode { Unmodified, Illinois, Slavic, IllinoisSlavic };
using namespace std; //!!

template <typename FunType>
bool BracketRoot(FunType f, double x0, double &a, double &b, double hinit = 1e-5, double hmax = 1e10, double lambda = 1.4) {

    if (hinit <= 0 || hmax <= 0 || lambda <= 0) throw domain_error("Invalid parameters");

    double a2 = x0, f1 = f(a2), h = hinit;
    bool korijen = false;

    while (fabs(h) < hmax) {
      double b2 = a2 + h;
      double f2 = f(b2);

      if (f1 * f2 <= 0) {
        if (a2 > b2) swap(a2, b2);
        a = a2;
        b = b2;
        korijen = true;
        break;
      }

      h = lambda * h;
      a2 = b2;
      f1 = f2;
    }

    h *= -1;

    a2 = x0, f1 = f(a2), h = hinit;
    while (fabs(h) < hmax) {
        double b2 = a2 - h;
        double f2 = f(b2);

        if (f1 * f2 <= 0) {
        if (a2 > b2) swap(a2, b2);
        a = a2;
        b = b2;
        korijen = true;
        break;
        }

        h = lambda * h;
        b2 = a2;
        f2 = f1;
    }

  return korijen;
}

template <typename FunType>
double RegulaFalsiSolve(FunType f, double a, double b, RegulaFalsiMode mode = Slavic, double eps = 1e-10, int maxiter = 100) {
    if (f(a) * f(b) > 0) throw range_error("Root must be bracketed");
    if (eps < 0 || maxiter < 0) throw domain_error("Invalid parameters");

    double f1, f2, c, staroC;
    int count;

    if (mode == Unmodified) {
        f1 = f(a);
        f2 = f(b);
        c = a;
        staroC = b;
        count = 0;
        while (fabs(c - staroC) > eps) {
        if (count == maxiter)
            throw logic_error("Given accuracy has not achieved");           //gramatika
        staroC = c;
        c = (a * f2 - b * f1) / (f2 - f1);
        double f3 = f(c);
        if (fabs(f3) < eps)
            return c;
        if (f1 * f3 < 0) {
            b = a;
            f2 = f1;
        }
        a = c;
        f1 = f3;
        count++;
        }
        return c;
    }

    else if (mode == Illinois) {
        f1 = f(a);
        f2 = f(b);
        c = a;
        staroC = b;
        count = 0;
        while (fabs(c - staroC) > eps) {
            if (count == maxiter) throw logic_error("Given accuracy has not achieved");         //gramatika
            staroC = c;
            c = (a * f2 - b * f1) / (f2 - f1);
            double f3 = f(c);
            if (fabs(f3) < eps) return c;
            if (f1 * f3 < 0) {
                b = a;
                f2 = f1;
            } else f2 = f2 / 2;
            
            a = c;
            f1 = f3;
            count++;
        }
        return c;
    }

    else if (mode == IllinoisSlavic) {
        function<double(double)> slavic;
        slavic = [](double x) { return x / (1 + fabs(x));};

        f1 = slavic(f(a));
        f2 = slavic(f(b));
        c = a;
        staroC = b;
        count = 0;
        while (fabs(c - staroC) > eps) {
            if (count == maxiter) throw logic_error("Given accuracy has not achieved");         //gramatika
            staroC = c;
            c = (a * f2 - b * f1) / (f2 - f1);
            double f3 = slavic(f(c));
            if (fabs(f3) < eps)
                return c;
            if (f1 * f3 < 0) {
                b = a;
                f2 = f1;
            } else f2 = f2 / 2;
            
            a = c;
            f1 = f3;
            count++;
        }
        return c;
    }

    function<double(double)> slavic;
    slavic = [](double x) { return x / (1 + fabs(x)); };

    f1 = slavic(f(a));
    f2 = slavic(f(b));
    c = a;
    staroC = b;
    count = 0;
    while (fabs(c - staroC) > eps) {
        if (count == maxiter)
        throw logic_error("Given accuracy has not achieved");       //gramatika
        staroC = c;
        c = (a * f2 - b * f1) / (f2 - f1);
        double f3 = slavic(f(c));
        if (fabs(f3) < eps)
        return c;
        if (f1 * f3 < 0) {
            b = c;
            f2 = f3;
        } else {
            a = c;
            f1 = f3;
        }
        count++;
    }
    return c;
}

template <typename FunType>
double RiddersSolve(FunType f, double a, double b, double eps = 1e-10, int maxiter = 100) {
    if (f(a) * f(b) > 0) throw range_error("Root must be bracketed");
    if (eps < 0 || maxiter < 0) throw domain_error("Invalid parameters");
    double f1 = f(a), f2 = f(b);
    int count = 0;

    while (fabs(b - a) > eps) {
        if (count == maxiter) throw logic_error("Given accuracy has not achieved");         //gramatika
        double c = (a + b) / 2;
        double f3 = f(c);

        if (fabs(f3) < eps)return c;

        int sign;
        if ((f1 - f2) > 0) sign = 1;
        else if ((f1 - f2) < 0) sign = -1;
        else sign = 0;

        double d = c + (f3 * (c - a) * sign) / sqrt(f3 * f3 - f1 * f2);
        double f4 = f(d);
        if (fabs(f4) < eps) return d;
        if (f3 * f4 <= 0) {
            a = c;
            b = d;
            f1 = f3;
            f2 = f4;
        } else if (f1 * f4 <= 0) {
            b = d;
            f2 = f4;
        } else {
            a = d;
            f1 = f4;
        }
        count++;
    }
    return (a+b)/2;
}

template <typename FunType1, typename FunType2>
double NewtonRaphsonSolve(FunType1 f, FunType2 fprim, double x0, double eps = 1e-10, double damping = 0, int maxiter = 100) {
    if (eps < 0 || maxiter < 0) throw domain_error("Invalid parameters");
    else if (damping < 0 || damping >= 1) throw domain_error("Invalid parameters");

    double delta_x = numeric_limits<double>::infinity();
    double v = f(x0), d = fprim(x0);
    int count = 0;

    while (fabs(delta_x) > eps) {
        if (fabs(fprim(x0)) < eps || count == maxiter || !isfinite(x0)) throw logic_error("Convergence has not achieved");
        if (fabs(v) <= eps) return x0;

        delta_x = v/d;
        double w = v;
        v = f(x0 - delta_x);
        d = fprim(x0 - delta_x);

        while (fabs(v) > fabs(w) || !isfinite(v) || d == 0) {
            delta_x *= damping;
            v = f(x0 - delta_x);
            d = fprim(x0 - delta_x);
            if (fabs(delta_x) < eps) throw logic_error("Convergence has not achieved");     //gramatika
        }

        x0 -= delta_x;
        count++;
    }
    return x0;
}

//par korisnih operatora za realizaciju funkcija
bool operator==(complex<double> c1, complex<double> c2) {
    return (fabs(c1.real() - c2.real()) < numeric_limits<double>::epsilon() &&
            fabs(c1.imag() - c2.imag()) < numeric_limits<double>::epsilon());
}

complex<double> operator*(complex<double> c1,  complex<double> c2) {
    return {c1.real() * c2.real() - c1.imag() * c2.imag(), c1.real() * c2.imag() + c1.imag() * c2.real()};
}

complex<double> operator*(double x, complex<double> c) {
    complex<double> temp(x, 0);
    return temp * c;
}

inline complex<double> operator*(complex<double> c, double x) {
  return x * c;
}

pair<complex<double>, bool> Laguerre(vector<double> p, int n, complex<double> x, double eps, int maxiter) {
    complex<double> delta_x = numeric_limits<double>::infinity();
    int k = 1;

    while (fabs(delta_x) > eps && k < maxiter) {
        complex<double> f = p[n];
        complex<double> d = 0;
        complex<double> s = 0;

        for (int i(n - 1); i >= 0; i--) {
            s = s * x + 2 * d;
            d = d * x + f;
            f = f * x + p[i];
        }
        if (f == 0) return {x, true};

        complex<double> r = sqrt((n - 1) * ((n - 1) * d * d - n * f * s));
        if (fabs(d + r) > fabs(d - r)) delta_x = n * f / (d + r);
        else delta_x = n * f / (d - r);

        x -= delta_x;
        k++;
    }

    if (fabs(delta_x) <= eps) return {x, true};

    return {x, false};
}

pair<complex<double>, bool> Laguerre(vector<complex<double>> p, int n, complex<double> x, double eps, int maxiter) {
    complex<double> delta_x = numeric_limits<double>::infinity();

    int k = 1;
    while (fabs(delta_x) > eps && k < maxiter) {
        complex<double> f = p[n];
        complex<double> d = 0;
        complex<double> s = 0;

        for (int i(n - 1); i >= 0; i--) {
            s = s * x + 2 * d;
            d = d * x + f;
            f = f * x + p[i];
        }
        if (f == 0) return {x, true};
        complex<double> r = sqrt((n - 1) * ((n - 1) * d * d - n * f * s));
        if (fabs(d + r) > fabs(d - r)) delta_x = n * f / (d + r);
        else delta_x = n * f / (d - r);

        x -= delta_x;
        k++;
    }
    if (fabs(delta_x) <= eps) return {x, true};

    return {x, false};
}

vector<complex<double>> PolyRoots(vector<complex<double>> coefficients, double eps = 1e-10, int maxiters = 100, int maxtrials = 10) {
    if (eps < 0 || maxiters < 0 || maxtrials < 0)  throw domain_error("Invalid parameters");

    int n = coefficients.size() - 1, i = n, count = n;
    vector<complex<double>> v;

    while (i >= 1) {
        if (count == maxiters) throw logic_error("Convergence has not achieved");       //gramatika
        int t = 1;
        bool var(false);
        complex<double> x;
        while (!var && (t < maxtrials)) {
            x = {0, 0};
            pair<complex<double>, bool> pair = Laguerre(coefficients, i, x, eps, maxiters);
            var = pair.second;
            x = pair.first;
            t++;
        }
        if (!var) throw logic_error("Convergence has not achieved");            //gramatika
        if (abs(x.imag()) <= eps) x = x.real();

        v.push_back(x);
        complex<double> v = coefficients[i];

        for (int j = i - 1; j >= 0; j--) {
            complex<double> w = coefficients[j];
            coefficients[j] = v;
            v = w + x * v;
        }
        i--;
        count++;
    }

    sort(v.begin(), v.end(), [](complex<double> x, complex<double> y) {
                if (x.real() < y.real()) return true;
                else if (x.real() > y.real()) return false;
                return x.imag() < y.imag();
                });
    return v;
}

vector<complex<double>> PolyRoots(vector<double> coefficients,  double eps = 1e-10, int maxiters = 100, int maxtrials = 10) {
    if (eps < 0 || maxiters < 0 || maxtrials < 0) throw domain_error("Invalid parameters");

    int n = coefficients.size() - 1, i = n, it = 0;
    vector<complex<double>> v(n + 1);
    while (i >= 1) {
        if (it == maxiters) throw logic_error("Convergence has not achieved");          //gramatika
        int t = 1;
        bool var(false);
        complex<double> x;
        while (!var && (t < maxtrials)) {
            x = {1, 1};
            pair<complex<double>, bool> pair = Laguerre(coefficients, i, x, eps, maxiters);
            var = pair.second;
            x = pair.first;
            t++;
        }
        if (!var) throw logic_error("Convergence has not achieved");            //gramatika
        if (abs(x.imag()) <= eps) {
            x = x.real();
            v[i] = x;
            double v = coefficients[i];
            for (int j = i - 1; j >= 0; j--) {
                double w = coefficients[j];
                coefficients[j] = v;
                v = w + x.real() * v;
            }
            i--;
        } 
        else {
            v[i] = x;
            v[i - 1] = conj(x);
            double alpha = 2 * x.real(), beta = abs(x);
            beta *= beta;
            double u = coefficients[i];
            double v = coefficients[i - 1] + alpha * u;
            for (int j = i - 2; j >= 0; j--) {
                double w = coefficients[j];
                coefficients[j] = u;
                u = v;
                v = w + alpha * v - beta * coefficients[j];
            }
            i -= 2;
        }
        it++;
    }
    v.erase(v.begin());
    sort(v.begin(), v.end(), [](complex<double> c1, complex<double> c2) {
                if (abs(c1.real() - c2.real()) > numeric_limits<double>::epsilon()) return c1.real() < c2.real();
                return c1.imag() < c2.imag();
                });

    return v;
}

void Testiraj(){
    try {
        auto f1 = [](double x) { return x * x - 4; }; //x = -2, 2
        double a, b;
        if (BracketRoot(f1, 0.0, a, b)) {
            cout<<"BracketRoot Test 1: Root bracketed between [" << a << ", " << b << "]\n";
        } else {
            cout<<"BracketRoot Test 1: Failed to bracket the root.\n";
        }

        double root = RegulaFalsiSolve(f1, a, b, Unmodified);
        cout<<"\nRegulaFalsiSolve Test 2: x = " << root << "\n";

        auto f2 = [](double x) { return x * x * x - x - 2; }; //x = 1.521
        double ridderRoot = RiddersSolve(f2, 1.0, 2.0);
        cout<<"\nRiddersSolve Test 3: x = " << ridderRoot << "\n";

        auto f3 = [](double x) { return sin(x) - 0.5; }; //x = 0.5236 (pi/6)
        auto f3_derivative = [](double x) { return cos(x); };

        double newtonRoot = NewtonRaphsonSolve(f3, f3_derivative, 0.5);
        cout<<"\nNewtonRaphsonSolve Test 4: x = " << newtonRoot << "\n";

        vector<double> poly1 = {1, -3, 2}; //x = 1, 2
        vector<complex<double>> roots = PolyRoots(poly1);
        cout<<"\nPolyRoots Test 5: Roots found:\n";
        for (const auto &root : roots) {
            cout << root << "\n";
        }

        vector<double> poly2 = {1, -6, 11, -6}; //x = 1, 2, 3
        vector<complex<double>> laguerreRoots = PolyRoots(poly2);
        cout<<"\nLaguerre Test 6: Roots found:\n";
        for (const auto &root : laguerreRoots) {
            cout << root << "\n";
        }

        vector<double> poly3 = {1, 0, 1}; // i, -i
        vector<complex<double>> complexRoots = PolyRoots(poly3);
        cout<<"\nPolyRoots Test 7: Roots found:\n";
        for (const auto &root : complexRoots) {
            cout << root << "\n";
        }

    } 
    catch (const exception &e) {
        cout<<"\nAn error occurred: " << e.what() << endl;
    }
}

int main() {
    Testiraj();
    //ma bas lijepo sjajno sve radi
    return 0;
}