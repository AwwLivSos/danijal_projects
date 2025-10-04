//TP 2023/2024: Zadaća 4, Zadatak 3
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <tuple>
#include <iomanip>
#include <vector>
#include <memory>
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
    void Ispisi() const {std::cout<<dan<<"/"<<mjesec<<"/"<<godina;}

};

void Datum::Postavi(int d, int m, int g){
    if(!provjeriDatum(d, m, g)) throw std::domain_error("Neispravan datum");
    Datum::dan=d; Datum::mjesec=m; Datum::godina=g;
}

bool Datum::provjeriDatum(int d, int m, int g) const {
  int broj_dana[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if ((g % 4 == 0 && g % 100 != 0) || g % 400 == 0)
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
    void Ispisi() const {std::cout << std::setw(2) << std::setfill('0') << sati << ":" << std::setw(2) << std::setfill('0') << minute << std::setfill(' ');}
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
public:
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
    std::cout << std::left << std::setw(30) << ime_pacijenta;
    std::cout<<std::right;
    datum_pregleda.Ispisi();
    std::cout << " ";
    vrijeme_pregleda.Ispisi();
    std::cout << std::endl;
}

class Pregledi{
    std::vector<std::shared_ptr<Pregled>> niz_pregleda;
public:
    explicit Pregledi() = default;
    Pregledi(std::initializer_list<Pregled> lista){
        for(const auto &p : lista) niz_pregleda.push_back(std::make_shared<Pregled>(p));
    }
    Pregledi(const Pregledi &p) : niz_pregleda(p.niz_pregleda){}
    Pregledi(Pregledi &&p) = default;
    Pregledi &operator =(Pregledi p){
        std::swap(p.niz_pregleda, niz_pregleda);
        return *this;
    }

    void RegistrirajPregled(const std::string &pacijent, const Datum &dat, const Vrijeme &v){
        niz_pregleda.push_back(std::make_shared<Pregled>(pacijent, dat, v));
    }
    void RegistrirajPregled(const std::string &pacijent, int d, int m, int g, int h, int min){
        niz_pregleda.push_back(std::make_shared<Pregled>(pacijent, d, m, g, h, min));
    }
    void RegistrirajPregled(std::shared_ptr<Pregled> p){
        niz_pregleda.push_back(p);
    }

    int DajBrojPregleda() const {return niz_pregleda.size();}
    int DajBrojPregledaNaDatum(const Datum &dat) const {
        return std::count_if(niz_pregleda.begin(), niz_pregleda.end(), [dat](std::shared_ptr<Pregled> p){
            return p->DajDatumPregleda().Ocitaj() == dat.Ocitaj();
        });
    }
    Pregled DajNajranijiPregled();
    Pregled &DajNajranijiPregled() const;
    void IsprazniKolekciju(){niz_pregleda.clear();}
    void ObrisiNajranijiPregled();
    void ObrisiPregledePacijenta(const std::string &s);

    void IspisiPregledeNaDatum(const Datum &dat) const;
    void IspisiSvePreglede() const;
};

void Pregledi::ObrisiNajranijiPregled(){
    if(!niz_pregleda.size()) throw std::range_error("Prazna kolekcija");
    auto najraniji = std::min_element(niz_pregleda.begin(), niz_pregleda.end(), [](std::shared_ptr<Pregled> p1, std::shared_ptr<Pregled> p2) {
        if(p1 == nullptr) return false;
        if(p2 == nullptr) return true;
        return Pregled::DolaziPrije(*p1, *p2);
    });
    niz_pregleda.erase(najraniji);
}

void Pregledi::ObrisiPregledePacijenta(const std::string &s){
    niz_pregleda.erase(std::remove_if(niz_pregleda.begin(), niz_pregleda.end(), [s](std::shared_ptr<Pregled> p) {
        return p->DajImePacijenta() == s;
    }), niz_pregleda.end());
}

void Pregledi::IspisiPregledeNaDatum(const Datum &dat) const{
    std::vector<std::shared_ptr<Pregled>> pomocni;
    for(int i=0; i<niz_pregleda.size(); i++){
        if(niz_pregleda.at(i)->DajDatumPregleda().Ocitaj() == dat.Ocitaj())
            pomocni.push_back((niz_pregleda.at(i)));
    }
    std::sort(pomocni.begin(), pomocni.end(), [](std::shared_ptr<Pregled> p1, std::shared_ptr<Pregled> p2){
        return Pregled::DolaziPrije(*p1, *p2);
    });

    for(int i=0; i<pomocni.size(); i++) pomocni[i]->Ispisi();
}

void Pregledi::IspisiSvePreglede() const{
    auto pomocni = niz_pregleda;
    std::sort(pomocni.begin(), pomocni.end(), [](std::shared_ptr<Pregled> p1, std::shared_ptr<Pregled> p2){
        return Pregled::DolaziPrije(*p1, *p2);
    });

    for(int i=0; i<pomocni.size(); i++) pomocni[i]->Ispisi();
}

Pregled Pregledi::DajNajranijiPregled(){
    if(niz_pregleda.empty()) throw std::domain_error("Nema registriranih pregleda");
    return **std::min_element(niz_pregleda.begin(), niz_pregleda.end(), [](std::shared_ptr<Pregled> p1, std::shared_ptr<Pregled> p2) {
        return Pregled::DolaziPrije(*p1, *p2);
    });
}

Pregled &Pregledi::DajNajranijiPregled() const{
    if(niz_pregleda.empty()) throw std::domain_error("Nema registriranih pregleda");
    return **std::min_element(niz_pregleda.begin(), niz_pregleda.end(), [](std::shared_ptr<Pregled> p1,  std::shared_ptr<Pregled> p2) {
        return Pregled::DolaziPrije(*p1, *p2);
    });
}

int main ()
{
    Pregledi p;

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
