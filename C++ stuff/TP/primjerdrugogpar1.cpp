#include <iostream>
#include <list>
#include <complex>
#include <stack>
#include <set>
#include <initializer_list>
#include <algorithm>
#include <vector>
#include <deque>

//prvi zadatak
struct Kompleksni {
    double re, im;
    Kompleksni *sljedeci;
};

Kompleksni* kreirajListu(const std::list<std::complex<double>> &lista){
    if(lista.empty()) return 0;

    Kompleksni *prvi{}, *trenutni{};

    for(const auto &c : lista){

        Kompleksni* novi = new Kompleksni{c.real(), c.imag(), nullptr};

        if(!prvi){
            prvi=novi;
            trenutni=novi;
        }
        else{
            trenutni->sljedeci = novi;
            trenutni = trenutni->sljedeci;
        }
    }

    return prvi;
}

//drugi zadatak
struct Kalamuta {
    virtual int foo() const { return 5;}
    long int bar () { return 4;}
    virtual int baz() const { return 3;}
    virtual ~Kalamuta() {}
};

struct Cincozna : public Kalamuta {
    int foo() const { return 6;}
    long bar () { return 8;}
    virtual int baz() { return 12;}
};

//treci zadatak
class KlasaA {
    int x, y;
public:
    KlasaA (int xx=2, int yy = 1) : x(xx), y(yy) {}
    virtual int DajX() const { return x; }
    virtual int DajY() const { return y; }
    virtual ~KlasaA() = default;
};

class KlasaB : public KlasaA {
    int z;
public:
    KlasaB (int x=1, int y=2, int z=0) : KlasaA(x, y) {this->z = z;}
    int DajX() const override { return KlasaA::DajX() + z; }
    int DajY() const override { return KlasaA::DajY() + z; }
    int DajZ() { return z; }
};

//peti zadatak
std::ostream &operator<<(std::ostream &tok, const std::stack<int> &s){
    auto drugi = s;
    while(!drugi.empty()){
        tok<<drugi.top()<<" ";
        drugi.pop();
    }
    return tok;
}

std::stack<int> operator-(std::stack<int> s1, std::stack<int> s2){
    if(s1.size() != s2.size()) throw std::domain_error("Stekovi nesaglasnih velcina!");

    std::stack<int> novi;
    while(!s1.empty()){ 
        novi.push(s1.top() - s2.top());
        s1.pop(); s2.pop();
    }
    return novi;
}

std::stack<int> &operator*=(std::stack<int> &s, int n){
    std::stack<int> pom;
    while(!s.empty()){
        pom.push(s.top() * n);
        s.pop();
    }

    while(!pom.empty()){
        s.push(pom.top());
        pom.pop();
    }

    return s;
}

bool operator*(const std::stack<int> &s){
    return !s.empty();
}

//cetvrti zadatak
template<typename Tip>
class Skup{
    std::vector<Tip> v;
public:

    Skup() = default;

    Skup(std::initializer_list<Tip> lista){
        for(const auto &x : lista) Dodaj(x);
    }

    int Velicina() const {return v.size();}

    void Dodaj(const Tip &n){
        if(!ImaLiGa(n)){
            v.push_back(n);
            std::sort(v.begin(), v.end());
        }
    }

    void Izbrisi(const Tip &n){
        auto it = std::find(v.begin(), v.end(), n);
        if(it != v.end() && *it == n) v.erase(it);
    }

    bool ImaLiGa(const Tip &n) const {
        return std::binary_search(v.begin(), v.end(), n);
    }

    void Ispisi() const {
        for(const auto &x : v) std::cout<<x<<" ";
    }
};

class Zivotinja{
public:
    virtual ~Zivotinja() = default;
    virtual void OglasiSe() const = 0;
    virtual Zivotinja *Kopiraj() const = 0;
};

class Pas : public Zivotinja{
public:
    void OglasiSe() const override {std::cout<<"Av!";}
    Zivotinja *Kopiraj() const override {return new Pas(*this);}
};

class Macka : public Zivotinja{
public:
    void OglasiSe() const override {std::cout<<"Mjau!";}
    Zivotinja *Kopiraj() const override {return new Macka(*this);}
};

class Krava : public Zivotinja{
public:
    void OglasiSe() const override {std::cout<<"Muu!";}
    Zivotinja *Kopiraj() const override {return new Krava(*this);}
};

class PolimorfnaZivotinja{
    Zivotinja *z;
public:
    PolimorfnaZivotinja() : z(nullptr) {}

    PolimorfnaZivotinja(const PolimorfnaZivotinja &druga) : z(druga.z!=nullptr ? druga.z->Kopiraj() : nullptr) {}

    PolimorfnaZivotinja(PolimorfnaZivotinja &&druga) : z(druga.z) {druga.z=nullptr;}

    ~PolimorfnaZivotinja() {delete z;}

    PolimorfnaZivotinja& operator =(const PolimorfnaZivotinja &druga){
        Zivotinja *nova = (druga.z ? druga.z->Kopiraj() : nullptr);
        delete z;
        z= nova;
        return *this;
    }

    PolimorfnaZivotinja &operator=(PolimorfnaZivotinja &&druga){
        std::swap(druga.z, z);
        return *this;
    }

    PolimorfnaZivotinja(const Zivotinja &druga) : z(druga.Kopiraj()) {}

    void OglasiSe() {
        if(!z) throw std::logic_error("Nepoznata vrsta zivotinje!");
        z->OglasiSe();
    }
};

//primjer 3, drugi zadatak
template<typename T>
class RedSPrioritetom{
    std::deque<T> d;
public:
    void Dodaj(const T &n) {d.push_back(n);}
    T DajNajveci() const {
        if(!d.empty()) return *std::max_element(d.begin(), d.end());
    }
    void UkloniNajveci(){
        if(!d.empty()){
            auto it = std::max_element(d.begin(), d.end());
            d.erase(it);
        }
    }
    int Velicina() const {return d.size();}
    bool Prazan() const {return d.empty();}
};

//primjer 2, prvi zadatak
class Ugao{
    double radijani;
    static constexpr double PI = 3.1415923653;
    void Normaliziraj(){
        while(radijani > 2*PI) radijani-=(2*PI);
        while(radijani < 0) radijani+=(2*PI);
    }
public:
    Ugao(double r = 0) : radijani(r){
        Postavi(r);
    }

    Ugao(int stepeni, int minute, int sekunde){
        if(stepeni<0 || minute<0 || sekunde<0) throw std::domain_error("Negativni stepeni!");

        int ukupno = stepeni*3600 + minute*60 + sekunde;
        ukupno%= (360*360);
        ukupno/=3600.0;

        radijani = ukupno * (PI/180);
    }

    void Postavi(double r){
        radijani=r;
        Normaliziraj();
    }

    void Postavi(int stepeni, int minute, int sekunde){
        *this = Ugao(stepeni, minute, sekunde);
    }

    double DajRadijane() const {return radijani;}
    int DajStepene() const {return radijani * (180/PI);}
    int DajMinute() const {return DajStepene()*60;}
    int DajSekunde() const {return DajMinute()*60;}

    double operator -() const {return 2*PI - radijani;}
    //Ugao operator -() const {return Ugao(2*PI - radijani);}
    Ugao &operator+=(const Ugao& u){
        radijani+=u.radijani;
        Normaliziraj();
        return *this;
    }
    Ugao &operator-=(const Ugao &u) {(*this)+=(-u.radijani); return *this;}
    Ugao &operator*=(double x) {radijani*=x; Normaliziraj(); return *this;}

    friend Ugao operator+(const Ugao &u1, const Ugao &u2){
        auto rez = u1;
        rez+=u2;
        return rez;
    }
    friend Ugao operator-(const Ugao &u1, const Ugao &u2) {
        auto rez = u1;
        rez-=u2;
        return rez;
    }
    friend Ugao operator*(const Ugao &u, double x) {
        auto rez = u;
        rez*=x;
        return rez;
    }
    friend Ugao operator*(double x, const Ugao &u) {return u*x;}

    Ugao& operator++(){
        radijani += (PI/180);
        Normaliziraj();
        return *this;
    }

    Ugao& operator++(int){
        auto drugi = *this;
        ++(*this);
        return drugi;
    }

    friend bool operator<(const Ugao &u1, const Ugao &u2) {return u1.radijani < u2.radijani;}

    friend std::ostream &operator<<(std::ostream &tok, const Ugao &u){
        return tok<<u.DajStepene()<<"d "<<u.DajMinute()<<"m "<<u.DajSekunde()<<"s";
    }

    friend std::istream &operator>>(std::istream &tok, Ugao &u){
        int stepeni, minute, sekunde;
        char d, m, s;

        if(tok>>stepeni>>d>>std::ws>> minute>>m>>std::ws>> sekunde>>s){
            if(d=='d' && m=='m' && s=='s') u.Postavi(stepeni, minute, sekunde);
            else tok.setstate(std::ios::failbit);
        }
        return tok;
    }
};

int main() {

    return 0;
}