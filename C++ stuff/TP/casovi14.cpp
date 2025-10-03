#include <iostream>
#include <cmath>
#include <ostream>
#include <algorithm>
#include <cstring>

class Sat{
    int sati, minute, sekunde;
public:
    Sat() : sati(0), minute(0), sekunde(0) {}
    Sat(int s, int m, int h) {PostaviNormalizirano(h, m, s);}
    Sat& PostaviNormalizirano(int h, int m, int s);
    Sat& Postavi(int h, int m, int s){
        if(!DaLiJeIspravno(h, m, s)) throw std::domain_error("Neispravno vrijeme");
        Sat::sati = sati; Sat::minute = minute; Sat::sekunde = sekunde;
        return *this;
    }
    bool DaLiJeIspravno(int sati, int minute, int sekunde) {
        return sati >= 0 && sati <= 23 && minute >= 0
        && minute <= 59 && sekunde >=0 && sekunde <= 59;
    }

    Sat operator +(int pomak) {return *this+=pomak;}

    Sat& operator +=(int pomak) {return PostaviNormalizirano(sati, minute, sekunde+pomak);}

    Sat& operator -=(int pomak) {return *this+=(-pomak);}
    Sat operator -(int pomak) {return *this-=pomak;}

    Sat& operator ++() {return *this+=1;}
    Sat operator ++(int) {
        auto novi = *this;
        ++(*this);
        return novi;
    }

    friend bool operator ==(const Sat &s1, const Sat &s2){
        return s1.sati == s2.sati && s1.minute == s2.minute && s1.sekunde == s2.sekunde;
    }
    friend bool operator !=(const Sat &s1, const Sat &s2) {return !(s1==s2);}

    friend std::ostream &operator <<(std::ostream &tok, Sat s);

    friend int operator -(const Sat &s1, const Sat &s2);
};

Sat &Sat::PostaviNormalizirano(int sati, int minute, int sekunde){
    int ukupno = (3600 * sati + 60 * minute + sekunde) % 86400;
    if(ukupno < 0) ukupno += 86400;
    Sat::sati = ukupno / 3600;
    Sat::minute = (ukupno % 3600) / 60;
    Sat::sekunde = ukupno % 60;
    return *this;
}

std::ostream &operator<<(std::ostream &tok, Sat s){
    if(s.sati < 10) tok<<"0";
    tok<<s.sati<<":";
    if(s.minute < 10) tok<<"0";
    tok<<s.minute<<":";
    if(s.sekunde<10)tok<<"0";
    tok<<s.sekunde;
    return tok;
}

int operator -(const Sat &s1, const Sat &s2){
    return 3600 * (s1.sati - s2.sati) + 60 * (s1.minute - s2.minute) + s1.sekunde - s2.sekunde;
}

struct Grad{
    char *ime;
    int broj_stanovnika;
};

class Stanovnistvo{
    Grad* gradovi;
    int kapacitet;
    int brojGradova;
public:
    explicit Stanovnistvo(int max);
    ~Stanovnistvo();
    Stanovnistvo(const Stanovnistvo &drugo);
    Stanovnistvo(Stanovnistvo &&drugo);
    Stanovnistvo &operator =(Stanovnistvo drugo);
    void DodajGrad(const char *ime, int broj_stanovnika);
    Grad operator [](const char *c);
    const Grad operator [](const char *c) const;
    void ObrisiGrad(const char *c);
    friend std::ostream &operator <<(std::ostream &tok, const Stanovnistvo &s);
};

std::ostream &operator <<(std::ostream &tok, const Stanovnistvo &s){
    for(int i=0; i<s.brojGradova; i++){
        tok<<s.gradovi[i].ime<<": "<<s.gradovi[i].broj_stanovnika<<std::endl;
    }
    return tok;
}

void Stanovnistvo::ObrisiGrad(const char *c){
    for(int i=0; i<brojGradova; i++) {
        if(std::strcmp(c, gradovi[i].ime) == 0){
            delete[] gradovi[i].ime;
            for(int j=i; j<brojGradova-1; j++) gradovi[j] = gradovi[j+1];
            brojGradova--;
            return;
        }
    }
}

Grad Stanovnistvo::operator[](const char *c){
    for(int i=0; i<brojGradova; i++) if(std::strcmp(c, gradovi[i].ime) == 0) return gradovi[i];
    throw std::domain_error ("Nepoznat grad!");
}

const Grad Stanovnistvo::operator[](const char *c) const {
    for(int i=0; i<brojGradova; i++) if(std::strcmp(c, gradovi[i].ime) == 0) return gradovi[i];
    throw std::domain_error ("Nepoznat grad!");
}

void Stanovnistvo::DodajGrad(const char *ime, int broj_stanovnika){
    if (brojGradova >= kapacitet)
        throw std::range_error("Kolekcija je puna!");
    if (broj_stanovnika <= 0)
        throw std::domain_error("Neispravan broj stanovnika!");

    for (int i = 0; i < brojGradova; i++) {
        if (std::strcmp(ime, gradovi[i].ime) == 0)
            throw std::domain_error("Grad već postoji!");
    }

    size_t len = std::strlen(ime);
    gradovi[brojGradova].ime = new char[len + 1];
    std::strcpy(gradovi[brojGradova].ime, ime);
    gradovi[brojGradova].broj_stanovnika = broj_stanovnika;
    brojGradova++;
}


Stanovnistvo::Stanovnistvo(int max) : kapacitet(max), brojGradova(0), gradovi(new Grad[kapacitet]{}){
    for(int i=0; i<kapacitet; i++){
        gradovi[i].ime = nullptr;
        gradovi[i].broj_stanovnika = 0;
    }
}

Stanovnistvo::~Stanovnistvo(){
    for(int i = 0; i < kapacitet; i++) {
        if (gradovi[i].ime != nullptr)
            delete[] gradovi[i].ime;
    }
    delete[] gradovi;

}

Stanovnistvo::Stanovnistvo(const Stanovnistvo &drugo)
    : kapacitet(drugo.kapacitet),
      brojGradova(drugo.brojGradova),
      gradovi(new Grad[drugo.kapacitet]{}) // ← initialize with zeroed Grad structs
{
    for (int i = 0; i < brojGradova; i++) {
        if (drugo.gradovi[i].ime == nullptr)
            throw std::logic_error("drugo.gradovi[i].ime is nullptr — corrupted Grad!"); // 💣 DEBUG GUARD

        size_t len = std::strlen(drugo.gradovi[i].ime);
        gradovi[i].ime = new char[len + 1];
        std::strcpy(gradovi[i].ime, drugo.gradovi[i].ime);
        gradovi[i].broj_stanovnika = drugo.gradovi[i].broj_stanovnika;
    }

    for (int i = brojGradova; i < kapacitet; i++) {
        gradovi[i].ime = nullptr;
        gradovi[i].broj_stanovnika = 0;
    }
}



Stanovnistvo::Stanovnistvo(Stanovnistvo &&drugo) : kapacitet(drugo.kapacitet), brojGradova(drugo.brojGradova), gradovi(drugo.gradovi){
    drugo.gradovi = nullptr;
    drugo.kapacitet=0;
    drugo.brojGradova=0;
}

Stanovnistvo &Stanovnistvo::operator=(Stanovnistvo drugo){
    std::swap(kapacitet, drugo.kapacitet);
    std::swap(brojGradova, drugo.brojGradova);
    std::swap(gradovi, drugo.gradovi);
    return *this;
}

int main(){

    Stanovnistvo s1(20);
    s1.DodajGrad("Travnik", 25000);
    s1.DodajGrad("Sarajevo", 2500000);
    std::cout<<s1;
    auto s2 = s1;
    std::cout<<s2<<s2["Travnik"].ime<<s1["Sarajevo"].broj_stanovnika;

    return 0;
}