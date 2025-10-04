//TP 2023/2024: Zadaća 5, Zadatak 3
#include <algorithm>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>

class Spremnik{
    double tezina;
    std::string sadrzaj;
public:
    Spremnik(double mass, std::string name) : tezina(mass), sadrzaj(name) {}
    double DajTezinu() const {return tezina;}
    std::string DajSadrzaj() const {return sadrzaj;}
    virtual double DajUkupnuTezinu() const = 0;
    virtual void Ispisi() const = 0;
    virtual Spremnik* DajKopiju() const = 0;
    virtual ~Spremnik(){}
};

class Sanduk : public Spremnik{
    std::vector<double> tezine_predmeta;
public:
    Sanduk(double m, std::string naziv, std::vector<double> predmeti) : Spremnik(m, naziv), tezine_predmeta(predmeti){}
    
    double DajUkupnuTezinu() const override{
        double suma{};
        for(auto x : tezine_predmeta) suma+=x;
        return suma+DajTezinu();
    }
    void Ispisi() const override;
    Spremnik* DajKopiju() const override {return new Sanduk(*this);}
};

void Sanduk::Ispisi() const{
    std::cout<<"Vrsta spremnika: Sanduk"<<std::endl
    <<"Sadrzaj: "<<DajSadrzaj()<<std::endl
    <<"Tezine predmeta: ";
    for(auto x : tezine_predmeta) std::cout<<x<<" ";
    std::cout<<"(kg)"<<std::endl<<"Vlastita tezina: "<<DajTezinu()<<" (kg)"<<std::endl
    <<"Ukupna tezina: "<<DajUkupnuTezinu()<<" (kg)"<<std::endl;
}

class Vreca : public Spremnik{
    double tezina_materije;
public:
    Vreca(double m, std::string naziv, double materija) : Spremnik(m, naziv), tezina_materije(materija){}
    double DajUkupnuTezinu() const override {return tezina_materije + DajTezinu();}
    void Ispisi() const override;
    Spremnik* DajKopiju() const override {return new Vreca(*this);}
};

void Vreca::Ispisi() const{
    std::cout<<"Vrsta spremnika: Vreca"<<std::endl
    <<"Sadrzaj: "<<DajSadrzaj()<<std::endl
    <<"Vlastita tezina: "<<DajTezinu()<<" (kg)"<<std::endl
    <<"Tezina pohranjene materije: "<<DajUkupnuTezinu()-DajTezinu()<<" (kg)"<<std::endl
    <<"Ukupna tezina: "<<DajUkupnuTezinu()<<" (kg)"<<std::endl;
}

class Bure : public Spremnik{
    double gustina;
    double zapremina;
public:
    Bure(double m, std::string naziv, double ro, double V) : Spremnik(m, naziv), gustina(ro), zapremina(V){}
    double DajUkupnuTezinu() const override{ return DajTezinu() + (gustina/1000)*zapremina; }
    double DajGustinu() const {return gustina; }
    double DajZapreminu() const {return zapremina; }
    void Ispisi() const override;
    Spremnik* DajKopiju() const override {return new Bure(*this);}
};

void Bure::Ispisi() const{
    std::cout<<"Vrsta spremnika: Bure"<<std::endl
    <<"Sadrzaj: "<<DajSadrzaj()<<std::endl
    <<"Vlastita tezina: "<<DajTezinu()<<" (kg)"<<std::endl
    <<"Specificna tezina tecnosti: "<<DajGustinu()<<" (kg/m^3)"<<std::endl
    <<"Zapremina tecnosti: "<<DajZapreminu()<<" (l)"<<std::endl
    <<"Ukupna tezina: "<<DajUkupnuTezinu()<<" (kg)"<<std::endl;
}

class Skladiste{
    std::vector<std::shared_ptr<Spremnik>> spremnici;
public:
    Skladiste(){}
    Skladiste (const Skladiste &s);
    Skladiste &operator =(const Skladiste &s);
    Spremnik* DodajSanduk(double m, std::string naziv, std::vector<double> predmeti);
    Spremnik* DodajVrecu(double, std::string, double);
    Spremnik* DodajBure(double, std::string, double, double);
    Spremnik* DodajSpremnik(Spremnik*, bool);
    void BrisiSpremnik(Spremnik*);
    Spremnik &DajNajlaksi();
    Spremnik &DajNajtezi();
    int BrojPreteskih(double) const;
    void IzlistajSkladiste() const;
    void UcitajIzDatoteke(std::string);
};

Skladiste::Skladiste(const Skladiste &s){
    try{
        for(int i=0; i<s.spremnici.size(); i++) spremnici.push_back(std::shared_ptr<Spremnik>(s.spremnici[i]->DajKopiju()));
    }catch(...){spremnici.clear(); throw;}                  //ne znam sto ce mi ovo al et za svaki slucaj
}

Skladiste &Skladiste::operator =(const Skladiste &s){
    if(this!=&s){
        spremnici.clear();
        try{
            for(int i=0; i<s.spremnici.size(); i++) spremnici.push_back(std::shared_ptr<Spremnik>(s.spremnici[i]->DajKopiju()));
        }catch(...){spremnici.clear(); throw;}
    }
    return *this;
}

Spremnik* Skladiste::DodajSanduk(double m, std::string naziv, std::vector<double> predmeti){
    auto novi = std::make_shared<Sanduk>(m, naziv, predmeti);
    spremnici.push_back(novi);  //ovo mi je promaklo, tip ovdje treba biti tip koji se dodaje
    return &(*novi);                    //dereferenciran pokazivac i uzeta adresa dobivenog predmeta
}

Spremnik* Skladiste::DodajVrecu(double m, std::string naziv, double materija){
    auto novi = std::make_shared<Vreca>(m, naziv, materija);
    spremnici.push_back(novi);
    return &(*novi);                    
}

Spremnik* Skladiste::DodajBure(double m, std::string naziv, double ro, double V){
    auto novi = std::make_shared<Bure>(m, naziv, ro, V);
    spremnici.push_back(novi);
    return &(*novi);                    
}

Spremnik* Skladiste::DodajSpremnik(Spremnik* s, bool moj){      //obicni pokazivac ili? ova cijela metoda moze bit opasna i netacna
    if(moj){
        spremnici.push_back(std::shared_ptr<Spremnik>(s));
        return &(*s);
    }else{
        auto novi = std::shared_ptr<Spremnik>(s->DajKopiju());                   //std::make_shared je zamka cista
        spremnici.push_back(novi);
        //nova adresa = novi poiner
        return &(*novi); //bez ovoga, kao belaj neki, ne bi trebao prolazit autotest
    }
    //return nova adresa
}

void Skladiste::BrisiSpremnik(Spremnik *s) {                 //upitno
    auto it = std::remove_if(spremnici.begin(), spremnici.end(), [s](const std::shared_ptr<Spremnik>& ptr) { return ptr.get() == s; });
    spremnici.erase(it, spremnici.end());
}

Spremnik &Skladiste::DajNajlaksi(){
    if(spremnici.empty()) throw std::range_error("Skladiste je prazno");
    return **std::min_element(spremnici.begin(), spremnici.end(), [](const std::shared_ptr<Spremnik>& a, const std::shared_ptr<Spremnik>& b) {return a->DajTezinu() < b->DajTezinu();});
    //dereferencirano dvaput jer min_element vraca iterator koji se dereferencira na pokazivac koji pokazuje na ono sto trazimo
}

Spremnik &Skladiste::DajNajtezi(){
    if(spremnici.empty()) throw std::range_error("Skladiste je prazno");
    return **std::max_element(spremnici.begin(), spremnici.end(), [](const std::shared_ptr<Spremnik>& a, const std::shared_ptr<Spremnik>& b) {return a->DajTezinu() < b->DajTezinu();});
}

int Skladiste::BrojPreteskih(double n) const{
    return std::count_if(spremnici.begin(), spremnici.end(), [n](const std::shared_ptr<Spremnik> &s){return s->DajUkupnuTezinu() > n;});
}

void Skladiste::IzlistajSkladiste() const {
    auto kopija = spremnici;
    std::sort(kopija.begin(), kopija.end(), [](const std::shared_ptr<Spremnik>& a, const std::shared_ptr<Spremnik>& b){return a->DajUkupnuTezinu() > b->DajUkupnuTezinu();});
    for(const auto &x : kopija) x->Ispisi();
}

void Skladiste::UcitajIzDatoteke(std::string ime){
    std::ifstream ulaz(ime);
    if(!ulaz) throw std::logic_error("Trazena datoteka ne postoji");
    spremnici.clear();
    std::string prvi_red, drugi_red;
    while(std::getline(ulaz, prvi_red)){
        std::istringstream iss(prvi_red);
        char prvo_slovo;
        std::string stvar;
        iss>>prvo_slovo;
        iss.get();                  //uzme razmak
        std::getline(iss, stvar);

        if(std::getline(ulaz, drugi_red)){
            std::istringstream iss2(drugi_red);             //novi iss jer string sadrzi jedan red u sebi uz getline
            if(prvo_slovo == 'S'){
                double tezina_sanduka;
                int broj_predmeta;
                if(!(iss2>>tezina_sanduka>>broj_predmeta)) throw std::logic_error("Datoteka sadrzi besmislene podatke");
                std::vector<double> predmeti(broj_predmeta);
                for(int i=0; i<broj_predmeta; i++) 
                    if(!(iss2>>predmeti[i])) throw std::logic_error("Datoteka sadrzi besmislene podatke");
                DodajSanduk(tezina_sanduka, stvar, predmeti);
            }
            else if (prvo_slovo == 'V'){
                double tezina_vrece, tezina_materije;
                if(!(iss2>>tezina_vrece>>tezina_materije)) throw std::logic_error("Datoteka sadrzi besmislene podatke");
                DodajVrecu(tezina_vrece, stvar, tezina_materije);
            }
            else if (prvo_slovo == 'B'){
                double tezina_bureta, gustina, zapremina;
                if(!(iss2>>tezina_bureta>>gustina>>zapremina)) throw std::logic_error("Datoteka sadrzi besmislene podatke");
                DodajBure(tezina_bureta, stvar, gustina, zapremina);
            }
            else throw std::logic_error("Datoteka sadrzi besmislene podatke");
        }
        else throw std::logic_error("Datoteka sadrzi besmislene podatke");
    }
    if(ulaz.bad()) throw std::logic_error("Problemi pri citanju datoteke");
}

int main ()
{
    try{
        Skladiste s1;
        s1.UcitajIzDatoteke("ROBA.TXT");
        s1.IzlistajSkladiste();
    }catch(std::logic_error &e){
        std::cout<<e.what()<<std::endl;
    }catch(std::range_error &e){
        std::cout<<e.what()<<std::endl;
    }catch(...){
        std::cout<<"Boga pitaj sta je problem"<<std::endl;
    }

	return 0;
}