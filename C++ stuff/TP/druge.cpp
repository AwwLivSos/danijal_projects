#include <vector>
#include <stdexcept>
#include <iostream>
#include <string>
#include <algorithm>
#include <initializer_list>

class Krug {
    double poluprecnik;
    static const double PI;
public:
    explicit Krug(double poluprecnik) { Postavi(poluprecnik); }
    Krug() = default;

    Krug &Postavi(double poluprecnik) {
        if(poluprecnik <= 0) throw std::domain_error("Neispravan poluprecnik");
        Krug::poluprecnik = poluprecnik;
        return *this;
    }

    double DajPoluprecnik() const { return poluprecnik; }
    double DajObim() const { return 2 * PI * DajPoluprecnik(); }
    double DajPovrsinu() const { return PI * DajPoluprecnik() * DajPoluprecnik(); }
    Krug &Skaliraj(double faktor) {
        if(faktor <= 0) throw std::domain_error("Neispravan faktor skaliranja");
        return Postavi(DajPoluprecnik() * faktor);
    }
    void Ispisi() const {
        std::cout << "R=" << DajPoluprecnik() << " O=" << DajObim()
        << " P=" << DajPovrsinu();
    }
};

const double Krug::PI = 4 * std::atan(1);

/*class Valjak {
    Krug baza;
    double visina;
    void PostaviVisinu(double visina) {
        if(visina <= 0) throw std::domain_error("Neispravna visina");
        Valjak::visina = visina;
    }
public:
    Valjak(const Krug &baza, double visina) : baza(baza) { PostaviVisinu(visina); }
    Valjak(double poluprecnik_baze, double visina) : baza(poluprecnik_baze) {
        PostaviVisinu(visina);
    }
    Valjak &Postavi(const Krug &baza, double visina) {
        Valjak::baza = baza; 
        PostaviVisinu(visina);
        return *this;
    }
    Valjak &Postavi(double poluprecnik_baze, double visina) {
        baza.Postavi(poluprecnik_baze); 
        PostaviVisinu(visina);
        return *this;
    }
    Krug &DajBazu() { return baza; }
    const Krug &DajBazu() const { return baza; }
    double DajPoluprecnikBaze() const { return baza.DajPoluprecnik(); }
    double DajVisinu() const { return visina; }
    double DajPovrsinu() const {
        return 2 * baza.DajPovrsinu() + baza.DajObim() * DajVisinu();
    }
    double DajZapreminu() const { return baza.DajPovrsinu() * DajVisinu(); }
    Valjak &Skaliraj(double faktor) {
        baza.Skaliraj(faktor); PostaviVisinu(faktor * DajVisinu());
        return *this;
    }
    void Ispisi() const {
        std::cout << "R=" << DajPoluprecnikBaze() << " H=" << DajVisinu()
        << " P=" << DajPovrsinu() << " V=" << DajZapreminu();
    }
}; */

class Krugovi {
    int broj_krugova, kapacitet;
    Krug **krugovi;
    void Dealociraj();
public:
    Krugovi(int kapacitet) : broj_krugova(0), kapacitet(kapacitet), krugovi(new Krug*[kapacitet]{}) {}
    Krugovi(std::initializer_list<Krug> lista);
    ~Krugovi() { Dealociraj(); }

    Krugovi(const Krugovi &k);
    Krugovi(Krugovi &&k);

    Krugovi &operator =(Krugovi k);

    void DodajKrug(double x, double y, double r);

    Krug operator[] (int i){
        return *krugovi[i];
    }

    Krug operator() (int i){
        if(i<0 || i>broj_krugova) throw std::domain_error("Neispravan indeks!");
        return *krugovi[i];
    }

    const Krug &DajKrug(int i) const;
    void IspisiKrugove() const;
};

void Krugovi::Dealociraj() {
    for(int i = 0; i < broj_krugova; i++) 
        delete krugovi[i];
    delete[] krugovi;
}


Krugovi::Krugovi(const Krugovi &k) : broj_krugova(k.broj_krugova), kapacitet(k.kapacitet), krugovi(new Krug*[k.kapacitet]{}) {
    try{
        for(int i=0; i<k.broj_krugova; i++){
            krugovi[i] = new Krug(*k.krugovi[i]);
        }
    }
    catch(...){
        Dealociraj();
        throw;
    }
}

Krugovi::Krugovi(std::initializer_list<Krug> lista) : broj_krugova(0), kapacitet(lista.size()), krugovi(new Krug*[lista.size()]{}){
    int i=0;
    for(auto x : lista){
        DodajKrug(x.x, x.y, x.r);
    }
}

Krugovi::Krugovi(Krugovi &&k) : broj_krugova(k.broj_krugova), kapacitet(k.kapacitet), krugovi(k.krugovi) {
    k.krugovi = nullptr;
    k.broj_krugova=0;
}

Krugovi &Krugovi::operator =(Krugovi k) {
    std::swap(broj_krugova, k.broj_krugova); 
    std::swap(kapacitet, k.kapacitet);
    std::swap(krugovi, k.krugovi);
    return *this;
}

void Krugovi::IspisiKrugove() const {
    Krug **krugovi_kopija = new Krug*[broj_krugova];
    std::copy(krugovi, krugovi + broj_krugova, krugovi_kopija);
    std::sort(krugovi_kopija, krugovi_kopija + broj_krugova, [](const Krug &k1, const Krug &k2) { return k.r > k2.r; });
    for(int i = 0; i < broj_krugova; i++)
        std::cout << "[(" << krugovi_kopija[i].x << "," << krugovi_kopija[i].y << "),"
        << krugovi_kopija[i].r << "]" << std::endl;

    delete[] krugovi_kopija;
}

template <typename Tip>
    class Skup {
        std::vector<Tip> elementi;
    public:
        Skup() {}

        Skup(std::initializer_list<Tip> lista){
            for(auto element : lista){
                elementi.push_back(element);
            }
        }

        int Velicina() const { return elementi.size(); }
        void Dodaj(const Tip &element);
        void Izbrisi(const Tip &element);
        bool ImaLiGa(const Tip &element) const;
        void Ispisi() const;
    };

/*Razvijte klasu “Ugao” (ili “Kut”, u skladu sa vašim jezičkim preferencijama) koja omogućava rad sa
uglovima (kutovima) u ravni. Jedini atribut ove klase je realni broj, koji predstavlja vrijednost ugla u
radijanima. Klasa podržava dva konstruktora. Jedan prima realni broj kao parametar i omogućava
zadavanje ugla u radijanima, dok drugi prima tri cijela broja kao parametra i omogućava zadavanje
ugla u stepenima, minutama i sekundama. Prvi konstruktor ima podrazumijevani parametar 0, što
omogućava da se prazan ugao zada bez ikakvih parametara. Također, u slučaju se zada ugao veći od
2π ili manji od 0, treba ga svesti na opseg od 0 do 2π. Ovaj konstruktor treba da podrži automatsku
pretvorbu realnih brojeva u objekte tipa “Ugao”. Drugi konstruktor dopušta da vrijednosti parametara
kojim se zadaju stepeni, minute i sekunde mogu biti i izvan tipičnog opsega, ali tada automatski treba
preračunavati preljev (npr. 130 sekundi treba automatski tretirati kao 2 minute i 10 sekundi, 750
stepeni treba tretirati kao 30 stepeni jer 360 stepeni predstavlja pun krug, itd.). Stoga, na primjer,
ugao zadan kao 130 stepeni, 90 minuta i 150 sekundi treba tretirati kao 131 stepen, 32 minuta i 30
sekundi. S druge strane, ukoliko je makar jedan od parametara negativan, konstruktor treba da baci
izuzetak. Dalje je potrebno podržati dvije verzije metode “Postavi” koje rade istu stvar kao i
konstruktor, a omogućavaju naknadno postavljanje ugla. Informacija o vrijednosti ugla u radijanima
dobija se pozivom metode “DajRadijane”, dok metode “DajStepene”, “DajMinute” i “DajSekunde” daju
vrijednost ugla u stepenima, minutama i sekundama. Pored ovih metoda, klasa podržava i izvjesne
operatore. Unarni operator “-” primijenjen na neki ugao daje njegovu dopunu do punog kruga, tj. ugla
od 2π radijana. Binarni operatori “+” i “-” daju kao rezultat zbir odnosno razliku dva ugla. Binarni
operator “*” množi ugao sa realnim brojem. Treba podržati kako množenje ugla sa brojem, tako i
množenje broja sa uglom (kod svih ovih operacija, dobijeni rezultat uvijek mora biti sveden na opseg
od 0 do 2π radijana). Binarni operatori “+=”,“-=” i “*=” trebaju osigurati da izrazi “X += Y”, “X -= Y” i
“X *= Y” uvijek imaju isto značenje kao i izrazi “X = X + Y”, “X = X - Y” i “X = X * Y” kadgod ovi izrazi
imaju smisla. Unarni operator “++” povećava ugao na koji je primijenjen za jedan ugaoni stepen, pri
čemu je potrebno podržati i prefiksnu i postfiksnu verziju ovog operatora. Operator “<” testira da li je
prvi ugao manji od drugog ili ne i daje odgovarajuću logičku vrijednost. Konačno, operator “<<”
podržava ispis ugla na ekran u obliku <stepeni> d <minute> m <sekunde> s, na primjer “23d 15m 42s”,
dok operator “>>” za čitanje ugla sa tastature, u istom obliku u kojem se i vrši ispis (u slučaju
neispravnog unosa, objekat toka treba biti postavljen u neispravno stanje). Sve metode klase treba
implementirati izvan deklaracije klase, osim trivijalnih metoda. Sve metode koje su inspektori
obavezno treba deklarirati kao takve.*/

const double PI = 3.14159265358;

class Ugao{
    double radijani;
public:
    Ugao(double radijani=0) : radijani(radijani) {}
    Ugao(int stepeni, int minute, int sekunde){
        Postavi(stepeni, minute, sekunde);
    }

    Ugao& Postavi(double radijani){
        Ugao::radijani = radijani;
        return *this;
    }
    Ugao& Postavi(int stepeni, int minute, int sekunde){
        if(stepeni<0 || minute<0 || sekunde<0) throw std::domain_error("Negativni stepeni!");

        int ukupneSekunde = stepeni*3600 + minute*60 + sekunde;

        radijani = (ukupneSekunde/3600.0) * (PI/180.0);

        return *this;
    }

    friend Ugao operator-(const Ugao &u1);
    friend Ugao operator-(const Ugao &u1, const Ugao &u2);
    friend Ugao operator+(const Ugao &u1, const Ugao &u2);
    friend Ugao operator*(const Ugao &u1, double faktor);
    friend Ugao operator*(double faktor, const Ugao &u1);
    friend std::ostream& operator<<(std::ostream &tok, const Ugao &u1);
    friend std::istream& operator>>(std::istream &tok, Ugao &u1);

    friend bool operator<(const Ugao &u1, const Ugao &u2);

    Ugao operator-(){
        Ugao rez(2*PI-radijani);
        return rez;
    }

    Ugao operator-(const Ugao &u1){
        return Ugao(radijani - u1.radijani);
    }

    Ugao& operator+=(const Ugao &u1){
        radijani += u1.radijani;
        return *this;
    }

    Ugao& operator-=(const Ugao &u1){
        *this = *this-u1;
        return *this;
    }

    Ugao& operator*=(double faktor){
        *this = *this*faktor;
        return *this;
    }

    Ugao& operator++(){
        radijani+=(PI/180.0);
        return *this;
    }

    Ugao& operator++(int){
        auto novi = *this;
        ++(*this);
        return novi;
    }


};

std::ostream& operator<<(std::ostream &tok, const Ugao &u1){
    return tok<<"Radijani: "<<u1.radijani;
}

std::istream& operator>>(std::istream &tok, Ugao &u1){
    double rad;
    tok>>rad;
    if(!tok){
        return tok;
    }
    if(rad < 0){
        tok.setstate(std::ios::failbit);
        return tok;
    }
    u1.radijani = rad;
    return tok;
}

Ugao operator*(double faktor, const Ugao &u1){
    return u1*faktor;
}

Ugao operator*(const Ugao &u1, double faktor){
    return Ugao(u1.radijani*faktor);
}

bool operator<(const Ugao &u1, const Ugao &u2){
    return u1.radijani<u2.radijani;
}

Ugao operator-(const Ugao &u1){
    Ugao rez(2*PI-u1.radijani);
    return rez;
}

Ugao operator-(const Ugao &u1, const Ugao &u2){
    return u1+(-u2);
}

Ugao operator+(const Ugao &u1, const Ugao &u2){
    Ugao rez=u1;
    return rez+=u2;
}

int main(){
    Krugovi k1(5);

    std::vector<int> v(2);

    k1[5];

    Ugao u;

    operator*(4.5, u);

    std::vector<int> w{1,2,3};
    w = v;

    k1.Postavi(6).Skaliraj(5).Ispisi();

    return 0;
}