#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <utility>
#include <initializer_list>
#include <algorithm>
#include <fstream>

class Cudo {
    std::string naziv;
    std::pair<std::string, std::string> lokacija; // Država i mjesto
    int godina_pronalaska;
public:
    Cudo(std::string s, std::pair<std::string, std::string> par, int g) : naziv(s), lokacija(par), godina_pronalaska(g) {}
    Cudo(std::string ime, std::string drzava, std::string mjesto, int g) : naziv(ime), godina_pronalaska(g), lokacija({drzava, mjesto}) {}

    std::string DajNaziv() const {return naziv;}
    std::pair<std::string, std::string> DajLokaciju() const {return lokacija;}
    int DajGodinu() const {return godina_pronalaska;}

    int& Godina() {return godina_pronalaska;}

    friend std::ostream& operator<<(std::ostream &tok, const Cudo &c){
        return tok<<c.DajNaziv()<<" "<<c.DajLokaciju().first<<" "<<c.DajLokaciju().second<<" "<<c.DajGodinu()<<std::endl;
    }
};
class SvjetskaCuda {
    std::vector<std::shared_ptr<Cudo>> cuda;
    
    struct Iterator{
        std::vector<std::shared_ptr<Cudo>>::const_iterator it;
        Cudo operator*() const {return **it;}
        Iterator operator++() {++it; return *this;}
        bool operator !=(const Iterator &drugi) const {return it!=drugi.it;}
    };
public:
    SvjetskaCuda() = default;
    ~SvjetskaCuda() = default;
    SvjetskaCuda(const SvjetskaCuda &sc){
        for(auto element : sc.cuda)
            cuda.push_back(std::make_shared<Cudo>(*element));
    }
    SvjetskaCuda(SvjetskaCuda &&sc) : cuda(sc.cuda){
        sc.cuda.clear();
    }
    SvjetskaCuda& operator=(SvjetskaCuda sc){
        std::swap(cuda, sc.cuda);
        return *this;
    }

    void Dodaj(std::string s, std::pair<std::string, std::string> par, int g){
        cuda.push_back(std::make_shared<Cudo>(s, par, g));
    }
    void Dodaj(const Cudo &c){
        cuda.push_back(std::make_shared<Cudo>(c));
    }
    void operator+=(const Cudo &c){
        Dodaj(c);
    }
    void Dodaj(const std::initializer_list<Cudo> &lista){
        for(auto elem : lista)
            cuda.push_back(std::make_shared<Cudo>(elem));
    }

    bool operator==(const SvjetskaCuda &sc) const{
        if(sc.cuda.size() != cuda.size()) return false;
        for(int i=0; i<cuda.size(); i++){
            if(cuda[i]->DajNaziv() != sc.cuda[i]->DajNaziv()
            || cuda[i]->DajGodinu() != sc.cuda[i]->DajGodinu()
            || cuda[i]->DajLokaciju() != sc.cuda[i]->DajLokaciju())
                return false;
        }
        return true;
    }

    void operator-=(std::string s){
        cuda.erase(std::find_if(cuda.begin(), cuda.end(), [s](std::shared_ptr<Cudo> pok){return pok->DajNaziv() == s;}));
    }

    SvjetskaCuda operator/(std::string drzava) const{
        SvjetskaCuda rez;
        for(auto cudo : cuda){
            if(cudo->DajLokaciju().first == drzava)
            rez.Dodaj(*cudo);
        }
        return rez;
    }
    
    int operator*() const {return cuda.size();}

    int& operator[](std::string naziv){
        for(auto c : cuda) 
            if(c->DajNaziv() == naziv)
                return c->Godina();

        throw std::domain_error("nije pronadjeno");
    }
    const int operator[] (std::string naziv) const{
        for(auto c : cuda) 
            if(c->DajNaziv() == naziv)
                return c->Godina();
    }

    auto begin() const {return Iterator{cuda.begin()};}
    auto end() const {return Iterator{cuda.end()};}
};

int main() {
    SvjetskaCuda unesco; // (1) - Oprez, ovo mora da radi i kad se implementira (7)
    Cudo c1("Mehina kafana", {"BiH", "Sarajevo, Sedrenik"}, 2016); // (2)
    Cudo c2("Ibrin golf dvojka", "Srbija", "Novi Pazar", 1843); // (3) - Druga sintaksa
    unesco.Dodaj("Piramide", {"BiH", "Visoko"}, 2005); // (4)
    unesco.Dodaj(c1); // (5);
    unesco += c2; // (6) - alternativna sintaksa za (5)
    unesco.Dodaj({{"Viseći vrtovi", {"Irak", "Babilon"}, -600},
    {"Keopsova piramida", {"Egipat", "Giza"}, -2550},
    {"Ajfelov toranj", {"Francuska", "Pariz"}, 1889}}); // (6) - Dodaje više odjednom
    SvjetskaCuda kopija(unesco); // (7) - Duboka kopija
    SvjetskaCuda kopija2(std::move(kopija)); // (8)
    if(kopija2 == unesco) std::cout << "Fakat su identicni!\n"; // (9)
    unesco -= "Mehina kafana"; // (10) - Briše čudo iz kolekcije
    kopija = kopija2; // (11)
    SvjetskaCuda bosanska_cuda;
    bosanska_cuda = unesco / "BiH"; // (12) - Izdvaja čuda koja su u BiH
    std::cout << "Broj bosanskih čuda: " << *bosanska_cuda << std::endl; // (13)
    std::cout << unesco["Ajfelov toranj"] << std::endl; // (14) - Daje godinu izgradnje
    unesco["Piramide"] = -10000; // (15) - Mijenja godinu izgradnje
    std::ofstream datoteka("CUDA.TXT");
    for(Cudo c : unesco) datoteka << c; // (16) - Format ispisa odaberite po volji 

    return 0;
}