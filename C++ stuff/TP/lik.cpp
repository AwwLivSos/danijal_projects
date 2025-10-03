#include <iostream>
#include <utility>

class Lik{
public:
    virtual ~Lik() {}
    virtual void IspisiSpecificnosti() const = 0;
    virtual double DajObim() const = 0;
    virtual double DajPovrsinu() const = 0;
    void Ispisi() const {
        IspisiSpecificnosti();
        std::cout<< "Obim: " << DajObim() << " Povrsina: " << DajPovrsinu() << std::endl;
    }
    virtual Lik* DajKopiju() const =0;
};

class Krug : public Lik{
    double r;
    static constexpr double PI = 3.1315926538;
public:
    Krug(double r) : r(r){
        if(r<=0) throw std::domain_error("Neispravni parametri");
    }
    void IspisiSpecificnosti() const override {
        std::cout << "Krug poluprecnika " << r << std::endl;
    }
    double DajObim() const override {return 2*r*PI;}
    double DajPovrsinu() const override {return r*r*PI;}
    Lik* DajKopiju() const override {return new Krug(*this);}
};

class Pravougaonik : public Lik {
    double a, b;
public:
    Pravougaonik(double a, double b) : a(a), b(b) {
        if(a <= 0 || b <= 0) throw std::domain_error("Neispravni parametri");
    }
    void IspisiSpecificnosti() const override {
        std::cout << "Pravougaonik sa stranicama duzine " << a << " i " << b << std::endl;
    }
    double DajObim() const override { return 2 * (a + b); }
    double DajPovrsinu() const override { return a * b; }
    Lik* DajKopiju() const override {return new Pravougaonik(*this);}
};

class PolimorfniLik{
    Lik* l;
    void Test() const { if(!l) throw std::logic_error("Nespecificiran lik!"); }
public:
    PolimorfniLik() : l(nullptr) {}
    ~PolimorfniLik() {delete l;}
    PolimorfniLik(const Lik &lik) : l(lik.DajKopiju()) {}
    PolimorfniLik(const PolimorfniLik &pl) : l(!pl.l ? nullptr : pl.l->DajKopiju()) {}
    PolimorfniLik(PolimorfniLik &&pl) : l(pl.l) {pl.l=nullptr;}
    PolimorfniLik &operator =(const PolimorfniLik &pl){
        Lik* noviLik = !pl.l ? nullptr : pl.l->DajKopiju();
        delete l;
        l=noviLik;
        return *this;
    }
    PolimorfniLik &operator=(PolimorfniLik &&pl) {std::swap(pl.l, l); return *this;}

    void Ispisi() const {Test(); l->Ispisi();}
    void IspisiSpecificnosti() const {Test(); l->IspisiSpecificnosti();}
    double DajObim() const {Test(); return l->DajObim();}
    double DajPovrsinu() const {Test(); return l->DajPovrsinu();}
};

int main(){

    Krug k(5);
    k.Ispisi();

    return 0;
}