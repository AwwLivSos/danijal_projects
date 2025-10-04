//TP 2023/2024: Zadaća 4, Zadatak 3
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <tuple>
#include <iomanip>
#include <string>

class Datum{
    int dan, mjesec, godina;
    bool provjeriDatum(int d, int m, int g) const;
public:
    Datum(int d, int m, int g) : dan(d), mjesec(m), godina(g){
        if(!provjeriDatum(d, m, g)) throw std::domain_error("Neispravan datum");
    }
    void Postavi(int d, int m, int g);
    std::tuple<int, int, int> Ocitaj() const {return {dan, mjesec, godina};}
    void Ispisi() const {std::cout<<dan<<"/"<<mjesec<<"/"<<godina<<std::endl;}

};

void Datum::Postavi(int d, int m, int g){
    if(!provjeriDatum(d, m, g)) throw std::domain_error("Neispravan datum");
    Datum::dan=d; Datum::mjesec=m; Datum::godina=g;
}

bool Datum::provjeriDatum(int d, int m, int g) const {
  int broj_dana[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (g % 4 == 0 && g % 100 != 0 || g % 400 == 0)
    broj_dana[1]++;
  return !(g < 1 || m < 1 || m > 12 || d < 1 || d > broj_dana[m - 1]);
}

class Vrijeme{
    int sati, minute;
    bool provjeriVrijeme(int h, int m) const;
public:
    Vrijeme(int h, int m) : sati(h), minute(m) {
        if (!provjeriVrijeme(h, m)) throw std::domain_error("Neispravno vrijeme");
    }
    void Postavi(int h, int m);
    std::pair<int, int> Ocitaj() const { return {sati, minute}; }
    void Ispisi() const {
        std::cout << std::setw(2) << std::setfill('0') << sati << ":" << std::setw(2) << std::setfill('0') << minute << std::endl << std::setfill(' ');
    }
};

void Vrijeme::Postavi(int h, int m) {
    if (!provjeriVrijeme(h, m)) throw std::domain_error("Neispravno vrijeme");
    Vrijeme::sati = h; Vrijeme::minute = m;
}

bool Vrijeme::provjeriVrijeme(int h, int m) const {
    return !(h < 0 || h > 23 || m < 0 || m > 59);
}

class Pregled{
    std::string ime_pacijenta;
    Datum datum_pregleda;
    Vrijeme vrijeme_pregleda;
public:                                                                                                     //pozivaju se konstruktori za datum i vrijeme koji imaju svoju provjeru ispravnosti
    Pregled(const std::string &pacijent, const Datum &dat, const Vrijeme &vr) : ime_pacijenta(pacijent), datum_pregleda(dat), vrijeme_pregleda(vr) {}
    Pregled(const std::string &pacijent, int d, int m, int g, int h, int min) : ime_pacijenta(pacijent), datum_pregleda(d, m, g), vrijeme_pregleda(h, min) {}

    Pregled &PromijeniPacijenta(std::string ime){
        ime_pacijenta=ime;
        return *this;
    }

    Pregled &PromijeniDatum(const Datum &dat){
        datum_pregleda=dat;
        return *this;
    }

    Pregled &PromijeniVrijeme(const Vrijeme &v) {
        vrijeme_pregleda=v;
        return *this;
    }

    void PomjeriDanUnaprijed();
    void PomjeriDanUnazad();

    std::string DajImePacijenta() const { return ime_pacijenta; }
    Datum DajDatumPregleda() const { return datum_pregleda; }
    Vrijeme DajVrijemePregleda() const { return vrijeme_pregleda; }

    static bool DolaziPrije(const Pregled &p1, const Pregled p2);
    void Ispisi() const;
};

void Pregled::PomjeriDanUnaprijed(){
    int d, g, m;
    std::tie(d, m, g) = datum_pregleda.Ocitaj();
    int broj_dana[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((g % 4 == 0 && g % 100 != 0) || g % 400 == 0) broj_dana[1]++;
    d++;
    if (d > broj_dana[m - 1]) {
        d = 1;
        m++;
        if (m > 12) {
            m = 1;
            g++;
        }
    }
    datum_pregleda.Postavi(d, m, g);
}

void Pregled::PomjeriDanUnazad(){
    int d, g, m;
    std::tie(d, m, g) = datum_pregleda.Ocitaj();                                               
    int broj_dana[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((g % 4 == 0 && g % 100 != 0) || g % 400 == 0) broj_dana[1]++;
    d--;
    if (d<1) {
        m--;
        if (m<1) {
            m = 12;
            g--;
        }
        d = broj_dana[m-1];
    }
    datum_pregleda.Postavi(d, m, g);
}

bool Pregled::DolaziPrije(const Pregled &p1, const Pregled p2){
    int d1, m1, g1, h1, min1, d2, m2, g2, h2, min2;
    std::tie(d1, m1, g1) = p1.DajDatumPregleda().Ocitaj();                                      //koristi tie da izvuces svaki komad individualno, pa ih onda slozi zajedno i uporedi
    std::tie(d2, m2, g2) = p2.DajDatumPregleda().Ocitaj();
    std::tie(h1, min1) = p1.DajVrijemePregleda().Ocitaj();
    std::tie(h2, min2) = p2.DajVrijemePregleda().Ocitaj();

    return std::make_tuple(g1, m1, d1, h1, min1) < std::make_tuple(g2, m2, d2, h2, min2);       //uređene n-torke se upoređuju leksikografksi
}

void Pregled::Ispisi() const{
    std::cout << std::left << std::setw(30) << ime_pacijenta << " ";
    std::cout<<std::right;
    datum_pregleda.Ispisi();
    std::cout << " ";
    vrijeme_pregleda.Ispisi();
    std::cout << std::endl;
}

class Pregledi{
    Pregled **niz_pregleda;
    int zakazani;
    int max_pregledi;
    void Dislociraj(){
        for(int i=0; i<zakazani; i++) delete niz_pregleda[i];
        delete[] niz_pregleda;
    }
public:
    explicit Pregledi(int n) : max_pregledi(n), niz_pregleda(new Pregled*[n]{}),  zakazani(0){}
    Pregledi(std::initializer_list<Pregled> lista);
    ~Pregledi(){Dislociraj();}
    Pregledi(const Pregledi &p);
    Pregledi(Pregledi &&p);
    Pregledi &operator =(Pregledi p);

    void RegistrirajPregled(const std::string &pacijent, const Datum &dat, const Vrijeme &v);
    void RegistrirajPregled(const std::string &pacijent, int d, int m, int g, int h, int min);
    void RegistrirajPregled(Pregled *p);

    int DajBrojPregleda() const {return zakazani;}
    int DajBrojPregledaNaDatum(const Datum &dat) const {
        return std::count_if(niz_pregleda, niz_pregleda+zakazani, [dat](const Pregled *p1){
            return p1->DajDatumPregleda().Ocitaj() == dat.Ocitaj();
        });
    }
    Pregled DajNajranijiPregled();
    const Pregled &DajNajranijiPregled() const;
    void IsprazniKolekciju();
    void ObrisiNajranijiPregled();
    void ObrisiPregledePacijenta(const std::string &s);

    void IspisiPregledeNaDatum(const Datum &dat) const;
    void IspisiSvePreglede() const;
};

Pregledi::Pregledi(std::initializer_list<Pregled> lista) : max_pregledi(lista.size()), zakazani(lista.size()), niz_pregleda(new Pregled *[lista.size()] {}){
    try{
        int i{};
        for(const auto exam : lista) niz_pregleda[i++] = new Pregled(exam);
    }
    catch(...){
        Dislociraj();
        throw;
    }
}

Pregledi::Pregledi(const Pregledi &p) : zakazani(p.zakazani), max_pregledi(p.max_pregledi), niz_pregleda(new Pregled *[p.max_pregledi]{}){
    try{
        for(int i=0; i<p.zakazani; i++) niz_pregleda[i] = new Pregled(*p.niz_pregleda[i]);
    }
    catch(...){
        Dislociraj(); 
        throw;
    }
}

Pregledi::Pregledi(Pregledi &&p) : zakazani(p.zakazani), max_pregledi(p.max_pregledi), niz_pregleda(p.niz_pregleda){
    p.zakazani=0;
    p.niz_pregleda=nullptr;
}

Pregledi &Pregledi::operator =(Pregledi p){
    std::swap(p.niz_pregleda, niz_pregleda);                    //OP kao i uvijek
    std::swap(p.zakazani, zakazani);
    std::swap(p.max_pregledi, max_pregledi);
    return *this;
}

void Pregledi::RegistrirajPregled(const std::string &pacijent, const Datum &dat, const Vrijeme &v){
    if(zakazani+1>max_pregledi) throw std::range_error("Dostignut maksimalni broj pregleda");
    niz_pregleda[zakazani++] = new Pregled(pacijent, dat, v);
}

void Pregledi::RegistrirajPregled(const std::string &pacijent, int d, int m, int g, int h, int min){
    if(zakazani+1>max_pregledi) throw std::range_error("Dostignut maksimalni broj pregleda");
    niz_pregleda[zakazani++] = new Pregled(pacijent, d, m, g, h, min);
}

void Pregledi::RegistrirajPregled(Pregled *p){
    if(zakazani+1>max_pregledi) throw std::range_error("Dostignut maksimalni broj pregleda");
    niz_pregleda[zakazani++] = p;
}

void Pregledi::IsprazniKolekciju(){
    Dislociraj();
    zakazani=0;
    niz_pregleda=new Pregled*[max_pregledi]{};
}

void Pregledi::ObrisiNajranijiPregled(){
    if(!zakazani) throw std::range_error("Prazna kolekcija");
    auto najraniji = std::min_element(niz_pregleda, niz_pregleda + zakazani, [](const Pregled *p1, const Pregled *p2) {             //ne ide metoda DajNajraniji jer mi ne vraca nikakvu referencu ili pokazivac
        if(p1 == nullptr) return false;
        if(p2 == nullptr) return true;
        return Pregled::DolaziPrije(*p1, *p2);
        });
    delete *najraniji;                                                                                                              //najraniji je tipa Pregled**
    std::move(najraniji + 1, niz_pregleda + zakazani, najraniji);                                                                   //move semantika dosta jaka za ovaj primjer
    zakazani--;
}

void Pregledi::ObrisiPregledePacijenta(const std::string &s){
    for(int i=0; i<zakazani;){
        if (niz_pregleda[i]->DajImePacijenta()==s) {
            delete niz_pregleda[i];
            std::move(niz_pregleda + i + 1, niz_pregleda + zakazani, niz_pregleda + i);                                             //nije tesko skontat sta radi al zasto je ovako efikasna, ko zna
            zakazani--;
        }
        else i++;
    }
}

void Pregledi::IspisiPregledeNaDatum(const Datum &dat) const{
    Pregled **pomocni = new Pregled*[zakazani];
    int pom_vel{};
    for(int i=0; i<zakazani; i++){
        if(niz_pregleda[i]->DajDatumPregleda().Ocitaj() == dat.Ocitaj())
            pomocni[pom_vel++] = new Pregled(*niz_pregleda[i]);
    }

    std::sort(pomocni, pomocni + pom_vel, [](const Pregled *p1, const Pregled *p2){
        return Pregled::DolaziPrije(*p1, *p2);
    });

    for(int i=0; i<pom_vel; i++) {
        pomocni[i]->Ispisi();
        delete pomocni[i];
    }
    delete[] pomocni;
}

void Pregledi::IspisiSvePreglede() const{
    Pregled **pomocni = new Pregled*[zakazani];
    for(int i=0; i<zakazani; i++) pomocni[i] = new Pregled(*niz_pregleda[i]);
    std::sort(pomocni, pomocni + zakazani, [](const Pregled *p1, const Pregled *p2){
        return Pregled::DolaziPrije(*p1, *p2);  //potencijalno ovdje dodati nullptr provjeru
    });

    for(int i=0; i<zakazani; i++)  {
        pomocni[i]->Ispisi();
        delete pomocni[i];
    }
    delete[] pomocni;
}

Pregled Pregledi::DajNajranijiPregled(){
    if(!zakazani) throw std::domain_error("Nema registriranih pregleda");
    return **std::min_element(niz_pregleda, niz_pregleda+zakazani, [](const Pregled *p1, const Pregled *p2){
        if(p1 == nullptr) return false;
        if(p2 == nullptr) return true;
        return Pregled::DolaziPrije(*p1, *p2);
    });
}
//ovo dvoje se mozda vjv moglo preko jedne funkcije
const Pregled &Pregledi::DajNajranijiPregled() const{                                                           //kako da ovo maksuz pozovem, bilo bi korisno gore
    if(!zakazani) throw std::domain_error("Nema registriranih pregleda");
    return **std::min_element(niz_pregleda, niz_pregleda+zakazani, [](const Pregled *p1, const Pregled *p2){
        if(p1 == nullptr) return false;
        if(p2 == nullptr) return true;
        return Pregled::DolaziPrije(*p1, *p2);
    });
}

int main ()
{
    std::cout<<"Unesite maksimalni broj pregleda: ";
    int maximalni;
    std::cin>>maximalni;
    Pregledi p(maximalni);

    for(;;){
        std::cout<<std::endl<<"1. Registriraj Pregled"<<
        std::endl<<"2. Daj Broj Pregleda"<<
        std::endl<<"3. Daj Broj Pregleda Na Datum"<<
        std::endl<<"4. Daj Najraniji Pregled"<<
        std::endl<<"5. Obrisi Najraniji Pregled"<<
        std::endl<<"6. Obrisi Preglede Pacijenta"<<
        std::endl<<"7. Ispisi Preglede Na Datum"<<
        std::endl<<"8. Ispisi Sve Preglede"<<
        std::endl<<"9. Isprazni Kolekciju"<<
        std::endl<<"10. Izlaz"<<std::endl;
        int opcija;
        std::cout << "Unesite izbor: ";
        std::cin>>opcija;
        switch(opcija){
            case 1:
                try{
                    std::cin.clear();
                    std::cin.ignore(99999, '\n');
                    std::string ime;
                    int d, m, g, h, min;
                    std::cout << "Unesite ime pacijenta: ";
                    std::getline(std::cin, ime);
                    std::cout << "Unesite datum i vrijeme pregleda (dan mjesec godina sati minute): ";
                    std::cin>>d>>m>>g>>h>>min;
                    p.RegistrirajPregled(ime, d, m, g, h, min);
                }
                catch(std::range_error &e) {
                    std::cout << e.what() << std::endl;
                }
                break;
            case 2:
                std::cout<<"Ukupan broj pregleda: "<<p.DajBrojPregleda()<<std::endl;
                break;
            case 3:
                {std::cout << "Unesite datum (dan mjesec godina): ";
                int d, m, g;
                std::cin.clear();
                std::cin.ignore(99999, '\n');
                std::cin>>d>>m>>g;
                Datum dat(d, m, g);
                std::cout << "Broj pregleda na datum: " << p.DajBrojPregledaNaDatum(dat) << std::endl;
                break;}
            case 4:
                try{
                    p.DajNajranijiPregled().Ispisi();
                }catch(std::domain_error &e){
                    std::cout<<e.what() << std::endl;
                }
                break;
            case 5:
                try{
                    p.ObrisiNajranijiPregled();
                    std::cout << "Najraniji pregled je obrisan." << std::endl;
                }
                catch(std::range_error &e){
                    std::cout << e.what() << std::endl;
                }
                break;
            case 6:
                {std::cin.clear();
                std::cin.ignore(99999, '\n');
                std::cout<<"Unesite ime pacijenta: ";
                std::string ime;
                std::getline(std::cin, ime);
                p.ObrisiPregledePacijenta(ime);
                std::cout << "Pregledi za pacijenta " << ime << " su obrisani." << std::endl;
                break;}
            case 7:
                {std::cout << "Unesite datum: ";
                int d, m, g;
                std::cin.clear();
                std::cin.ignore(99999, '\n');
                std::cin>>d>>m>>g;
                Datum dat(d, m, g);
                p.IspisiPregledeNaDatum(dat);
                break;}
            case 8:
                std::cout << "Svi pregledi: \n";
                p.IspisiSvePreglede();
                break;
            case 9:
                p.IsprazniKolekciju();
                std::cout << "Kolekcija ispraznjena." << std::endl;
                break;
            default:
                std::cout << "Kraj programa.";
                return 0;
        }
    }
}
