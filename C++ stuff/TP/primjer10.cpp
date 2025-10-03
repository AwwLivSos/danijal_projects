/*Elektrotehničkom fakultetu je potreban interni telefonski imenik svojih uposlenika. Stoga je potrebno
razviti kontejnersku klasu “Imenik” koja će čuvati kolekciju podataka o uposlenicima i njihovim
(lokalnim) telefonskim brojevima. Prva ideja je bila da se podaci o jednom uposleniku čuvaju u
strukturi “Uposlenik” koja kao svoje atribute sadrži ime uposlenika zajedno sa prezimenom (tipa niza
od max. 20 znakova), adresu (tipa niza od max. 50 znakova) i lokalni broj telefona (cjelobrojnog tipa).
Dalje je planirano da se podaci o svim uposlenicima čuvaju u dinamički alociranom nizu čiji su elementi
gore opisane strukture (kojem se pristupa putem odgovarajućeg atrubuta klase).*/

/*Za kreiranje primjeraka klase, predviđeno je postojanje konstruktora sa jednim parametrom koji vrši
neophodnu dinamičku alokaciju, pri čemu parametar predstavlja maksimalni broj uposlenika koji se
mogu evidentirati. Ovaj konstruktor se ne smije koristiti za automatsku konverziju cjelobrojnih
podataka u tip ove klase. Predviđen je i destruktor koji se brine o oslobađanju svih resursa koje su
primjerci ove klase zauzeli tokom svog života. Radi bezbjednog kopiranja i međusobnog dodjeljivanja
primjeraka ove klase, planirano je da se podrže i konstruktor kopije te preklopljeni operator dodjele,
koji će se zasnivati na strategiji dubokog kopiranja.*/

/*Za dodavanje novih podataka u imenik predviđene su tri metode istog imena “DodajUposlenika”. Prva
metoda kao parametre prima podatke o uposleniku (ime i prezime, adresa i broj telefona). U slučaju da
su podaci o imenu i prezimenu ili adresi predugi, baca se izuzetak. Druga metoda kao jedini parametar
ima konstantnu referencu na neki objekat tipa “Uposlenik” koja sadrži sve podatke o uposleniku koji se
upisuje, dok treća metoda umjesto konstantne reference prima pokazivač na odgovarajući objekat tipa
“Uposlenik” (u ovom trećem slučaju, klasa “Imenik” ne preuzima vlasništvo nad objektom, tj. nije
odgovorna za njegovo eventualno brisanje). Sve tri metode bacaju izuzetak ukoliko se dostigne
maksimalni broj uposlenika koji se mogu evidentirati.*/

/*
Za pretragu imenika predviđene su metoda “IspisiUposlenika” koja ispisuje podatke o uposleniku čije
se ime i prezime zadaje kao parametar, metoda “IspisiSveNaSlovo” koja ispisuje podatke o svim
uposlenicima čije ime počinje slovom koje se zadaje kao parametar, te metoda “IspisiImenik” koja
ispisuje čitav telefonski imenik sortiran po abecednom poretku. Predviđen je i preklopljeni operator
“[]” koji vraća referencu na broj telefona uposlenika čije se ime zadaje unutar uglastih zagrada
(referenca se vraća sa ciljem da se omogući izmjena broja). U slučaju da se ovaj operator primijeni na
konstantni objekat, umjesto reference treba vratiti kopiju broja.
*/

/*
Klasa “Imenik” također treba da podržava unarni operator “!” koji vraća logičku vrijednost “true”
ukoliko je imenik prazan, a “false” u suprotnom, te binarni operator “+” koji sastavlja dva imenika u
jedan. Preciznije, ukoliko su “X” i “Y” dva imenika (tj. objekta tipa “Imenik”), “X + Y” treba da bude novi
imenik koji u sebi sadrži sve podatke kako iz imenika “X”, tako i iz imenika “Y”.
*/

/*
Konačno, za trajno čuvanje podataka pohranjenih u imeniku predviđene su i metode “Sacuvaj” i
“Obnovi” koje snimaju čitav telefonski imenik u binarnu datoteku čije se ime zadaje kao parametar,
odnosno obnavljaju njegov sadržaj iz binarne datoteke čije se ime zadaje kao parametar.*/

/*Mada klasa koju ste razvili u prethodnom zadatku radi jako lijepo i ispunjava postavljene zahtjeve,
ispostavilo se da ona neracionalno koristi računarske resurse, te da bi bolje bilo u dinamički
alociranom nizu čuvati ne same objekte tipa “Uposlenik” nego pokazivače na takve objekte, pri čemu bi
se individualni objekti tipa “Uposlenik” alocirali dinamički. Napišite ponovo klasu “Imenik” u skladu sa 
ovako izmijenjenom organizacijom, ali tako da interfejs ostane isti, odnosno da korisnik klase ne
primijeti da je došlo do promjene u internoj organizaciji klase.*/

#include <utility>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <cstring>
#include <memory>

struct Uposlenik{
    char* imeprezime;
    char* adresa;
    int telbroj;
};

class Imenik{
    Uposlenik** uposlenici;
    int kapacitet;
    int brojUposlenika;

    void Dealociraj(){
        for(int i=0; i<brojUposlenika; i++){
            delete[] uposlenici[i]->imeprezime;
            uposlenici[i]->imeprezime = nullptr;

            delete[] uposlenici[i]->adresa;
            uposlenici[i]->adresa = nullptr;

            delete uposlenici[i];
            uposlenici[i] = nullptr;
        }
        delete[] uposlenici;
        uposlenici = nullptr;
    }

    void Kopiraj(const Imenik &drugi){
        for(int i=0; i<brojUposlenika; i++) {
            uposlenici[i] = new Uposlenik{nullptr, nullptr, drugi.uposlenici[i]->telbroj};

            uposlenici[i]->imeprezime = new char[std::strlen(drugi.uposlenici[i]->imeprezime) + 1];
            uposlenici[i]->adresa = new char[std::strlen(drugi.uposlenici[i]->adresa) + 1];

            std::strcpy(uposlenici[i]->imeprezime, drugi.uposlenici[i]->imeprezime);
            std::strcpy(uposlenici[i]->adresa, drugi.uposlenici[i]->adresa);
        }
    }

public:
    explicit Imenik(int max) : kapacitet(max), brojUposlenika(0), uposlenici(new Uposlenik*[max]{}) {}

    ~Imenik() {
        Dealociraj();
    }
    
    Imenik(const Imenik &drugi) : kapacitet(drugi.kapacitet), brojUposlenika(drugi.brojUposlenika), uposlenici(new Uposlenik*[drugi.kapacitet]{}){
        Kopiraj(drugi);
    }

    Imenik(Imenik &&drugi) : kapacitet(drugi.kapacitet), brojUposlenika(drugi.brojUposlenika), uposlenici(drugi.uposlenici){
        drugi.uposlenici = nullptr;
        drugi.brojUposlenika=0;
    }

    Imenik &operator=(const Imenik &drugi){
        if(this != &drugi){
            Dealociraj();
            Kopiraj(drugi);
        }
        return *this;
    }

    Imenik &operator=(Imenik &&drugi){
        if(this != &drugi){
            std::swap(brojUposlenika, drugi.brojUposlenika);
            std::swap(kapacitet, drugi.kapacitet);
            std::swap(uposlenici, drugi.uposlenici);
        }
        return *this;
    }

    void DodajUposlenika(const char *ime, const char* adresa, int broj) {
        if(brojUposlenika == kapacitet) throw std::range_error("Dostignut kapacitet imenika");
        if(std::strlen(ime) >= 20 || std::strlen(adresa) >= 50) throw std::domain_error("Ime ili adresa predugi!");

        uposlenici[brojUposlenika] = new Uposlenik;

        uposlenici[brojUposlenika]->imeprezime = new char[std::strlen(ime) + 1];
        std::strcpy(uposlenici[brojUposlenika]->imeprezime, ime);

        uposlenici[brojUposlenika]->adresa = new char[std::strlen(adresa) + 1];
        std::strcpy(uposlenici[brojUposlenika]->adresa, adresa);

        uposlenici[brojUposlenika]->telbroj = broj;

        brojUposlenika++;
    }

    void DodajUposlenika(const Uposlenik &u) {
        DodajUposlenika(u.imeprezime, u.adresa, u.telbroj);
    }

    void DodajUposlenika(Uposlenik *u, bool mojaBriga=false){
        if(!mojaBriga)
            DodajUposlenika(u->imeprezime, u->adresa, u->telbroj);

        else {
            uposlenici[brojUposlenika] = u;
            brojUposlenika++;
        }
    }

    void IspisiUposlenika(const char *ime) const{
        for(int i=0; i<brojUposlenika; i++){
            if(std::strcmp(ime, uposlenici[i]->imeprezime) == 0){
                std::cout<<uposlenici[i]->imeprezime<<", "<<uposlenici[i]->adresa<<", "<<uposlenici[i]->telbroj;
                return;
            }
        }
        std::cout<<"Uposlenik nije pronađen";
    }

    void IspisiSveNaSlovo(char c) const{
        for(int i=0; i<brojUposlenika; i++){
            if(c == uposlenici[i]->imeprezime[0]){
                //IspisiUposlenika(uposlenici[i].imeprezime);

                std::cout<<uposlenici[i]->imeprezime<<", "<<uposlenici[i]->adresa<<", "<<uposlenici[i]->telbroj;
            }
        }
    }

    void IspisiImenik() const{
        Uposlenik** kopija = new Uposlenik*[brojUposlenika];
        for(int i=0; i<brojUposlenika; i++) 
            kopija[i] = new Uposlenik(*uposlenici[i]);

        std::sort(kopija, kopija+brojUposlenika, [](const Uposlenik *u1, const Uposlenik *u2){
            return std::strcmp(u1->imeprezime, u2->imeprezime) < 0;
        });

        for(int i=0; i<brojUposlenika; i++){
            std::cout<<kopija[i]->imeprezime<<", "<<kopija[i]->adresa<<", "<<kopija[i]->telbroj<<std::endl;
        }

        for(int i=0; i<brojUposlenika; i++) delete kopija[i];
        delete[] kopija;
    }

    int &operator[](const char *ime){
        for(int i=0; i<brojUposlenika; i++){
            if(std::strcmp(ime, uposlenici[i]->imeprezime) == 0)
                return uposlenici[i]->telbroj;
        }
    }

    const int operator[](const char *ime) const{
        for(int i=0; i<brojUposlenika; i++){
            if(std::strcmp(ime, uposlenici[i]->imeprezime) == 0)
                return uposlenici[i]->telbroj;
        }
    }

    bool operator!() {return brojUposlenika == 0;}

    friend Imenik operator+(const Imenik &prvi, const Imenik &drugi){
        Imenik novi(prvi.kapacitet + drugi.kapacitet);

        for(int i=0; i<prvi.brojUposlenika; i++) novi.DodajUposlenika(prvi.uposlenici[i]);
        for(int i=0; i<drugi.brojUposlenika; i++) novi.DodajUposlenika(drugi.uposlenici[i]);

        return novi;
    }

    void Sacuvaj(std::string ime){
        std::ofstream dat(ime, std::ios::binary);
        dat.write(reinterpret_cast<char*>(&brojUposlenika), sizeof(brojUposlenika));

        for(int i=0; i<brojUposlenika; i++){
            dat.write(reinterpret_cast<char*>(uposlenici[i]->imeprezime), sizeof(uposlenici[i]->imeprezime));
            dat.write(reinterpret_cast<char*>(uposlenici[i]->adresa), sizeof(uposlenici[i]->adresa));
            dat.write(reinterpret_cast<char*>(&uposlenici[i]->telbroj), sizeof(int));
        }
    }

    void Obnovi(std::string ime){
        std::ifstream dat (ime, std::ios::binary);
        if(!dat) throw std::domain_error("Datoteka ne postoji!");

        int noviBroj;
        dat.read(reinterpret_cast<char*>(&noviBroj), sizeof(noviBroj));
        if(noviBroj >= kapacitet) throw std::domain_error("Nesaglasne velicine imenika");

        Dealociraj();

        uposlenici = new Uposlenik*[kapacitet]{};

        brojUposlenika = noviBroj;
        for(int i=0; i<brojUposlenika; i++){
            uposlenici[i] = new Uposlenik;
            uposlenici[i]->imeprezime = new char[20];
            uposlenici[i]->adresa = new char[50];

            dat.read(reinterpret_cast<char*>(uposlenici[i]->imeprezime), sizeof(uposlenici[i]->imeprezime));
            dat.read(reinterpret_cast<char*>(uposlenici[i]->adresa), sizeof(uposlenici[i]->adresa));
            dat.read(reinterpret_cast<char*>(&uposlenici[i]->telbroj), sizeof(int));
        }
    }
};

int main(){

    Imenik im2(10);
    im2.Obnovi("IMENIK2.BIN");
    im2.IspisiImenik();

    return 0;
}