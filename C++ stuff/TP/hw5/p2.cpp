//TP 2023/2024: Zadaća 5, Zadatak 2
#include <iomanip>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <algorithm> 
#include <functional>
#include <iterator>

class Berza{
    std::vector<int> cijene;
    int donja_granica, gornja_granica;
public:
    explicit Berza(int a) : gornja_granica(a), donja_granica(0) {if (a<0) throw std::range_error("Ilegalne granicne cijene");}
    Berza(int a, int b) : gornja_granica(b), donja_granica(a) {if (a<0 || b<0 || b<a) throw std::range_error("Ilegalne granicne cijene");}
    void RegistrirajCijenu(int n);
    int DajBrojRegistriranihCijena() const {return cijene.size();}
    void BrisiSve() {cijene.clear();}
    int DajMinimalnuCijenu() const;
    int DajMaksimalnuCijenu() const;
    bool operator !() const {return cijene.empty();}
    int DajBrojCijenaVecihOd(int n) const;
    void Ispisi() const;
    int operator [](int i) const;
    Berza &operator ++();
    Berza operator ++(int);
    Berza &operator --();
    Berza operator --(int);
    Berza operator -() const;
    Berza &operator +=(int b) {*this = *this + b; return *this;}
    Berza &operator -=(int b) {*this = *this - b; return *this;}
    Berza &operator +=(const Berza &b) {*this = *this + b; return *this;}
    Berza &operator -=(const Berza &b) {*this = *this - b; return *this;}

    friend Berza operator +(const Berza &b, int a);
    friend Berza operator -(const Berza &b, int a);
    friend Berza operator +(int a, const Berza &b) {return b+a;}
    friend Berza operator -(int a, const Berza &b);
    friend Berza operator +(const Berza &b1, const Berza &b2);
    friend Berza operator -(const Berza &b1, const Berza &b2);
    friend bool operator ==(const Berza &b1, const Berza &b2);
    friend bool operator !=(const Berza &b1, const Berza &b2);
};

void Berza::RegistrirajCijenu(int n){
    if (n<donja_granica || n>gornja_granica) throw std::range_error("Ilegalna cijena");
    cijene.push_back(n);
}

int Berza::DajMinimalnuCijenu() const {
    if(cijene.empty()) throw std::range_error("Nema registriranih cijena");
    return *std::min_element(cijene.begin(), cijene.end());
}

int Berza::DajMaksimalnuCijenu() const{
    if(cijene.empty()) throw std::range_error("Nema registriranih cijena");
    return *std::max_element(cijene.begin(), cijene.end());
}

int Berza::DajBrojCijenaVecihOd(int n) const{
    if(cijene.empty()) throw std::range_error("Nema registriranih cijena");
    return std::count_if(cijene.begin(), cijene.end(), std::bind(std::greater<int>(), std::placeholders::_1, n));       //wtf
}

void Berza::Ispisi() const{
    std::vector<double> help(cijene.size());
    std::copy(cijene.begin(), cijene.end(), help.begin());
    std::sort(help.rbegin(), help.rend());                                                                              //WTF?!?
    std::transform(help.begin(), help.end(), std::ostream_iterator<double>(std::cout<<std::setprecision(2)<<std::fixed, "\n"), bind(std::divides<double>(), std::placeholders::_1, 100.));
}

int Berza::operator [](int i) const{
    if(i<1 || i>cijene.size()) throw std::range_error("Neispravan indeks");
    return cijene.at(i-1);
}

Berza &Berza::operator++(){
    if(std::any_of(cijene.begin(), cijene.end(), std::bind(std::greater<int>(), std::placeholders::_1, gornja_granica-100)))
        throw std::range_error("Prekoracen dozvoljeni opseg cijena");
    std::transform(cijene.begin(), cijene.end(), cijene.begin(), std::bind(std::plus<int>(), std::placeholders::_1, 100));
    return *this;
}

Berza Berza::operator++(int){
    Berza pom(*this);
    ++(*this);
    return pom;
}

Berza &Berza::operator--(){
    if(std::any_of(cijene.begin(), cijene.end(), std::bind(std::less<int>(), std::placeholders::_1, donja_granica+100)))
        throw std::range_error("Prekoracen dozvoljeni opseg cijena");
    std::transform(cijene.begin(), cijene.end(), cijene.begin(), std::bind(std::minus<int>(), std::placeholders::_1, 100));
    return *this;
}

Berza Berza::operator--(int){
    Berza pom(*this);
    --(*this);
    return pom;
}

Berza Berza::operator-() const {
    Berza pom(gornja_granica, donja_granica);
    std::transform(cijene.begin(), cijene.end(), std::back_inserter(pom.cijene), std::bind(std::minus<int>(), donja_granica+gornja_granica, std::placeholders::_1));
    return *this;
}

Berza operator +(const Berza &b, int a){
    auto nova(b);
    if(std::any_of(nova.cijene.begin(), nova.cijene.end(), std::bind(std::greater<int>(), std::placeholders::_1, nova.gornja_granica-a)) || 
       std::any_of(nova.cijene.begin(), nova.cijene.end(), std::bind(std::less<int>(), std::placeholders::_1, nova.donja_granica-a)))
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    std::transform(nova.cijene.begin(), nova.cijene.end(), nova.cijene.begin(), std::bind(std::plus<int>(), std::placeholders::_1, a));
    return nova;
}

Berza operator -(const Berza &b, int a){
    auto nova(b);
    if(std::any_of(nova.cijene.begin(), nova.cijene.end(), std::bind(std::greater<int>(), std::placeholders::_1, nova.gornja_granica+a)) || 
       std::any_of(nova.cijene.begin(), nova.cijene.end(), std::bind(std::less<int>(), std::placeholders::_1, nova.donja_granica+a)))
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    std::transform(nova.cijene.begin(), nova.cijene.end(), nova.cijene.begin(), std::bind(std::minus<int>(), std::placeholders::_1, a));
    return nova;
}

Berza operator -(int a, const Berza &b){
    Berza nova(b.donja_granica, b.gornja_granica);
    std::transform(b.cijene.begin(), b.cijene.end(), std::back_inserter(nova.cijene), std::bind(std::minus<int>(), a, std::placeholders::_1));
    if(std::any_of(nova.cijene.begin(), nova.cijene.end(), std::bind(std::greater<int>(), std::placeholders::_1, b.gornja_granica)) || 
       std::any_of(nova.cijene.begin(), nova.cijene.end(), std::bind(std::less<int>(), std::placeholders::_1, b.donja_granica)))
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    return nova;
}

Berza operator +(const Berza &b1, const Berza &b2){
    if(b1.cijene.size() != b2.cijene.size() || b1.donja_granica != b2.donja_granica || b1.gornja_granica != b2.gornja_granica)
        throw std::domain_error("Nesaglasni operandi");

    Berza nova(b1.donja_granica, b1.gornja_granica);
    std::transform(b1.cijene.begin(), b1.cijene.end(), b2.cijene.begin(), std::back_inserter(nova.cijene), std::plus<int>());
    if(std::any_of(nova.cijene.begin(), nova.cijene.end(), std::bind(std::greater<int>(), std::placeholders::_1, b1.gornja_granica)))
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
    return nova;
}

Berza operator -(const Berza &b1, const Berza &b2){
    if(b1.cijene.size() != b2.cijene.size() || b1.donja_granica != b2.donja_granica || b1.gornja_granica != b2.gornja_granica)
        throw std::domain_error("Nesaglasni operandi");
    
    Berza nova(b1.donja_granica, b1.gornja_granica);
    std::transform(b1.cijene.begin(), b1.cijene.end(), b2.cijene.begin(), std::back_inserter(nova.cijene), std::minus<int>());
    if(std::any_of(nova.cijene.begin(), nova.cijene.end(), std::bind(std::less<int>(), std::placeholders::_1, b1.donja_granica)))
        throw std::domain_error("Prekoracen dozvoljeni opseg cijena");

    return nova;
}

bool operator == (const Berza &b1, const Berza &b2){
    return b1.gornja_granica == b2.gornja_granica && b1.donja_granica == b2.donja_granica && b1.cijene.size() == b2.cijene.size() && equal(b1.cijene.begin(), b1.cijene.end(), b2.cijene.begin());
}

bool operator != (const Berza &b1, const Berza &b2){
    return !(b1==b2);
}

int main ()
{

	return 0;
}
