#include <iostream>
#include <stdexcept>

/*Konstruktor klase kreira datum u skladu sa informacijama koje se prosljeđuju kroz parametre (uz obaveznu kontrolu ispravnosti datuma) i baca izuzetak u slučaju da su proslijeđeni pogrešni podaci.
Podržana su dva načina kako se može zadati mjesec: kao vrijednost tipa Mjeseci ili kao cijeli broj u opsegu od 1–12. Metode Postavi načelno obavljaju istu funkciju kao i konstruktori.
Metode DajDan, DajMjesec i DajGodinu služe za očitavanje informacija o danu, mjesecu i godini koje su pohranjene u datumu.
Metoda DajImeMjeseca vraća ime mjeseca (pokazivač na prvi znak imena).
Metoda DajBrojDanaUMjesecu vraća ukupan broj dana u mjesecu koji se čuva u datumu.
DaLiJePrestupna vraća informaciju da li je godina prestupna.
DajDanUGodini vraća redni broj dana u godini (npr. 3/7/2002. je 184-ti dan).
DajDanUSedmici vraća dan u sedmici tipa Dani, dok DajImeDanaUSedmici vraća ime tog dana.
IdiNaSljedeciDan i IdiNaPrethodniDan pomjeraju datum za jedan dan unaprijed ili unazad. PomjeriZa pomjera za proizvoljan broj dana.
Unesi omogućava unos datuma sa tastature (format: dan/mjesec/godina). U slučaju greške baca se izuzetak i ulazni tok se vraća u ispravno stanje.
Ispisi ispisuje datum u formatu: 3. Juli 2002. (Srijeda).
Funkcija BrojDanaIzmedju vraća broj dana između dva datuma.*/

enum Mjeseci { Januar = 1, Februar, Mart, April, Maj, Juni, Juli, August, Septembar, Oktobar, Novembar, Decembar };
enum Dani { Ponedjeljak = 1, Utorak, Srijeda, Cetvrtak, Petak, Subota, Nedjelja };

class Datum{
    int dan, mjesec, godina;
public:
    Datum(int dan, Mjeseci mjesec, int godina) {Postavi(dan, mjesec, godina);}
    Datum(int dan, int mjesec, int godina) {Postavi(dan, mjesec, godina);}
    void Postavi(int dan, Mjeseci mjesec, int godina);
    void Postavi(int dan, int mjesec, int godina);
    int DajDan() const {return dan;}
    Mjeseci DajMjesec() const {return Mjeseci(mjesec);}
    const char *DajImeMjeseca() const;
    int DajBrojDanaUMjesecu() const;
    int DajGodinu() const  {return godina;}
    bool DaLiJePrestupna() const {return godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0;}
    int DajDanUGodini() const;
    Dani DajDanUSedmici() const {return Dani(DajDanUGodini()%7 + 1);}
    const char *DajImeDanaUSedmici() const;
    void IdiNaSljedeciDan() {PomjeriZa(1);}
    void IdiNaPrethodniDan() {PomjeriZa(-1);}
    void PomjeriZa(int broj_dana);
    void Unesi();
    void Ispisi() const {std::cout<<dan<<". "<<DajImeMjeseca()<<" "<<godina<<". ("<<DajDanUSedmici()<<")";}
    friend int BrojDanaIzmedju(const Datum &d1, const Datum &d2);  
};

int BrojDanaIzmedju(const Datum &d1, const Datum &d2){
    int dani1 = d1.DajDanUGodini() + (d1.DajGodinu() - 1) * 365 + (d1.DajGodinu() - 1) / 4 - (d1.DajGodinu() - 1) / 100 + (d1.DajGodinu() - 1) / 400;
    int dani2 = d2.DajDanUGodini() + (d2.DajGodinu() - 1) * 365 + (d2.DajGodinu() - 1) / 4 - (d2.DajGodinu() - 1) / 100 + (d2.DajGodinu() - 1) / 400;

    return std::abs(dani1 - dani2);
}

void Datum::Postavi(int dan, int mjesec, int godina) {
    int broj_dana[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0) broj_dana[1]++;

    if(godina < 1 || dan < 1 || mjesec < 1 || mjesec > 12 || dan > broj_dana[mjesec - 1])
        throw std::domain_error("Neispravan datum!");

    this->dan = dan; this->mjesec = mjesec; this->godina = godina;
}

void Datum::Postavi(int dan, Mjeseci mjesec, int godina) {
    int broj_dana[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0) broj_dana[1]++;

    if(godina < 1 || dan < 1 || mjesec < 1 || mjesec > 12 || dan > broj_dana[mjesec - 1])
        throw std::domain_error("Neispravan datum!");

    this->dan = dan; this->mjesec = mjesec; this->godina = godina;
}

const char *Datum::DajImeMjeseca() const{
    char* imena[]{"Januar", "Februar", "Mart", "April", "Maj", "Juni", "Juli", "August", "Septembar", "Oktobar", "Novembar", "Decembar"};
    return imena[mjesec-1];
}

int Datum::DajBrojDanaUMjesecu() const{
    int broj_dana[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0) broj_dana[1]++;
    return broj_dana[mjesec-1];
}

int Datum::DajDanUGodini() const{
    int brojDana{};
    int broj_dana[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0) broj_dana[1]++;

    for(int i=0; i<mjesec-1; i++) brojDana += broj_dana[i];
    brojDana+=dan;

    return brojDana;
}

const char *Datum::DajImeDanaUSedmici() const{
    char *imena[]{ "Ponedjeljak", "Utorak", "Srijeda", "Cetvrtak", "Petak", "Subota", "Nedjelja" };
    return imena[DajDanUSedmici()-1];
}

void Datum::PomjeriZa(int pomak){
    int broj_dana[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(godina % 4 == 0 && godina % 100 != 0 || godina % 400 == 0) broj_dana[1]++;

    dan += pomak;

    while(dan < 1 || dan > broj_dana[mjesec - 1]) {
        if(dan < 1) {
            mjesec--;
            if(mjesec < 1) {
                mjesec = Decembar;
                godina--;
            }
            dan += broj_dana[mjesec - 1];
        } else if(dan > broj_dana[mjesec - 1]) {
            dan -= broj_dana[mjesec - 1];
            mjesec++;
            if(mjesec > Decembar) {
                mjesec = Januar;
                godina++;
            }
        }
    }
}

void Datum::Unesi(){
    char kosa;
    std::cin>>dan>>kosa>>mjesec>>kosa>>godina;
    if(kosa != '/') throw std::domain_error("Neispravan unos!");
}

class Student{
    int **ocjene;
    int brojOcjena;
    int maxOcjene;
public:
    Student() : ocjene(nullptr), brojOcjena(0), maxOcjene(10) {}
    Student(int max) : maxOcjene(max), brojOcjena(0), ocjene(new int*[max]{}) {}
    Student(const Student &drugi) : brojOcjena(drugi.brojOcjena), maxOcjene(drugi.maxOcjene), ocjene(new int*[drugi.maxOcjene]{}) {
        for(int i=0; i<brojOcjena; i++){
            ocjene[i] = new int(*drugi.ocjene[i]);
        }
    }
    Student(Student &&drugi) : brojOcjena(drugi.brojOcjena), maxOcjene(drugi.maxOcjene), ocjene(drugi.ocjene){
        drugi.brojOcjena=0;
        drugi.maxOcjene=0;
        drugi.ocjene = nullptr;
    }
    
    ~Student(){
        for(int i=0; i<brojOcjena; i++){
            delete ocjene[i];
            ocjene[i] = nullptr;
        }
        delete ocjene;
        ocjene = nullptr;
    }
};

int main(){
    
    Student s;

    return 0;
}