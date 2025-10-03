#include <list>
#include <iostream>
#include <complex>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <stack>
#include <fstream>
#include <complex>

struct Kompleksni {
    double re, im;
    Kompleksni *sljedeci;
};

Kompleksni* KreirajListu(const std::list<std::complex<double>> &lista){
    if(lista.empty()) return nullptr;

    Kompleksni* prvi{};
    Kompleksni* pomocni{};

    for(auto broj : lista){
        Kompleksni* novi = new Kompleksni{broj.real(), broj.imag(), nullptr};

        if(!prvi)
            prvi = novi;
        else 
            pomocni->sljedeci = novi;

        pomocni = novi;
    }

    return prvi;
}

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

class KlasaA {
    int x, y;
public:
    KlasaA(int xx = 0, int yy = 0) : x(xx), y(yy) {}
    virtual int DajX() const { return x; }
    virtual int DajY() const { return y; }
    virtual ~KlasaA() {}
};

class KlasaB : public KlasaA {
    int z;
public:
    KlasaB(int x = 0, int y = 0, int z = 0) : KlasaA(x, y) {this->z = z;}
    int DajX() const override { return KlasaA::DajX() + z; }
    int DajY() const override { return KlasaA::DajY() + z; }
    int DajZ() { return z; }
};

template<typename Tip>
class Skup{
    std::vector<Tip> elementi;
public:
    Skup() = default;

    Skup(std::initializer_list<Tip> lista){
        for(auto elem : lista)
            Dodaj(elem);
    }

    int Velicina() const {return elementi.size();}

    void Dodaj(Tip elem){
        auto it = std::lower_bound(elementi.begin(), elementi.end(), elem);
        if(it == elementi.end() || *it != elem)
            elementi.insert(it, elem);
    }

    void Izbrisi(Tip elem){
        auto it = std::lower_bound(elementi.begin(), elementi.end(), elem); 
        if(it != elementi.end() && *it == elem)
            elementi.erase(it);
    }

    bool ImaLiGa(Tip elem) const{
        return std::binary_search(elementi.begin(), elementi.end(), elem);
    } 

    void Ispisi() const{
        for(const auto &x : elementi) std::cout<<x<<" ";
    }
};

template <typename Tip>
std::ostream& operator<<(std::ostream &tok, const std::stack<Tip> &stek){
    auto pomocni = stek;

    while(!pomocni.empty()){
        tok << pomocni.top()<<" ";
        pomocni.pop();
    }

    return tok;
}

template <typename Tip>
std::stack<Tip> operator-(const std::stack<Tip>& stek1, const std::stack<Tip>& stek2){
    if(stek1.size() != stek2.size()) throw std::domain_error("Stekovi moraju biti iste velicine!");
    
    std::stack<Tip> rezultat;
    auto pomocni1 = stek1;
    auto pomocni2 = stek2;

    while(!pomocni1.empty()){
        rezultat.push(pomocni1.top() - pomocni2.top());
        pomocni1.pop();
        pomocni2.pop();
    }

    return rezultat;
}

template <typename Tip>
std::stack<Tip>& operator*=(std::stack<Tip> &stek, int faktor){
    std::stack<Tip> pomocni;

    while(!stek.empty()){
        pomocni.push(stek.top() * faktor);
        stek.pop();
    }

    while(!pomocni.empty()){
        stek.push(pomocni.top());
        pomocni.pop();
    }

    return stek;
}

template <typename Tip>
bool operator*(const std::stack<Tip> &s){
    return !s.empty();
}

class Artikal{
    std::string naziv;
    double cijena;

public:
    Artikal(const std::string& naziv, double cijena) : naziv(naziv), cijena(cijena) {}
    virtual ~Artikal() = default;

    std::string DajNaziv() const {return naziv;}
    double DajCijenu() const {return cijena;}

    virtual void Ispisi() const = 0;
    virtual Artikal* DajKopiju() const = 0;
};

class Brasno : public Artikal{
    double kilogrami;

public:
    Brasno(double cijena, double kilogrami) : Artikal("Brasno", cijena), kilogrami(kilogrami) {}
    Brasno(const Brasno &drugi) : Artikal(drugi), kilogrami(drugi.kilogrami) {}
    Brasno(Brasno &&drugi) : Artikal(drugi), kilogrami(drugi.kilogrami) {}

    double DajKilograme() const {return kilogrami;}

    void Ispisi() const override{
        std::cout<<"Brasno od "<<DajCijenu()<<"KM ("<<DajKilograme()<<"kg)";
    }

    Artikal* DajKopiju() const override {return new Brasno(*this);}
};

class Ulje : public Artikal{
    double litri;

public:
    Ulje(double cijena, double litri) : Artikal("Ulje", cijena), litri(litri) {}
    Ulje(const Ulje &drugi) : Artikal(drugi), litri(drugi.litri) {}
    Ulje(Ulje &&drugi) : Artikal(drugi), litri(drugi.litri) {}

    double DajLitre() const {return litri;}
    void Ispisi() const override {
        std::cout<<"Ulje od "<<DajCijenu()<<"KM ("<<DajLitre()<<"l)";
    }
    Artikal* DajKopiju() const override {return new Ulje(*this);}
};

class Prodavnica{
    std::vector<Artikal*> artikli;

public:
    Prodavnica() = default;
    Prodavnica(const Prodavnica &p){
        for(auto a : p.artikli)
            artikli.push_back(a->DajKopiju());
    }
    Prodavnica(Prodavnica &&p) : artikli(p.artikli){
        p.artikli.clear();
    }
    ~Prodavnica() {
        for(auto a : artikli){
            delete a;
            a = nullptr;
        }
    }
    Prodavnica& operator=(Prodavnica p){
        std::swap(artikli, p.artikli);

        return *this;
    }

    void DodajUlje(const Ulje &u){
        artikli.push_back(new Ulje(u));
    }
    void DodajBrasno(const Brasno &b){
        artikli.push_back(new Brasno(b));
    }
    void DodajUlje(double cijena, double litri){
        artikli.push_back(new Ulje(cijena, litri));
    }
    void DodajBrasno(double cijena, double kilogrami){
        artikli.push_back(new Brasno(cijena, kilogrami));
    }

    void Ispisi() const{
        for(auto a : artikli){
            a->Ispisi();
            std::cout<<std::endl;
        }
    }
    Artikal* operator[](int i){
        if(i<0 || i>artikli.size()) throw std::out_of_range("Indeks izvan opsega!");
        return artikli[i];
    }

};

class Zivotinja{
public:
    virtual ~Zivotinja() = default;
    virtual void OglasiSe() const = 0;
    virtual Zivotinja* DajKopiju() const = 0;
};

class Pas : public Zivotinja{
public:
    void OglasiSe() const override {std::cout<<"Av!";}
    Zivotinja* DajKopiju() const override {return new Pas(*this);}
};

class Macka : public Zivotinja{
public:
    void OglasiSe() const override {std::cout<<"Maju!";}
    Zivotinja* DajKopiju() const override {return new Macka(*this);}
};

class Krava : public Zivotinja{
public:
    void OglasiSe() const override {std::cout<<"Muu!";}
    Zivotinja* DajKopiju() const override {return new Krava(*this);}
};

class PolimorfnaZivotinja{
    Zivotinja* z;
    void Provjera() const {if(!z) throw std::logic_error("Nepoznata vrsta zivotinje!");}
public:
    PolimorfnaZivotinja() : z(nullptr) {}
    PolimorfnaZivotinja(const PolimorfnaZivotinja &pz){
        if(pz.z == nullptr) z = nullptr;
        else z = pz.z->DajKopiju();
    }
    PolimorfnaZivotinja& operator=(const PolimorfnaZivotinja& pz){
        if(this != &pz){
            delete z;
            if(pz.z == nullptr) z = nullptr;
            else z = pz.z->DajKopiju();
        }
        return *this;
    }
    PolimorfnaZivotinja(const Zivotinja& druga){
        z = druga.DajKopiju();
    }

    ~PolimorfnaZivotinja() {delete z;}

    void OglasiSe() const{
        Provjera();
        z->OglasiSe();
    }
};

int main(){

    /* std::list<std::complex<double>> lista{{1,2}, {3,4}, {5,6}};

    auto pocetak = KreirajListu(lista);
    auto temp = pocetak;

    while(temp != nullptr){
        std::cout<<temp->re<<"+"<<temp->im<<"i"<<std::endl;
        temp = temp->sljedeci;
    }

    while(pocetak != nullptr){
        auto pomocni = pocetak;
        pocetak = pocetak->sljedeci;

        delete pomocni;
        pomocni = nullptr;
    } */

    /* Kalamuta *pk = new Kalamuta, *ck = new Cincozna;
    Cincozna *cc = new Cincozna;
    std::cout << pk->foo() << " " << pk->bar() << " " << pk->baz() << std::endl;        //5 4 3
    std::cout << ck->foo() << " " << ck->bar() << " " << ck->baz() << std::endl;        //6 4 3
    std::cout << cc->foo() << " " << cc->bar() << " " << cc->baz() << std::endl;        //6 8 12
    delete pk; delete cc; delete ck; */

    /* KlasaA ka1(2,1), ka2(5, 3), ka3 (3);
    KlasaB kb1(1,2,3), kb2(1,2), kb3 (4, 2, 1);
    std::cout << ka1.DajX() << " " << ka1.DajY() << std::endl; // 2 1
    std::cout << ka2.DajX() << " " << ka2.DajY() << std::endl; // 5 3
    std::cout << ka3.DajX() << " " << ka3.DajY() << std::endl; // 3 0
    std::cout << kb1.DajX() << " " << kb1.DajY() << " " << kb1.DajZ() << "\n"; // 4 5 3
    std::cout << kb2.DajX() << " " << kb2.DajY() << " " << kb2.DajZ() << "\n"; // 1 2 0
    std::cout << kb3.DajX() << " " << kb3.DajY() << " " << kb3.DajZ() << "\n"; // 5 3 1 */

    /* Skup<int> s{3,89,62,3,5,5};
    s.Dodaj(9);
    s.Dodaj(3);
    s.Izbrisi(89);
    std::cout<<s.ImaLiGa(5)<<std::endl;
    s.Ispisi(); */

    /* int niz1[]{1, 5, 7, 3}, niz2[]{1, 9, 0, 4};
    std::stack<int> q1, q2;
    for(auto i : niz1) q1.push(i); // ubacimo elemente
    for(auto i : niz2) q2.push(i); // redoslijed je OBRNUT
    std::cout << q2 << std::endl; // 4 0 9 1 - ispis
    // moraju biti istih dimenzija, inace baciti izuzetak po volji!
    std::cout << q1 - q2 <<std::endl; // 0, -4, 7, -1 ispis
    q1 *= 3; // q1 = {9, 21, 15, 3} - redoslijed OBRNUT
    std::cout << q1 << std::endl;
    if(*q1) std::cout << "Nije prazan";
    else std::cout << "Prazan"; */

    /* Prodavnica p, p2;
    Ulje u (2, 1); Brasno b (1.1, 1);
    p.DodajUlje(u); 
    p.DodajUlje(7, 5);
    p.DodajBrasno(17, 20); 
    p.DodajBrasno(b);

    Prodavnica p3(p);
    p3.Ispisi(); //svaki artikal, u novom redu
    p2 = p;
    Artikal *a = p2[0];
    a->Ispisi(); // Ulje 1l, 2KM
    p2[2]->Ispisi(); // Brasno 17kg, 20KM */

    std::ifstream ulaz("KOMPLEKSNI.TXT");
    if(!ulaz){
        std::cout<<"Datpteka ne postoji";
        return 0;
    }

    std::ofstream izlaz("KOMPLEKSNI.DAT", std::ios::binary);
    
    double realni, imaginarni;
    char zarez;

    while(ulaz >> realni >> zarez >> imaginarni){
        std::complex<double> broj(realni, imaginarni);
        izlaz.write(reinterpret_cast<char *>(&broj), sizeof(broj));
    }

    if(!ulaz.eof()){
        std::cout<<"Greska pri citanju iz dtaoteke";
        return 0;
    }

    izlaz.close();
    ulaz.close();

    std::fstream binarni("KOMPLEKSNI.DAT", std::ios::binary | std::ios::in | std::ios::out);
    if(!binarni){
        std::cout<<"Datpteka ne postoji";
        return 0;
    }

    std::complex<double> broj;
    while(binarni.read(reinterpret_cast<char *>(&broj), sizeof(broj))){
        std::complex<double> razmijenjeni(broj.imag(), broj.real());
        binarni.seekp(-sizeof(std::complex<double>), std::ios::cur);
        binarni.write(reinterpret_cast<char*>(&razmijenjeni), sizeof(razmijenjeni));
    }

    if(!binarni.eof()){
        std::cout<<"Greska pri citanju ili pisanju datoteke";
        return 0;
    }

    binarni.seekg(0, std::ios::beg);
    while(binarni.read(reinterpret_cast<char*>(&broj), sizeof(broj))){
        std::cout<<broj.real()<<" + "<<broj.imag()<<"i"<<std::endl;
    }
    binarni.close();

    return 0;
}