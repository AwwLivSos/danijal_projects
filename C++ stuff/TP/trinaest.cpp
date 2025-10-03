#include <iostream>
#include <string>

class ApstraktniStudent{
    std::string ime, prezime;
    int indeks;
    int polozeni;
    double prosjek;
    
public:
    ApstraktniStudent(std::string ime, std::string prezime, int indeks) : ime(ime), prezime(prezime), indeks(indeks), polozeni(0), prosjek(5) {}
    virtual ~ApstraktniStudent() = default;
    std::string DajIme() const { return ime; }
    std::string DajPrezime() const { return prezime; }
    int DajBrojIndeksa() const { return indeks; }
    int DajBrojPolozenih() const { return polozeni; }
    double DajProsjek() const { return prosjek; }
    void RegistrirajIspit(int ocjena) {
        if(ocjena < 5 || ocjena > 10) throw std::domain_error("Neispravna ocjena");
        if(ocjena != 5) {
            prosjek = (prosjek * polozeni + ocjena) / (polozeni + 1);
            polozeni++;
        }
    }
    void PonistiOcjene() { polozeni = 0; prosjek = 5; }

    virtual void IspisiPodatke() const = 0;
    virtual ApstraktniStudent *DajKopiju() const = 0;
};

class StudentBachelor : public ApstraktniStudent{
public:
    StudentBachelor(std::string ime, std::string prezime, int indeks) : ApstraktniStudent(ime, prezime, indeks) {}
    void IspisiPodatke() const override {
        std::cout << "Student bachelor studija " << DajIme() << " " << DajPrezime()
        << ", sa brojem indeksa " << DajBrojIndeksa() << "," << std::endl
        << "ima prosjek " << DajProsjek() << "." << std::endl;
    }
    ApstraktniStudent *DajKopiju() const override {return new StudentBachelor(*this);}
};

class StudentMaster : public ApstraktniStudent{
    int godina_diplomiranja;
public:
    StudentMaster(std::string ime, std::string prezime, int indeks, int god_dipl) : ApstraktniStudent(ime, prezime, indeks), godina_diplomiranja(god_dipl) {}
    void IspisiPodatke() const override {
        std::cout << "Student master studija " << DajIme() << " " << DajPrezime()
        << ", sa brojem indeksa " << DajBrojIndeksa() << "," << std::endl
        << "zavrsio bachelor studij godine " << godina_diplomiranja << ", "
        << "ima prosjek " << DajProsjek() << "." << std::endl;
    }
    ApstraktniStudent *DajKopiju() const override {return new StudentMaster(*this);}
};

class Student{
    ApstraktniStudent *as;
    void Test() const { if(!as) throw std::logic_error("Nespecificiran student!"); }
public:
    Student() : as(nullptr) {}
    ~Student() {delete as;}

    Student(const ApstraktniStudent &s) : as(s.DajKopiju()) {}

    Student(const Student &s) : as(!s.as ? nullptr : s.as->DajKopiju()) {
        if(s.as == nullptr) as = nullptr;
        else as = s.as->DajKopiju();
    }

    Student(Student &&s) : as(s.as) { s.as = nullptr; }

    Student &operator =(Student s) { std::swap(as, s.as); return *this; }

    std::string DajIme() const { Test(); return as->DajIme(); }

    std::string DajPrezime() const { Test(); return as->DajPrezime(); }
    int DajBrojIndeksa() const { Test(); return as->DajBrojIndeksa(); }
    int DajBrojPolozenih() const { Test(); return as->DajBrojPolozenih(); }
    double DajProsjek() const { Test(); return as->DajProsjek(); }
    void RegistrirajIspit(int ocjena) { Test(); as->RegistrirajIspit(ocjena); }
    void PonistiOcjene() { Test(); as->PonistiOcjene(); }
    void IspisiPodatke() const { Test(); as->IspisiPodatke(); }

};

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

int main() {
    Kalamuta *pk = new Kalamuta;
    Kalamuta *ck = new Cincozna;
    Cincozna *cc = new Cincozna;

    std::cout << pk->foo() << " " << pk->bar() << " " << pk->baz() << std::endl;

    //6 4 3
    std::cout << ck->foo() << " " << ck->bar() << " " << ck->baz() << std::endl;

    std::cout << cc->foo() << " " << cc->bar() << " " << cc->baz() << std::endl;
    delete pk; delete cc; delete ck;
    return 0;
}