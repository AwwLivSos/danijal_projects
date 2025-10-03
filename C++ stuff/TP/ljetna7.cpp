#include <iostream>
#include <cmath>
#include <string>
#include <utility>
#include <algorithm>

const double PI = 3.14159265358;

class Ugao{
    double radijani;

    void normaliziraj(){
        radijani = std::fabs(radijani);

        while(radijani > 2*PI) 
            radijani -= (2*PI);
    }
public:
    explicit Ugao(double r = 0){
        Postavi(r);
    }
    Ugao(int stepeni, int minute, int sekunde){
        Postavi(stepeni, minute, sekunde);
    }

    void Postavi(double r){
        radijani = r;
        normaliziraj();
    }
    void Postavi(int h, int m, int s){
        int ukupneSekunde = h*3600 + m*60 + s;

        radijani = (ukupneSekunde/3600.0) * PI/180.0;
        normaliziraj();
    }

    double DajRadijane() const {return radijani;}
    int DajStepene() const {
        return radijani * 180.0/PI;
    }
    int DajMinute() const {
        return (radijani * 180.0/PI)*60;
    }
    int DajSekunde() const {
        return (radijani * 180.0/PI)*3600;
    }

    Ugao operator-() const {
        return Ugao(2*PI - radijani);
    }

    friend Ugao operator +(const Ugao &u1, const Ugao &u2){
        return Ugao(u1.DajRadijane() + u2.DajRadijane());
    }
    friend Ugao operator -(const Ugao &u1, const Ugao &u2){
        return Ugao(u1.DajRadijane() - u2.DajRadijane());
    }
    friend Ugao operator *(const Ugao &u1, double faktor){
        return Ugao(u1.DajRadijane() * faktor);
    }
    friend Ugao operator *(double faktor, const Ugao &u1){
        return u1*faktor;
    }

    Ugao &operator +=(const Ugao &u2){
        radijani += u2.DajRadijane();
        normaliziraj();
        return *this;
    }

    Ugao &operator -=(const Ugao &u2){
        radijani -= u2.DajRadijane();
        normaliziraj();
        return *this;
    }

    Ugao &operator *=(double faktor){
        radijani *= faktor;
        normaliziraj();
        return *this;
    }

    Ugao &operator++(){
        radijani += PI/180.0;
        normaliziraj();
        return *this;
    }
    Ugao operator++(int){
        auto temp = *this;
        ++(*this);
        return temp;
    }

    friend bool operator<(const Ugao &u1, const Ugao &u2){
        return u1.DajRadijane() < u2.DajRadijane();
    }

    friend std::ostream &operator<<(std::ostream &tok, const Ugao &u1){
        return tok<<u1.DajStepene()<<"d "<<u1.DajMinute()%60<<"m "<<u1.DajSekunde()%60<<"s";
    }

    friend std::istream &operator>>(std::istream &tok, Ugao &u){
        int step, min, sec;
        char h, m, s;

        if(tok>>step>>h>>min>>m>>sec>>s){
            if(h!='d' || m!='m' || s!='s'){
                tok.setstate(std::ios::failbit);
            }
            else u.Postavi(step, min, sec);
        }

        return tok;
    }

};

class Roba{
    double tezina;
    std::string naziv;
    double tezinaUnutrasnjosti;
public:
    Roba(double w, std::string s, double wu) : tezina(w), naziv(s), tezinaUnutrasnjosti(wu) {}
    virtual ~Roba() = default;
    double DajTezinu() const {return tezina;}
    std::string DajNaziv() const {return naziv;}
    double DajTezinuSadrzaja() const {return tezinaUnutrasnjosti;}
    double DajUkupnuTezinu() const {return tezina+tezinaUnutrasnjosti;}
    virtual void Ispisi() const = 0;
    virtual Roba* DajKopiju() const = 0;
};

class Sanduk : public Roba{
    int brojPredmeta;
public:
    Sanduk(double w, std::string s, int n, double wu) : Roba(w, s, wu), brojPredmeta(n) {}
    void Ispisi() const override {std::cout<<"Sanduk sa "<<brojPredmeta<<" predmeta, tezine "<<DajTezinu()<<"kg.";}
    Roba* DajKopiju() const override {return new Sanduk(*this);}
};

class Bure : public Roba{
public:
    Bure(double w, std::string s, double wu) : Roba(w, s, wu) {}
    void Ispisi() const override {std::cout<<"Bure od "<<DajTezinuSadrzaja()<<"l, tezine "<<DajTezinu()<<"kg.";}
    Roba* DajKopiju() const override {return new Bure(*this);}
};

class Skladiste{
    Roba** elementi;
    int kapacitet;
    int velicina;
public:
    explicit Skladiste(int max) : kapacitet(max), velicina(0), elementi(new Roba*[max]{}) {}
    ~Skladiste(){
        for(int i=0; i<velicina; i++)
            delete elementi[i];
        delete[] elementi;
    }
    Skladiste(const Skladiste &s) : kapacitet(s.kapacitet), velicina(s.velicina), elementi(new Roba*[s.kapacitet]{}){
        for(int i=0; i<s.velicina; i++){
            elementi[i] = (*s.elementi[i]).DajKopiju();
        }
    }
    Skladiste(Skladiste &&s) : kapacitet(s.kapacitet), velicina(s.velicina), elementi(s.elementi){
        s.velicina=0;
        s.elementi=nullptr;
    }

    Skladiste &operator=(Skladiste s){
        std::swap(kapacitet, s.kapacitet);
        std::swap(velicina, s.velicina);
        std::swap(elementi, s.elementi);

        return *this;
    }

    int DajBrojObjekata() const {return velicina;}
    void DodajSanduk(double w, std::string s, int n, double wu){
        if(velicina == kapacitet) throw std::domain_error("Kolekcija popunjena!");

        elementi[velicina++] = new Sanduk(w, s, n, wu);
    }
    void DodajBure(double w, std::string s, double wu){
        if(velicina == kapacitet) throw std::domain_error("Kolekcija popunjena!");

        elementi[velicina++] = new Bure(w, s, wu);
    }

    Roba& DajNajlaksi() const {
        if(velicina==0) throw std::domain_error("Prazna kolekcija!");

        return **std::min_element(elementi, elementi+velicina, [](Roba *r1, Roba *r2){
            return r1->DajTezinu() < r2->DajTezinu();
        });
    }
    Roba& DajNajtezi() const {
        if(velicina==0) throw std::domain_error("Prazna kolekcija!");

        return **std::max_element(elementi, elementi+velicina, [](Roba *r1, Roba *r2){
            return r1->DajTezinu() < r2->DajTezinu();
        });
    }

    int DajBrojTezih(int granica){
        return std::count_if(elementi, elementi+velicina, [granica](Roba *r){
            return r->DajUkupnuTezinu() > granica; 
        });
    }

    void Ispisi() const{
        Roba** kopija = new Roba*[kapacitet]{};
        for(int i=0; i<velicina; i++)
            kopija[i] = elementi[i]->DajKopiju();

        std::sort(kopija, kopija+velicina, [](Roba *r1, Roba* r2){
            return r1->DajTezinu() < r2->DajTezinu();
        });

        for(int i = 0; i<velicina; i++){
            kopija[i]->Ispisi();
            std::cout<<std::endl;
        }

        for(int i=0; i<velicina; i++)
            delete kopija[i];
        delete[] kopija;
    }

    Roba &operator[](int indeks){
        if(indeks<0 || indeks>velicina) throw std::domain_error("Neispravan indeks!");

        return *elementi[indeks];
    }

    const Roba &operator[](int indeks) const{
        if(indeks<0 || indeks>velicina) throw std::domain_error("Neispravan indeks!");

        return *elementi[indeks];
    }
};

int main(){
    

    return 0;
}