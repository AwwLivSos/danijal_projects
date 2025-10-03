#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <utility>
#include <cmath>
#include <string> 
#include <fstream>

class GrafickiElement {
public:
    enum Boje {Crvena, Zelena, Zuta, Plava, Ljubicasta, Crna, Bijela};

    virtual ~GrafickiElement() {}

    virtual double DajPovrsinu() const = 0;

    virtual GrafickiElement *DajKopiju() const = 0;

    GrafickiElement(Boje boja) : boja(boja) {}
    Boje DajBoju() const { return boja; }
    virtual void Ispisi() const { std::cout << boja; };
private:
    Boje boja;
};

class Tacka : public GrafickiElement {
    double x, y;
public:
    Tacka(GrafickiElement::Boje boja, double x, double y) : GrafickiElement(boja), x(x), y(y) {}

    void Ispisi() const override { std::cout << DajBoju() << " {" << x << "," << y << "}"; }

    Tacka &operator*=(double k){
        x*=k; y*=k;
        return *this;
    }

    double DajPovrsinu() const override {return 0;}
    GrafickiElement* DajKopiju() const override {return new Tacka(*this);}

    friend Tacka operator*(const Tacka &t, double k) {
        auto nova = t;
        return nova*=k;
    }

    friend Tacka operator*(double k, const Tacka &t) {return t*k;}

    double DajX() const {return x;}
    double DajY() const {return y;}
};

class Poligon : public GrafickiElement {
    int broj_tjemena;
    double *x, *y;
public:
    Poligon(GrafickiElement::Boje boja, int broj_tjemena) : GrafickiElement(boja), broj_tjemena(broj_tjemena){
            try{
                x = new double[broj_tjemena];

                try{
                    y = new double[broj_tjemena];
                }
                catch(...){
                    delete[] x;
                    x=nullptr;
                    y=nullptr;
                    throw;
                }
            }
            catch(...){
                x = nullptr;
                throw;
            }
        }

    Poligon(const Poligon &p) : GrafickiElement(p.DajBoju()), broj_tjemena(p.broj_tjemena){

            try{
                x = new double[broj_tjemena];

                try{
                    y = new double[broj_tjemena];
                }
                catch(...){
                    delete[] x;
                    x=nullptr;
                    y=nullptr;
                    throw;
                }
            }
            catch(...){
                x = nullptr;
                throw;
            }

            std::copy(p.x, p.x + p.broj_tjemena, x);
            std::copy(p.y, p.y + p.broj_tjemena, y);
        }

    Poligon(Poligon &&p) : GrafickiElement(p.DajBoju()), broj_tjemena(p.broj_tjemena), x(p.x), y(p.y){
        p.x = nullptr;
        p.y = nullptr;
        p.broj_tjemena = 0;
    }

    ~Poligon(){
        delete[] x;
        delete[] y;
    }

    Poligon &operator=(Poligon p){
        std::swap(p.x, x);
        std::swap(p.y, y);
        std::swap(p.broj_tjemena, broj_tjemena);
        return *this;
    }

    double DajPovrsinu() const override {
        double suma{};
        for(int i=0; i<broj_tjemena-1; i++)
            suma += (x[i] * y[i+1] -x[i+1] * y[i]);
        suma += (x[broj_tjemena-1] * y[0] - x[0] * y[broj_tjemena-1]);

        return std::fabs(suma)/2;
    }

    GrafickiElement* DajKopiju() const override {return new Poligon(*this);}

    void PostaviTjeme(int redni_broj, double x, double y) {
        if(redni_broj < 0 || redni_broj > broj_tjemena) 
            throw std::range_error("Ilegalan indeks");
        Poligon::x[redni_broj - 1] = x; this->y[redni_broj - 1] = y;
    }

    void PostaviTjeme(int redni_broj, const Tacka &t){
        PostaviTjeme(redni_broj, t.DajX(), t.DajY());
    }

    const Tacka operator[](int indeks) const{
        if(indeks < 0 || indeks > broj_tjemena) throw std::range_error("Ilegalan indeks");

        return Tacka(DajBoju(), x[indeks-1], y[indeks-1]);
    }

    Tacka operator[](int indeks) {
        if(indeks < 0 || indeks > broj_tjemena) throw std::range_error("Ilegalan indeks");

        return Tacka(DajBoju(), x[indeks-1], y[indeks-1]);
    }

    void Ispisi() const override {
        std::cout << DajBoju() << " {";
        for(int i = 0 ; i < broj_tjemena; i++) {
            std::cout << "{" << x[i] << "," << y[i] << "}";
            if(i != broj_tjemena - 1) std::cout << ",";
        }
        std::cout << "}";
    }
};

class Krug : public GrafickiElement{
    Tacka centar;
    double poluprecnik;
public:
    Krug(GrafickiElement::Boje boja, double x, double y, double r) : GrafickiElement(boja), centar(boja, x, y), poluprecnik(r) {}
    Krug(GrafickiElement::Boje boja, Tacka t, double r) : GrafickiElement(boja), centar(t), poluprecnik(r) {}

    double DajPoluprecnik() const {return poluprecnik;}
    Tacka DajCentar() const {return centar;}

    double DajPovrsinu() const override {
        return poluprecnik * poluprecnik * 3.1415926538;
    }

    GrafickiElement* DajKopiju() const override {return new Krug(*this);}

    void Ispisi() const { std::cout<<"{{"<<centar.DajX()<<","<<centar.DajY()<<"}, "<<poluprecnik;}
};

class GrafickiSistem {
    std::vector<std::shared_ptr<GrafickiElement>> elementi;
public:
    GrafickiSistem() {}

    GrafickiSistem(const GrafickiSistem &gs) {
        for(int i=0; i<gs.elementi.size(); i++)
            elementi.push_back(std::shared_ptr<GrafickiElement>(gs.elementi[i]->DajKopiju()));
    }

    GrafickiSistem(GrafickiSistem &&gs) : elementi(gs.elementi) {
        gs.elementi.clear();
    }

    GrafickiSistem &operator=(GrafickiSistem gs){
        std::swap(elementi, gs.elementi);
        return *this;
    }

    void RegistrirajElement(GrafickiElement *e) {
        elementi.push_back(std::shared_ptr<GrafickiElement>(e->DajKopiju()));
    }

    void RegistrirajElement(GrafickiElement &e){
        elementi.push_back(std::shared_ptr<GrafickiElement>(e.DajKopiju()));
    }

    void IspisiSve() const {
        for(int i = 0; i < elementi.size(); i++) {
            elementi[i]->Ispisi();
            std::cout << std::endl;
        }
    }

    void UcitajIzDatoteke(std::string imeDatoteke){
        elementi.clear();

        std::ifstream ulaz(imeDatoteke);
        if(!ulaz) throw std::domain_error("Datoteka ne postoji!");

        int brojElemenata;
        ulaz >> brojElemenata>>std::ws;

        for(int i=0; i<brojElemenata; i++){
            char znak;
            ulaz>>znak;

            int boja;
            ulaz>>boja;
            auto b = static_cast<GrafickiElement::Boje>(boja);

            if(znak=='T'){
                double x, y;
                ulaz>>x>>y;
                RegistrirajElement(new Tacka(b, x, y));
            }
            else if(znak == 'P'){
                int brojTjemena;
                ulaz>>brojTjemena;

                Poligon* p = new Poligon(b, brojTjemena);

                for(int j=0; j<brojTjemena; j++){
                    double x, y;
                    ulaz>>x>>y;
                    p->PostaviTjeme(j, x, y);
                }

                RegistrirajElement(p);
            }
            else if(znak == 'K'){
                double x, y, r;
                ulaz>>x>>y>>r;
                RegistrirajElement(new Krug(b, x, y, r));
            }
        }
    }
};

int main() {
    GrafickiSistem s;

    s.UcitajIzDatoteke("ELEMENTI.TXT");
    s.IspisiSve();
}