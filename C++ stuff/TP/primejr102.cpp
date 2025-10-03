/*Obje razvijene klase u zadacima 1 i 2 rade dobro, ali su pomalo “staromodne”, jer ne koriste modernije
koncepte jezika C++. Zbog toga je odlučeno da se klasa “Imenik” iz zadatka 1 modernizira. Prva izmjena
je što će, radi bolje enkapsulacije, “Uposlenik” sada biti klasa a ne struktura, pri čemu će njeni atributi
koji čuvaju ime i prezime odnosno adresu sada biti dinamički stringovi (tj. tipa “string”) a ne obični
nizovi znakova. Pored ovih atributa, klasa “Uposlenik” će sadržavati konstruktor koji inicijalizira sve
atribute u skladu sa parametrima konstruktora, te trivijalne pristupne metode “DajIme”, “DajAdresu” i
“DajTelefon” pomoću kojih se mogu saznati vrijednosti odgovarajućih atributa.
*/

/*Druga izmjena je što će se podaci o uposlenicima umjesto u dinamički alociranom nizu objekata tipa
“Uposlenik” čuvati u vektoru objekata tipa “Uposlenik”. Ovim otpada potreba da se putem konstruktora
zadaje maksimalan broj uposlenika koji se mogu registrirati. Također je odlučeno da metode “Sacuvaj” i
“Obnovi” koriste tekstualne datoteke a ne binarne, pri čemu možete sami odlučiti kako će izgledati
format zapisa u datoteci (prelaskom na tekstualne umjesto binarnih datoteka ujedno rješavamo i
problem uzrokovan činjenicom da vektori i stringovi nisu POD tipovi podataka, a opće je poznato da
ne-POD tipovi podataka i binarne datoteke baš i nisu “u ljubavi”). Napišite ponovo klasu “Imenik” u
skladu sa ovako izmijenjenom organizacijom, ali tako da interfejs ostane isti, odnosno da korisnik klase
ne primijeti da je došlo do promjene u internoj organizaciji klase u odnosu na klasu iz zadatka 1 (osim
izmjene koja je uzrokovana gubitkom parametra konstruktora).
*/

/*Mada i prethodna klasa također radi izvrsno, u šta ne treba ni sumnjati, ona posjeduje isti problem po
pitanju korištenja računarskih resursa kao i klasa iz zadatka 1. Naime, bolje bi bilo da se u vektoru ne
čuvaju sami objekti tipa “Uposlenik” nego pokazivači na takve objekte, pri čemu se individualni objekti
tipa “Uposlenik” alociraju dinamički. Napišite ponovo klasu “Imenik” u skladu sa ovako izmijenjenom
organizacijom, ali tako da interfejs ostane isti kao u prethodnom zadatku, odnosno da korisnik klase ne
primijeti da je došlo do promjene u internoj organizaciji u odnosu na klasu iz prethodnog zadatka (a ni
u odnosu na klasu iz zadatka 2 osim izmjene koja je uzrokovana gubitkom parametra konstruktora).
*/

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <memory>

class Uposlenik{
    std::string imeprezime, adresa;
    int telbroj;
public:
    Uposlenik(std::string ime, std::string adresa, int broj) : imeprezime(ime), adresa(adresa), telbroj(broj) {}
    std::string DajIme() const {return imeprezime;}
    std::string DajAdresu() const {return adresa;}
    int DajBroj() const {return telbroj;}
    int &DajBroj() {return telbroj;}
};

class Imenik{
    std::vector<std::shared_ptr<Uposlenik>> uposlenici;

public:
    Imenik() = default;

    Imenik(const Imenik &drugi){
        for(auto up : drugi.uposlenici) 
            uposlenici.push_back(std::make_shared<Uposlenik>(*up));
    }

    Imenik(Imenik &&drugi) : uposlenici(drugi.uposlenici){
        drugi.uposlenici.clear();
    }

    Imenik &operator=(Imenik drugi){
        std::swap(uposlenici, drugi.uposlenici);
        return *this;
    }

    void DodajUposlenika(const std::string &ime, const std::string &adresa, int broj){
        uposlenici.push_back(std::make_shared<Uposlenik>(Uposlenik{ime, adresa, broj}));
    }

    void DodajUposlenika(const Uposlenik &u){
        uposlenici.push_back(std::make_shared<Uposlenik>(u));
    }

    void DodajUposlenika(Uposlenik *u){
        uposlenici.push_back(std::make_shared<Uposlenik>(*u));
    }

    void IspisiUposlenika(std::string ime) const{
        for(auto up : uposlenici) 
            if(up->DajIme() == ime) std::cout<<up->DajIme()<<", "<<up->DajAdresu()<<", "<<up->DajBroj();
    }

    void IspisiSveNaSlovo(char c) const{
        for(auto up : uposlenici){
            if(up->DajIme()[0] == c) std::cout<<up->DajIme()<<", "<<up->DajAdresu()<<", "<<up->DajBroj();
        }
    }

    void IspisiImenik() const{
        std::vector<Uposlenik> kopija;

        for(auto up : uposlenici) kopija.push_back(*up);

        std::sort(kopija.begin(), kopija.end(), [](const Uposlenik &u1, const Uposlenik &u2){
            return u1.DajIme() < u2.DajIme();
        });

        for(auto &up : kopija) std::cout<<up.DajIme()<<", "<<up.DajAdresu()<<", "<<up.DajBroj() << std::endl;
    }

    int &operator[](std::string ime){
        for(auto up : uposlenici)
            if(up->DajIme() == ime)
                return up->DajBroj();
    }

    const int operator[](std::string ime) const{
        for(auto up : uposlenici)
            if(up->DajIme() == ime)
                return up->DajBroj();
    }

    bool operator!() {return uposlenici.empty();}

    friend Imenik operator+(const Imenik &prvi, const Imenik &drugi){
        Imenik novi;
        novi.uposlenici = prvi.uposlenici;
        for(auto up : drugi.uposlenici) novi.DodajUposlenika(*up);
        return novi;
    }

    void Sacuvaj(std::string ime){
        std::ofstream dat(ime);
        for(auto up : uposlenici) 
            dat << up->DajIme() << "|" << up->DajAdresu() << "|" << up->DajBroj() <<std::endl;
    }

    void Obnovi(std::string ime){
        std::ifstream dat(ime);
        if(!dat) throw std::domain_error("Nepostojeca datoteka");

        uposlenici.clear();

        std::string novoIme, novaAdresa;
        int noviBroj;

        while(std::getline(dat, novoIme, '|') && std::getline(dat, novaAdresa, '|') && dat>>noviBroj){
            dat.ignore(99999, '\n');
            uposlenici.push_back(std::make_shared<Uposlenik>(novoIme, novaAdresa, noviBroj));
        }

        if(!dat.eof()) throw std::domain_error("Problemi pri citanju datoteke");
    }

};

int main(){

    Uposlenik a("aaaa", "aaaaaaa", 5), b("bbbbb", "bbb bbbb bb", 10), c("ccc", "ccc c c c cc c c", 15);
    
    Imenik im;

    im.DodajUposlenika(a);
    im.DodajUposlenika(c);
    im.DodajUposlenika(b);

    im.IspisiImenik();


    return 0;
}