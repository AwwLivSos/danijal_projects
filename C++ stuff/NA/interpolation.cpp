#include <limits>
#include <stdexcept>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <iostream>

using VektorParova=std::vector<std::pair<double, double>>;

class AbstractInterpolator{
    VektorParova tacke;
    mutable int spec;
protected:
    int Locate(double x) const;
    VektorParova DajOGTacke() const{return tacke;}
public:
    AbstractInterpolator(const VektorParova &data) : tacke(data), spec(-1){
        std::sort(tacke.begin(), tacke.end(), [](const std::pair<double, double> &t1, const std::pair<double, double> &t2){
            if(std::fabs(t1.first-t2.first) < 10 * std::numeric_limits<double>::epsilon()) throw std::domain_error("Invalid data set");
            return t1.first<t2.first;
        });
    }
    virtual double operator()(double x) const = 0;
};

int AbstractInterpolator::Locate(double x) const{
    int n = tacke.size();

    if(x<=tacke[0].first) {spec=0; return spec;}
    if(x>tacke[n-1].first) {spec=n; return spec;}

    if(spec>0 && spec<n){
        if (x>tacke[spec-1].first && x<=tacke[spec].first) return spec;
        if (spec>1 && x>tacke[spec-2].first && x<=tacke[spec-1].first) return --spec;
        if (spec<n-1 && x>tacke[spec].first && x<=tacke[spec+1].first) return ++spec;
    }

    auto it = std::lower_bound(tacke.begin(), tacke.end(), x, [](const std::pair<double, double> &t1, const double &t2){
        return t1.first<t2;
    });

    spec=std::distance(tacke.begin(), it);

    return spec;
}

class LinearInterpolator : public AbstractInterpolator{
public:
    LinearInterpolator(const VektorParova &data) : AbstractInterpolator(data){}
    double operator()(double x) const override;
};

double LinearInterpolator::operator()(double x) const{
    std::pair<double, double> t1, t2;
    int k = Locate(x);
    auto temp = DajOGTacke();
    int n=temp.size();

    if(k==0) {t1=temp[0]; t2=temp[1];}
    else if(k==n) {t1=temp[n-2]; t2=temp[n-1];}
    else{t1=temp[k-1]; t2=temp[k];}

    return ((t2.first-x)/(t2.first-t1.first))*t1.second + ((x-t1.first)/(t2.first-t1.first))*t2.second;
};

template <typename FpType> bool fpequal(FpType x, FpType y) {       //KOJI K****?!??
    FpType eps = 10 * std::numeric_limits<FpType>::epsilon() * (std::fabs(x) + std::fabs(y));
    return std::fabs(x - y) <= eps;
}

class PolynomialInterpolator : public AbstractInterpolator {
  VektorParova v;               //stvarno nije bilo potrebe za ovim 
  std::vector<double> njutnovi;
  std::vector<double> pomocni;
public:
  PolynomialInterpolator(const std::vector<std::pair<double, double>> &data) : AbstractInterpolator(data), v(DajOGTacke()), pomocni(data.size()) {
        for (const auto &xy : data) njutnovi.push_back(xy.second);

        pomocni[0] = v[v.size() - 1].second;
        for (int j = 1; j <= data.size() - 1; j++) {
            for (int i = data.size(); i >= j + 1; i--) {
                njutnovi[i - 1] = (njutnovi[i - 1] - njutnovi[i - 2]) / (v[i - 1].first - v[i - j - 1].first);
            }
            pomocni[j] = njutnovi[njutnovi.size() - 1];
        }
  }

  void AddPoint(const std::pair<double, double> &p) {
    for (std::pair<double, double> xy : v)
      if (fpequal(xy.first, p.first))
        throw std::domain_error("Invalid point");

    pomocni.push_back(0);
    v.push_back(p);
    double old_helper = pomocni[0];
    pomocni[0] = p.second;

    for (int j = 1; j <= v.size() - 1; j++) {
      double temp = pomocni[j];
      pomocni[j] = (pomocni[j - 1] - old_helper) / (v[v.size() - 1].first - v[v.size() - j - 1].first);
      old_helper = temp;
    }
    njutnovi.push_back(pomocni[v.size() - 1]);
  }

  std::vector<double> GetCoefficients() const {
    int n = v.size();
    std::vector<double> w(n + 1), p(n);

    w[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++)
            p[j] += njutnovi[i - 1] * w[j];
        w[i] = w[i - 1];
        for (int j = i - 1; j > 0; j--)
            w[j] = w[j - 1] - v[i - 1].first * w[j];
        w[0] *= -v[i - 1].first;
    }

    return p;
  }

  double operator()(double x) const override {

    double s = njutnovi[0], f = 1;
    for (int j = 1; j <= njutnovi.size() - 1; j++) {
      f *= (x - this->v[j - 1].first);
      s += njutnovi[j] * f;
    }
    return s;
  }
};

class PiecewisePolynomialInterpolator : public AbstractInterpolator{
    int order;
public: 
    PiecewisePolynomialInterpolator(const VektorParova &data, int order) : AbstractInterpolator(data), order(order){
        if(order<1 || order>=DajOGTacke().size()) throw std::domain_error("Invalid order");
    }
    double operator()(double x) const override;
};

double PiecewisePolynomialInterpolator::operator()(double x) const{
    int k=Locate(x), n=DajOGTacke().size(), recht, links;
    std::vector<double> v1, v2;

    if(order%2==0){links=k-(order/2); recht=k+(order/2);}
    else{links=k-(order-1)/2; recht=k+(order+1)/2;}

    if (links < 1) for (int i=0; i<order+1; i++) {v1.push_back(DajOGTacke()[i].first); v2.push_back(DajOGTacke()[i].second);}
    //izuzetno pregledno...
    else if (recht > n) for (int i=n-order-1; i<n; i++) {v1.push_back(DajOGTacke()[i].first); v2.push_back(DajOGTacke()[i].second);}
    //haj bar je skrati linije
    else for (int i=links-1; i < recht; i++) {v1.push_back(DajOGTacke()[i].first); v2.push_back(DajOGTacke()[i].second);}

    n=v2.size();
    double s=0, p=1;
    for(int i=0; i<n; i++){
        p=v2[i];
        for(int j=0; j<n; j++)
            if(j!=i) p*=((x-v1[j]) / (v1[i]-v1[j]));
        s+=p;
    }

    return s;
} 

class SplineInterpolator : public AbstractInterpolator{
    std::vector<double> q, s, r;
public:
    SplineInterpolator(const VektorParova &data);
    double operator()(double x) const override;
};

SplineInterpolator::SplineInterpolator(const VektorParova &data) : AbstractInterpolator(data), q(data.size()), s(data.size()), r(data.size()){
    int n=data.size();
    auto splajn = DajOGTacke();
    for (int i=1; i<=n-2; i++) {
        s[i]=2*(splajn[i+1].first-splajn[i-1].first);
        r[i]=3*(((splajn[i+1].second-splajn[i].second) / (splajn[i+1].first-splajn[i].first)) - ((splajn[i].second-splajn[i-1].second) / (splajn[i].first-splajn[i-1].first)));
    }

    for (int i=1; i<=n-3; i++) {
        double mi=(splajn[i+1].first-splajn[i].first) / s[i];
        s[i+1]-=mi*(splajn[i + 1].first-splajn[i].first);
        r[i+1]-=mi*r[i];                                            //splajn ne valja nesto
    }

    r[n-2]/=s[n-2];
    for (int i=n-3; i>=1; i--) r[i] = (r[i]-(splajn[i+1].first-splajn[i].first) * r[i+1]) / s[i];

    for (int i = 0; i <= n - 2; i++) {
        double dx=splajn[i+1].first-splajn[i].first;
        s[i]=(r[i+1]-r[i])/(3*dx);
        q[i]=(splajn[i+1].second-splajn[i].second) / dx-(dx*(r[i+1]+2*r[i]) / 3);
    }
}

double SplineInterpolator::operator()(double x) const{
    int k=Locate(x);
    if(k==0) k=1;
    else if(k==DajOGTacke().size()) k=DajOGTacke().size()-1;

    double t=x-DajOGTacke()[k-1].first, y=DajOGTacke()[k-1].second;
    return y+t*(q[k-1]+t*(r[k-1]+s[k-1]*t));
}

class BarycentricInterpolator : public AbstractInterpolator{
    std::vector<double> weights;
public:
    BarycentricInterpolator(const VektorParova &data, int order);
    double operator()(double x) const override;
    std::vector<double> GetWeights() const {return weights;}
};

BarycentricInterpolator::BarycentricInterpolator(const VektorParova &data, int order) : AbstractInterpolator(data), weights(data.size()){
    if(order<0 || order>DajOGTacke().size()) throw std::domain_error("Invalid order");
    int n=DajOGTacke().size();
     
    for(int i=1; i<=n; i++) {
        double p = 0;               //upitno, provjeriti
        for(int k=std::max(1, i-order); k<=std::min(i, n-order); k++){    //interesantna konstrukcija
            p=1;
            for(int j=k; j<=k+order; j++)
            if(j!=i){
                double x1=data[i-1].first, x2=data[j-1].first;
                p/=(x1-x2);
            }
            if(k%2==0)
            p=-p;
        }
        weights[i-1]+=p;
    }
}

double BarycentricInterpolator::operator()(double x) const{
    double p{}, q{};
    for(int i=1; i<=DajOGTacke().size(); i++){         //neki je belaj ovdje
        if(std::fabs(x-DajOGTacke()[i-1].first) < 10 * std::numeric_limits<double>::epsilon()) return DajOGTacke()[i-1].second;
        double u = weights[i-1]/(x - DajOGTacke()[i-1].first);
        p += u*DajOGTacke()[i-1].second;
        q+=u;
    }
    return p/q;
}

class TrigonometricInterpolator : public AbstractInterpolator {         //samo Bog zna sta je sa ovime
public:
    TrigonometricInterpolator(const std::vector<std::pair<double, double>> &data): AbstractInterpolator(data) {
        if(std::fabs(data[0].second - data[data.size()-1].second) >= 1e-10) throw std::domain_error("Function is not periodic");
        else DajOGTacke().pop_back();
    }
    double operator()(double x) const override {
        int n = DajOGTacke().size();
        double mi = 2*3.141592653/(DajOGTacke()[n-1].first - DajOGTacke()[0].first);
        double s = 0;
        if(n % 2 == 0) {
            for(int k = 0; k < n-1; k++) {
                double p = DajOGTacke()[k].second;
                for(int j = 0; j < n-1; j++) {
                    if(j == k) continue;
                    p *= (std::sin(mi/2)*(x-DajOGTacke()[j].first))/(std::sin(mi/2)*(DajOGTacke()[k].first-DajOGTacke()[j].first));
                }
                s += p;
            }
        } else {
            for(int k = 0; k < n-1; k++) {
                double p = DajOGTacke()[k].second;
                double alfa = 0;
                for(int j = 0; j < n-1; j++) {
                    if(j == k) continue;
                    alfa -= DajOGTacke()[j].first;
                    p *= (std::sin(mi/2)*(x-DajOGTacke()[j].first))/(std::sin(mi/2)*(DajOGTacke()[k].first-DajOGTacke()[j].first));
                }
                p *= (std::sin(mi/2)*(x-alfa))/(std::sin(mi/2)*(DajOGTacke()[k].first-alfa));
                s += p;
            }
        }
        return s;
    }
};

void TestAbstractInterpolator() {
    class DummyInterpolator : public AbstractInterpolator {
    public:
        DummyInterpolator(const VektorParova &data) : AbstractInterpolator(data) {}
        double operator()(double x) const override {
            return 0.0; //dummy implemenmtacija ko fol
        }
    };

    VektorParova data = {{0, 0}, {1, 1}};
    DummyInterpolator di(data);

    std::cout << "Test za AbstractInterpolator sa nekim DummyInterpolator...\n";
    for (double x = 0; x <= 1; x += 0.5) {
        double y = di(x);
        std::cout << "di(" << x << ") = " << y << "\n";
    }
    std::cout << "\n";
}

void TestLinearInterpolator() {
    VektorParova data = {{0, 0}, {1, 1}, {2, 4}, {3, 9}};
    LinearInterpolator li(data);

    std::cout << "Test za LinearInterpolator...\n";
    for (double x = 0; x <= 3; x += 0.5) {
        double y = li(x);
        std::cout << "li(" << x << ") = " << y << "\n";
    }
    std::cout << "\n";
}

void TestPolynomialInterpolator() {
    VektorParova data = {{-2, 4}, {-1, 1}, {0, 0}, {1, 1}, {2, 4}};
    PolynomialInterpolator pi(data);

    std::cout << "Test za PolynomialInterpolator...\n";
    for (double x = -2; x <= 2; x += 0.5) {
        double y = pi(x);
        std::cout << "pi(" << x << ") = " << y << "\n";
    }
    std::cout << "\n";
}

void TestSplineInterpolator() {                     //ovo je kao za pi
    VektorParova data = {{0, 0}, {M_PI / 4, std::sin(M_PI / 4)}, {M_PI / 2, std::sin(M_PI / 2)}, {3 * M_PI / 4, std::sin(3 * M_PI / 4)}, {M_PI, std::sin(M_PI)}};
    SplineInterpolator si(data);

    std::cout << "Test za SplineInterpolator...\n";
    for (double x = 0; x <= M_PI; x += 0.1) {
        double y = si(x);
        double actual = std::sin(x);
        std::cout << "si(" << x << ") = " << y << ", prava vrijednost: " << actual << "\n";
    }
    std::cout << "\n";
}

void TestPiecewisePolynomialInterpolator() {
    VektorParova data = {{0, 0}, {1, 1}, {2, 4}, {3, 9}, {4, 16}};
    PiecewisePolynomialInterpolator ppi(data, 2);

    std::cout << "Test za PiecewisePolynomialInterpolator...\n";
    for (double x = 0; x <= 4; x += 0.5) {
        double y = ppi(x);
        std::cout << "ppi(" << x << ") = " << y << "\n";
    }
    std::cout << "\n";
}

void TestBarycentricInterpolator() {
    VektorParova data = {{0, 1}, {M_PI / 2, 0}, {M_PI, -1}, {3 * M_PI / 2, 0}, {2 * M_PI, 1}};
    BarycentricInterpolator bi(data, 2);

    std::cout << "Test za BarycentricInterpolator...\n";
    for (double x = 0; x <= 2 * M_PI; x += 0.5) {
        double y = bi(x);
        std::cout << "bi(" << x << ") = " << y << "\n";
    }
    std::cout << "\n";
}

void TestTrigonometricInterpolator() {
    VektorParova data = {{0, 0}, {M_PI / 2, 1}, {M_PI, 0}, {3 * M_PI / 2, -1}, {2 * M_PI, 0}};
    TrigonometricInterpolator ti(data);

    std::cout << "Test za TrigonometricInterpolator...\n";
    for (double x = 0; x <= 2 * M_PI; x += 0.1) {
        double y = ti(x);
        double actual = std::sin(x);
        std::cout << "ti(" << x << ") = " << y << ", prava vrijednost: " << actual << "\n";
    }
    std::cout << "\n";
}

int main() {
    TestAbstractInterpolator();
    TestLinearInterpolator();
    TestPolynomialInterpolator();
    TestSplineInterpolator();
    TestPiecewisePolynomialInterpolator();
    TestBarycentricInterpolator();
    TestTrigonometricInterpolator();

    return 0;
}