//TP 2023/2024: Zadaća 4, Zadatak 5
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <memory>

class Student{
    int index;
    std::string ime, adresa, telefon, godina;
    std::string skloniRazmak(std::string s); 
    std::string formatirajGodinu(const std::string &s){
        if(s=="1") return "1/B"; 
        if(s=="2") return "2/B"; 
        if(s=="3") return "3/B";
        return s;
    }
    bool dobarTelefon(const std::string &t);

public:
    Student(int idx, std::string g, std::string name, std::string address, std::string phone) :
    index(idx), ime(skloniRazmak(name)), adresa(address), telefon(phone), godina(formatirajGodinu(g)){
        if(index<10000 || index>99999|| !dobarTelefon(phone)) throw std::domain_error("Neispravni parametri");
        if(godina!="1" && godina!="2" && godina!="3" && godina!="1/B" && godina!="2/B" && godina!="3/B" && godina!="1/M" && godina!="2/M" && godina!="3/M" && godina!="1/D" && godina!="2/D" && godina!="3/D")
            throw std::domain_error("Neispravni parametri");
    }                                                                       //hardkodirano da je velicina ispravna
    int DajIndeks() const {return index;}
    std::string DajImePrezime() const {return ime;}
    std::string DajAdresu() const {return adresa;}
    std::string DajTelefon() const {return telefon;}
    std::string DajGodinuStudija() const {return godina;}

    void Ispisi() const;
};

void Student::Ispisi() const{
    std::cout << "Broj indeksa: " << DajIndeks() << std::endl;
    std::cout << "Godina studija: " << DajGodinuStudija() << std::endl;
    std::cout << "Ime i prezime: " << DajImePrezime() << std::endl;
    std::cout << "Adresa: " << DajAdresu() << std::endl;
    std::cout << "Telefon: " << DajTelefon() << std::endl;
}

std::string Student::skloniRazmak(std::string s){
    bool razmak = false;
    std::string rez;
    for(char c : s){
        if(c==' '){
            if(!razmak){
                rez.push_back(' ');
                razmak =1;
            }
        }
        else {
            rez.push_back(c);
            razmak=0;
        }
    }
    while(!rez.empty() && rez.at(0)==' ') rez=rez.substr(1);
    while(!rez.empty() && rez.at(rez.size()-1)==' ') rez.pop_back();
    return rez;
}

bool Student::dobarTelefon(const std::string &t){
    int i=0;
    int n=t.size();

    while(i<n && std::isdigit(t.at(i))) i++;
    if(i==0 || i>=n || t.at(i) != '/') return false;
    i++;

    int novi_pocetak=i;
    while(i<n && std::isdigit(t.at(i))) i++;
    if(i==novi_pocetak || i>=n || t.at(i) != '-') return false;
    i++;

    while(i<n && std::isdigit(t.at(i))) i++;
    if(i==novi_pocetak || i!=n) return false;

    return true;
}

class Laptop{
    int ev_broj;
    std::string naziv, karakteristike;
    Student *kod_koga_je;                   //OVDJE je besmisleno koristiti pametni pokazivac, a ni nema potrebe za njim jer se nista ne alocira
public:
    Laptop(int broj, std::string ime, std::string specs) : ev_broj(broj), naziv(ime), karakteristike(specs), kod_koga_je(nullptr){
        if(broj<0) throw std::domain_error("Neispravni parametri");
    }
    int DajEvidencijskiBroj() const {return ev_broj;}
    std::string DajNaziv() const {return naziv;}
    std::string DajKarakteristike() const {return karakteristike;}

    void Zaduzi(Student &s){
        if(kod_koga_je) throw std::domain_error("Laptop vec zaduzen");
        kod_koga_je=&s;
    }
    void Razduzi(){kod_koga_je=nullptr;}
    bool DaLiJeZaduzen() const {return kod_koga_je;}                        //nullptr, tj nula, tj FALSE!!
    Student &DajKodKogaJe() {
        if(!kod_koga_je) throw std::domain_error("Laptop nije zaduzen");
        return *kod_koga_je;
    }
    Student *DajPokKodKogaJe() const {return kod_koga_je;}
    
    void Ispisi() const{
        std::cout << "Evidencijski broj: " << DajEvidencijskiBroj() << std::endl;
        std::cout << "Naziv: " << DajNaziv() << std::endl;
        std::cout << "Karakteristike: " << DajKarakteristike() << std::endl;
    }
};

class Administracija{
    std::map<int, std::shared_ptr<Student>> student_mapa;
    std::map<int, std::shared_ptr<Laptop>> laptopi_mapa;
public:
    Administracija() {}
    Administracija(const Administracija &a){
        for(const auto &b : a.student_mapa) student_mapa[b.first] = std::make_shared<Student>(*b.second);
        for(const auto &b : a.laptopi_mapa) laptopi_mapa[b.first] = std::make_shared<Laptop>(*b.second);
    }

    Administracija(Administracija &&a) : student_mapa(a.student_mapa), laptopi_mapa(a.laptopi_mapa){}

    Administracija &operator =(Administracija a){
        std::swap(a.student_mapa, student_mapa);
        std::swap(a.laptopi_mapa, laptopi_mapa);
        return *this;
    }

    void RegistrirajNovogStudenta(int idx, std::string g, std::string name, std::string address, std::string phone){
        if(student_mapa.find(idx) != student_mapa.end()) throw std::domain_error("Student s tim indeksom vec postoji");         //NAUCI KORISTIT FIND!!
        student_mapa[idx] = std::make_shared<Student>(idx, g, name, address, phone);
    }
    void RegistrirajNoviLaptop(int broj, std::string ime, std::string specs){
        if(laptopi_mapa.find(broj) != laptopi_mapa.end()) throw std::domain_error("Laptop s tim evidencijskim brojem vec postoji");
        laptopi_mapa[broj] = std::make_shared<Laptop>(broj, ime, specs);
    }
    
    Student &NadjiStudenta(int indx){
        auto it = student_mapa.find(indx);
        if(it==student_mapa.end()) throw std::domain_error("Student nije nadjen");
        return *it->second;
    }

    Student NadjiStudenta(int indx) const {
        auto it = student_mapa.find(indx);
        if(it==student_mapa.end()) throw std::domain_error("Student nije nadjen");
        return *it->second;
    }

    Laptop &NadjiLaptop(int br){
        auto it = laptopi_mapa.find(br);
        if(it==laptopi_mapa.end()) throw std::domain_error("Laptop nije nadjen");
        return *it->second;
    }

    Laptop NadjiLaptop(int br) const {
        auto it = laptopi_mapa.find(br);
        if(it==laptopi_mapa.end()) throw std::domain_error("Laptop nije nadjen");
        return *it->second;
    }

    void IzlistajStudente() const{
        for(const auto &par : student_mapa){
            par.second->Ispisi();
            std::cout<<std::endl;
        }
    }

    void IzlistajLaptope() const{
        for(const auto &par : laptopi_mapa){
            par.second->Ispisi();
            std::cout<<std::endl;
            if(par.second->DaLiJeZaduzen()){
                std::cout<<"Zaduzio(la): "<<par.second->DajKodKogaJe().DajImePrezime()<<" ("<<par.second->DajKodKogaJe().DajIndeks()<<")"<<std::endl;
            }
        }
    }

    void ZaduziLaptop(int idx, int evidencijski){
        try{
            NadjiStudenta(idx);
            NadjiLaptop(evidencijski);
        }
        catch(std::domain_error &e){throw e;}       //pokusaj trika ili glup fazon?
        if(laptopi_mapa.find(evidencijski)->second->DaLiJeZaduzen()) throw std::domain_error("Laptop vec zaduzen");
        
        for(const auto &lap : laptopi_mapa){        //prolazimo kroz sve laptope da vidimo da li je nekoga od njih zaduzio student sa zadanim indeksom, prvo se provjerava da li je laptop uopste zaduzen (Kratkospojna evaluacija!)
            if(lap.second->DaLiJeZaduzen() && lap.second->DajKodKogaJe().DajIndeks() == idx)
                throw std::domain_error("Student je vec zaduzio laptop");
        }
//bespotrebno dugacko ali basically nađemo laptop sa odgovarajucim evindecijskim brojem i njemu zaduzujemo studenta sa odgovarajucim indeksom preko find na mapi i ->second (dereferencirano)
        laptopi_mapa.find(evidencijski)->second->Zaduzi(*student_mapa.find(idx)->second);
    }

    int NadjiSlobodniLaptop() const {
        for(const auto &lap : laptopi_mapa){
            if(!lap.second->DaLiJeZaduzen()) return lap.first;          //mnogo logicnije
        }
        throw std::domain_error("Nema slobodnih laptopa");
    }

    void RazduziLaptop(int ev){         
        try{
            NadjiLaptop(ev);
        }
        catch(std::domain_error &e){throw e;}
        if(!NadjiLaptop(ev).DaLiJeZaduzen()) throw std::domain_error("Laptop nije zaduzen");
        NadjiLaptop(ev).Razduzi();
    }

    void PrikaziZaduzenja() const{
        int count{};
        for(const auto &lap : laptopi_mapa){
            if(lap.second->DaLiJeZaduzen()){
                count++;
                std::cout<<"Student "<<lap.second->DajKodKogaJe().DajImePrezime()<<" ("<<lap.second->DajKodKogaJe().DajIndeks()<<") ";
                std::cout<<"zaduzio/la laptop broj "<<lap.first<<std::endl;
            }
        }
        if(!count) std::cout<<"Nema zaduzenja"<<std::endl;
    }
};

int main ()
{
    Administracija admin;
    int opcija;
    for(;;){
        std::cout << "Odaberite jednu od sljedecih opcija: " << std::endl;
        std::cout << "1 - RegistrirajNovogStudenta" << std::endl;
        std::cout << "2 - RegistrirajNoviLaptop" << std::endl;
        std::cout << "3 - IzlistajStudente" << std::endl;
        std::cout << "4 - IzlistajLaptope" << std::endl;
        std::cout << "5 - NadjiSlobodniLaptop" << std::endl;
        std::cout << "6 - ZaduziLaptop" << std::endl;
        std::cout << "7 - RazduziLaptop" << std::endl;
        std::cout << "8 - PrikaziZaduzenja" << std::endl;
        std::cout << "K - Kraj programa" << std::endl;

        std::cin>>opcija;

        try{
            switch (opcija){
                case 1:{
                    int indeks;
                    std::string godina, ime, adresa, telefon;
                    std::cout<<"\nUnesite broj indeksa: ";
                    std::cin>>indeks;
                    std::cin.clear();
                    std::cin.ignore(99999, '\n');
                    std::cout << "\nUnesite godinu studija (formata A/X, gdje je A godina studija, a X prima vrijednosti B,M,D, zavisno od studija): ";
                    std::getline(std::cin, godina);
                    std::cout << "\nUnesite ime i prezime studenta: ";
                    std::getline(std::cin, ime);
                    std::cout << "\nUnesite adresu studenta: ";
                    std::getline(std::cin, adresa);
                    std::cout << "\nUnesite broj telefona (formata x/x-x): ";
                    std::getline(std::cin, telefon);

                    admin.RegistrirajNovogStudenta(indeks, godina, ime, adresa, telefon);

                    std::cout<<"Student uspjesno registrovan!"<<std::endl;}
                    break;
                case 2:{
                    int broj;
                    std::string naziv, specs;
                    std::cout<<"\nUnesite evidencijski broj laptopa ";
                    std::cin>>broj;
                    std::cin.clear();
                    std::cin.ignore(99999, '\n');
                    std::cout << "\nUnesite naziv laptopa ";
                    std::getline(std::cin, naziv);
                    std::cout << "\nUnesite karakteristike laptopa: ";
                    std::getline(std::cin, specs);

                    admin.RegistrirajNoviLaptop(broj, naziv, specs);
                    std::cout<<"Laptop uspjesno registrovan!"<<std::endl;
                    break;}
                case 3:
                    admin.IzlistajStudente();
                    break;
                case 4:
                    admin.IzlistajLaptope();
                    break;
                case 5:
                    std::cout<<"Prvi slobodan laptop ima evidencijski broj: "<<admin.NadjiSlobodniLaptop()<<std::endl;
                    break;
                case 6:
                    int broj, indeks;
                    std::cout<<"\nUnesite evidencijski broj laptopa: ";
                    std::cin>>broj;
                    std::cout<<"\nUnesite broj indeksa: ";
                    std::cin>>indeks;

                    admin.ZaduziLaptop(indeks, broj);
                    std::cout<<"Laptop uspjesno zaduzen!"<<std::endl;
                    break;
                case 7:
                    int br;
                    std::cout<<"\nUnesite evidencijski broj laptopa: ";
                    std::cin>>br;

                    admin.RazduziLaptop(br);
                    std::cout<<"Laptop uspjesno razduzen!"<<std::endl;
                    break;
                case 8:
                    admin.PrikaziZaduzenja();
                    break;
                default:
                    return 0;
            }
        }
        catch(std::domain_error &e){
            std::cout<<"Izuzetak: "<<e.what()<<"!"<<std::endl;
        }
    }
}
