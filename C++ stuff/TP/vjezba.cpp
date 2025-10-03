#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <tuple>
#include <list>

template <typename Tip>
struct Datum{
    Tip dan, mjesec, godina;
};

struct Cvor{
    int broj;
    Cvor* sljedeci;
};

Cvor* StvoriListu(){
    Cvor* pocetak = new Cvor{1, nullptr};
    Cvor* pomocni = pocetak;

    for(int i = 2; i < 100; i++) {
        pomocni->sljedeci = new Cvor{i, nullptr};
        pomocni = pomocni->sljedeci;
    }

    return pocetak;
}

std::pair<int, int> vratiStudenta(){
    return {2,2};
}

template <typename Tip1, typename Tip2>
    struct Par{
        Tip1 first;
        Tip2 second;
    };

class Student{
    std::string ime;
    std::string prezime;
    int index = 0;
    int datumUpisa;

    static int brojStudenata;

public:
    Student& unesiIme(const std::string &ime){
        if(ime.length() == 0 || ime.length() > 100) throw std::domain_error("ime nije validno");

        Student::ime = ime;

        return *this;
    }

    std::string getIme() const {
        return ime;
    }

    void Ispisi() const{
        std::cout<<ime<<" "<<prezime;
    }
};

class Sat{
    int sati;
    int minute;
    int sekunde;
public:

    Sat(int sati = 0, int minute = 0, int sekunde = 0) : sati(sati), minute(minute), sekunde(sekunde){
        if(!DaLiJeIspravno(sati, minute, sekunde)) throw std::domain_error("Neispravno vrijeme!");
    }

    Sat(const Sat &drugi){
        sati = drugi.sati;
        minute = drugi.minute;
        sekunde = drugi.sekunde;
    }

    Sat(Sat &&drugi){
        sati = drugi.sati;
        minute = drugi.minute;
        sekunde = drugi.sekunde;
    }

    void Postavi(int h, int m, int s){
        if(!DaLiJeIspravno(h, m, s)) throw std::domain_error("Neispravno vrijeme!"); 
        sati = h;
        minute = m;
        sekunde = s;
    }

    bool DaLiJeIspravno(int h, int m, int s){
        return !(h<=0 || m<0 || s<0 || h>=24 || m>=60 || s>=60);
    }

    Sat &PostaviNormalizirano(int h, int m, int s){

        m += (s/60);
        s%=60;

        h += (m/60);
        m%=60;

        h%=24;

        if(s<0){
            s+=60;
            m--;
        }

        if(m<0){
            m+=60;
            h--;
        }

        if(h<0) h+=24;

        sati = h;
        minute = m;
        sekunde = s;

        return *this;
    }

    Sat &PomjeriZa(int brojSekundi){
        return PostaviNormalizirano(sati, minute, sekunde+brojSekundi);
    }

    Sat &Sljedeci(){
        return PomjeriZa(1);
    }

    Sat &Prethodni(){
        return PomjeriZa(-1);
    }

    void Ispisi() const{
        std::cout<<sati<<":"<<minute<<":"<<sekunde;
    }

    int DajSate() const {return sati;}
    int DajMinute() const {return minute;}
    int DajSekunde() const {return sekunde;}

    friend int BrojSekundiIzmedju(const Sat &s1, const Sat &s2);
};

int BrojSekundiIzmedju(const Sat &s1, const Sat &s2){
    int sekunde1 = s1.DajSekunde() + s1.DajMinute()*60 + s1.DajSate()*3600;
    int sekunde2 = s2.sekunde + s2.minute*60 + s2.sati*3600;

    return std::abs(sekunde1-sekunde2);
}

Sat f(){
    return Sat(1, 2, 3);
}

int main(){

    int broj = 6;
    std::vector<int> v(6);
    std::vector<int> w(v);

    Sat s1;
    s1.Ispisi();
    std::cout<<std::endl;
    s1.PostaviNormalizirano(1841, 18948, 601).PomjeriZa(121).Sljedeci().Ispisi();
    
    return 0;
}