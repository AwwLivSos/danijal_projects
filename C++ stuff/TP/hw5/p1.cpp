//TP 2023/2024: Zadaća 5, Zadatak 1
#include <initializer_list>
#include <ios>
#include <iostream>
#include <cmath>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <fstream>
#include <sstream>

class Razlomak{
    long long int brojnik;
    long long int nazivnik;
    static void skrati(long long int &a, long long int &b);
    static long long int NZD(long long int a, long long int b);
    static void provjeriPrekoracenjeZaMnozenje(long long int x, long long int y);
    static void provjeriPrekoracenjeZaSabiranje(long long int x, long long int y);
    static void provjeriPrekoracenjeZaOduzimanje(long long int x, long long int y);
    static void provjeriPrekoracenjeZaDijeljenje(long long int x, long long int y);
public:
    Razlomak(long long int br=0, long long int nz=1);
    Razlomak(std::initializer_list<long long int> lista);
    long long int DajBrojnik() const {return brojnik;}
    long long int DajNazivnik() const {return nazivnik;}

    friend Razlomak operator +(const Razlomak &p, const Razlomak &q);
    friend Razlomak operator -(const Razlomak &p, const Razlomak &q);
    friend Razlomak operator *(const Razlomak &p, const Razlomak &q);
    friend Razlomak operator /(const Razlomak &p, const Razlomak &q);

    Razlomak operator +() const {return *this;}
    Razlomak operator -() const {
        if(brojnik == std::numeric_limits<long long int>::min()) throw std::overflow_error("Nemoguce dobiti tacan rezultat");
        return Razlomak(-brojnik, nazivnik);
    }

    Razlomak &operator +=(const Razlomak &p){ *this = *this + p; return *this; }
    Razlomak &operator -=(const Razlomak &p){ *this = *this - p; return *this; }
    Razlomak &operator *=(const Razlomak &p){*this = *this * p; return *this; }
    Razlomak &operator /=(const Razlomak &p){ *this = *this / p; return *this; }
    Razlomak &operator ++(){ *this+=Razlomak(1); return *this; }
    Razlomak &operator --(){ *this-=Razlomak(1); return *this; }
    Razlomak operator --(int){
        Razlomak pom(*this);
        --(*this);
        return pom;
    }
    Razlomak operator ++(int){
        Razlomak pom(*this);
        ++(*this);
        return pom;
    }

    friend bool operator ==(const Razlomak &p, const Razlomak &q) {return p.brojnik == q.brojnik && p.nazivnik == q.nazivnik;}
    friend bool operator !=(const Razlomak &p, const Razlomak &q) {return !(p==q);}
    friend bool operator <(const Razlomak &p, const Razlomak &q) {return (static_cast<long double>(p) < static_cast<long double>(q));}          //mozda prvo skrati
    friend bool operator <=(const Razlomak &p, const Razlomak &q) {return (static_cast<long double>(p) <= static_cast<long double>(q));}
    friend bool operator >(const Razlomak &p, const Razlomak &q) {return (static_cast<long double>(p) > static_cast<long double>(q));}
    friend bool operator >=(const Razlomak &p, const Razlomak &q) {return (static_cast<long double>(p) >= static_cast<long double>(q));}

    friend std::ostream &operator <<(std::ostream &tok, const Razlomak &r);
    friend std::istream &operator >>(std::ostream &tok, Razlomak &r);
    
    operator long double() const { return static_cast<long double>(brojnik)/nazivnik;}
};

void Razlomak::provjeriPrekoracenjeZaMnozenje(long long int x, long long int y){                //potencijalno dodati jos provjera!!
    if(y>0 && (x>std::numeric_limits<long long int>::max()/y || x<std::numeric_limits<long long int>::min()/y))
        throw std::overflow_error("Nemoguce dobiti tacan rezultat");
    if(y<0 && x>0 && (y>std::numeric_limits<long long int>::max()/x || y<std::numeric_limits<long long int>::min()/x))
        throw std::overflow_error("Nemoguce dobiti tacan rezultat");
}

void Razlomak::provjeriPrekoracenjeZaSabiranje(long long int x, long long int y){
    if((y>0 && x>std::numeric_limits<long long int>::max()-y ) || (y<0 && x<std::numeric_limits<long long int>::min()-y))
        throw std::overflow_error("Nemoguce dobiti tacan rezultat");
}

void Razlomak::provjeriPrekoracenjeZaOduzimanje(long long int x, long long int y) {             //ovo je klimavo
    if ((y > 0 && x < std::numeric_limits<long long int>::max() + y) || (y < 0 && x > std::numeric_limits<long long int>::min() + y))
        throw std::overflow_error("Nemoguce dobiti tacan rezultat");
}

void Razlomak::provjeriPrekoracenjeZaDijeljenje(long long int x, long long int y){              //i ovdje mozda bude trebalo jos provjera
    if(x==std::numeric_limits<long long int>::min() && y==-1)
        throw std::overflow_error("Nemoguce dobiti tacan rezultat");
}

void Razlomak::skrati(long long int &a, long long int &b) {
    auto nzd=Razlomak::NZD(a, b);
    a/=nzd;
    b/=nzd;
    if(b<0) a*=-1, b*=-1;
}

Razlomak::Razlomak(long long int br, long long int nz){
    if(nz==0) throw std::logic_error("Nekorektan razlomak");
    skrati(br, nz);
    brojnik=br;
    nazivnik=nz;
}

Razlomak::Razlomak(std::initializer_list<long long int> lista){
    auto it =lista.begin();
    if(lista.size()>=3) throw std::logic_error("Niđe veze");
    if(lista.size()==1) *this = Razlomak(*it);
    else *this = Razlomak(*it, *(it+1));
}

Razlomak operator +(const Razlomak &p, const Razlomak &q){
    auto p1 = p.brojnik;
    auto p2 = q.brojnik;
    auto q1 = p.nazivnik;
    auto q2 = q.nazivnik;
    auto r = Razlomak::NZD(q1, q2);
    Razlomak::provjeriPrekoracenjeZaDijeljenje(q2, r);
    Razlomak::provjeriPrekoracenjeZaDijeljenje(q1, r);
    Razlomak::provjeriPrekoracenjeZaMnozenje(p1, q2/r);
    Razlomak::provjeriPrekoracenjeZaMnozenje(p2, q1/r);
    Razlomak::provjeriPrekoracenjeZaMnozenje(q1, q2/r);
    Razlomak::provjeriPrekoracenjeZaSabiranje(p1*(q2/r), p2*(q1/r));

    return Razlomak(p1*(q2/r)+p2*(q1/r), q1*(q2/r));
}

Razlomak operator -(const Razlomak &p, const Razlomak &q){
    auto p1 = p.brojnik;
    auto p2 = q.brojnik;
    auto q1 = p.nazivnik;
    auto q2 = q.nazivnik;
    auto r = Razlomak::NZD(q1, q2);
    Razlomak::provjeriPrekoracenjeZaDijeljenje(q2, r);
    Razlomak::provjeriPrekoracenjeZaDijeljenje(q1, r);
    Razlomak::provjeriPrekoracenjeZaMnozenje(p1, q2/r);
    Razlomak::provjeriPrekoracenjeZaMnozenje(p2, q1/r);
    Razlomak::provjeriPrekoracenjeZaMnozenje(q1, q2/r);
    Razlomak::provjeriPrekoracenjeZaOduzimanje(p1*(q2/r), p2*(q1/r));

    return Razlomak(p1*(q2/r)-p2*(q1/r), q1*(q2/r));
}

Razlomak operator *(const Razlomak &p, const Razlomak &q){
    auto p1 = p.brojnik;
    auto p2 = q.brojnik;
    auto q1 = p.nazivnik;
    auto q2 = q.nazivnik;
    auto s = Razlomak::NZD(p1, q2);
    auto t = Razlomak::NZD(p2, q1);
    Razlomak::provjeriPrekoracenjeZaDijeljenje(p1, s);
    Razlomak::provjeriPrekoracenjeZaDijeljenje(p2, t);
    Razlomak::provjeriPrekoracenjeZaDijeljenje(q1, t);
    Razlomak::provjeriPrekoracenjeZaDijeljenje(q2, s);
    Razlomak::provjeriPrekoracenjeZaMnozenje(p1/s, p2/t);
    Razlomak::provjeriPrekoracenjeZaMnozenje(q1/t, q2/s);

    return Razlomak((p1/s)*(p2/t), (q1/t)*(q2/s));
}

Razlomak operator /(const Razlomak &p, const Razlomak &q){
    auto p1 = p.brojnik;
    auto p2 = q.brojnik;
    auto q1 = p.nazivnik;
    auto q2 = q.nazivnik;
    auto r = Razlomak::NZD(q1, q2);
    auto u = Razlomak::NZD(p1, p2);
    Razlomak::provjeriPrekoracenjeZaDijeljenje(p1, u);
    Razlomak::provjeriPrekoracenjeZaDijeljenje(q2, r);
    Razlomak::provjeriPrekoracenjeZaDijeljenje(q1, r);
    Razlomak::provjeriPrekoracenjeZaDijeljenje(p2, u);
    Razlomak::provjeriPrekoracenjeZaMnozenje(p1/u, q2/r);
    Razlomak::provjeriPrekoracenjeZaMnozenje(q1/r, p1/u);

    return Razlomak((p1/u)*(q2/r), (q1/r)*(p2/u));
}

long long int Razlomak::NZD(long long int a, long long int b){
    if(b==0) return std::abs(a);                                        //upitno!!
    else return NZD(b, a%b);
}

std::ostream &operator <<(std::ostream &tok, const Razlomak &r){    
    tok<<r.brojnik;                                                    
    if(r.nazivnik!=1) tok<<"/"<<r.nazivnik;
    return tok;
}

std::istream &operator >>(std::istream &tok, Razlomak &r){
    long long int a, b=1;
    char slesh;

    tok>>std::ws>>a;

    if(tok.peek()=='/') {
        tok>>slesh>>b;
        if(slesh!='/' || !tok){
            tok.setstate(std::ios::failbit);
            return tok;
        }
    }

    if(tok.peek() != std::istream::traits_type::eof() && !std::isspace(tok.peek())){
        tok.setstate(std::ios::failbit);
        return tok;
    }
    r=Razlomak(a, b);
    return tok;
}

int main ()
{
	return 0;
}