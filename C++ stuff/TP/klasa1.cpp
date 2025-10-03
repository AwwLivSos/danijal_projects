#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>

class Krug{
    double radijus;
public:
    double povrsina(){
       return DajRadijus()*DajRadijus()*3.1415923;
    }
    double obim(){
        return radijus*2*3.1415923;
    }

    void namjestiRadijus(double radijus){
        Krug::radijus=radijus;
    }
    double DajRadijus() const{
        return radijus;
    }
};

class Pravougaonik{
    double visina, sirina;
public:
    double Obim(){
        return 2*visina+2*sirina;
    }
    double Povrsina(){
        return visina*sirina;
    }
};


class Osoba{
    std::string ime;
    int dob;
    std::string drzava;
public:
    std::string dajIme() const {return ime;}
    int dajDob() const {return dob;}
    std::string dajDrzavu() const {return drzava;}

    void postaviIme(const std::string &s){
        ime=s;
    }
    void postaviDob(int d){
        dob = d;
    }
    void postaviDrzavu(const std::string &drzava){
        Osoba::drzava=drzava;
    }
};

class Automobil{
    std::string proizvodjac, model;
    int godina;
public:
    std::string dajProizvodjaca() const {return proizvodjac;}
    std::string dajModel() const {return model;}
    int dajGodinu() const {return godina;}

    void postaviProizvodjaca(const std::string &s){proizvodjac=s;}
    void postaviModel(const std::string &s){model=s;}
    void postaviGodinu(int g){godina=g;}

    friend int main();
};

/*Write a C++ program to implement a class called Date
that has private member variables for day, month, and year. 
Include member functions to set and get these variables,
as well as to validate if the date is valid.*/

class Datum{
    int dan, mjesec, godina;
public:
    int dajDan() const {return dan;}
    int dajMjesec() const {return mjesec;}
    int dajGodinu() const {return godina;}

    void PostaviDatum(int dan, int mjesec, int godina) {
        int broj_dana[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0) broj_dana[1]++;

        if(godina < 1 || mjesec < 1 || mjesec > 12 || dan < 1 || dan > broj_dana[mjesec - 1])
            throw std::domain_error("Neispravan datum!");

        this->dan = dan; this->mjesec = mjesec; this->godina = godina;
    }
};

class Kutija{
    int l, b, h;
public:
    Kutija(){
        l=b=h=0;
    }

    Kutija(int length, int breadth, int height){
        l=length;
        b=breadth;
        h=height;
    }

    Kutija(const Kutija &druga){
        l=druga.l;
        b=druga.b;
        h=druga.h;
    }

    int getLength() const {return l;}
    int getBreadth() const {return b;}
    int getHeight() const {return h;}

    long long int getVolume() const {return l*(long long int)b*h;}
};

class Vrijeme{
    int sati, minute, sekunde;
public:
    Vrijeme() : sati(0), minute(0), sekunde(0) {}

    Vrijeme(int h, int m, int s) : sati(h), minute(m), sekunde(s) {
        if(h<0 || m<0 || s<0) throw std::domain_error("Neispravno vrijeme!");

        while(sekunde>=60){
            sekunde-=60;
            minute++;
        }
        while(minute>=60){
            minute-=60;
            sati++;
        }

        while(sati>=24) {sati-=24;}
    }

    void Ispisi(){
        std::cout<<sati<<":"<<minute<<":"<<sekunde<<std::endl;
    }

    int dajSate() const {return sati;}
    int dajMinute() const {return minute;}
    int dajSekunde() const {return sekunde;}

    friend Vrijeme SaberiVrijeme(const Vrijeme &v1, const Vrijeme &v2);
};

Vrijeme SaberiVrijeme(const Vrijeme &v1, const Vrijeme &v2){
    int noviH = v1.dajSate() + v2.dajSate();
    int noviM = v1.dajMinute() + v2.dajMinute();
    int noviS = v1.dajSekunde() + v2.dajSekunde();

    return Vrijeme(noviH, noviM, noviS);
}

int main(){
    std::vector<int> v(10, 10);
    std::vector<int> w(v);

    Vrijeme v1(200,200,200);
    v1.Ispisi();
    Vrijeme v2(100,100,100);
    v2.Ispisi();

    Vrijeme v3 = SaberiVrijeme(v1, v2);
    v3.Ispisi();

    return 0;
}