#include <iostream>
#include <cmath>
#include <vector>

class Vektor3D{
    double x, y, z;
    mutable int brojIspisa=0;
    
    static int brojvektora;

public:

    Vektor3D(double x, double y, double z){
        this->x = x; this->y=y; Vektor3D::z=z;
    }

    Vektor3D(const Vektor3D &kopija);
    
    Vektor3D &Postavi(double x, double y, double z){
        Vektor3D::x=x; this->y = y; Vektor3D::z = z;
        brojvektora++;
        return *this;
    }

    Vektor3D &PostaviX(double x){this->x = x; return *this;}
    Vektor3D &PostaviY(double y){this->y = y; return *this;}
    Vektor3D &PostaviZ(double z){this->z = z; return *this;}

    void Ocitaj(double &x, double &y, double &z){
        x=Vektor3D::x; y= Vektor3D::y; z=Vektor3D::z;
    }

    void Ispisi() const {
        std::cout << "{" << x << "," << y << "," << z << "}";
        brojIspisa++;
    }

    double DajX() const {return x;}
    double DajY() const {return y;}
    double DajZ() const {return z;}

    double DajDuzinu() const { return std::sqrt(x * x + y * y + z * z); }

    Vektor3D &PomnoziSaSkalarom(double s){
        x *= s; y *= s; z *= s;
        return *this;
    }

    friend Vektor3D ZbirVektora(const Vektor3D &v1, const Vektor3D &v2);
};

Vektor3D::Vektor3D(const Vektor3D &kopija){
    this->x = kopija.x;
    this->y = kopija.y;
    this->z = kopija.x;
}

/*Modelirati klasu u C++ sa nazivom BankovniRacun koja predstavlja jedan bankovni racun sa atributima
ime, balans i broj racuna. Klasa treba da ima metodu Postavi, koja prima argumente za sva 3 atributa
i postavlja ih na te vrijednosti. Ako jedan od argumenata nije ispravan (prazno ime ili negativni brojevi),
metoda treba da baci izuzetak sa porukom "Neispravni parametri!". Omogućiti kaskadni poziv drugih metoda
iz ove metode (napraviti da ova metoda vraca referencu na modificirani objekat). 
Potrebne su i dvije metode sa nazivima Uplati i Isplati koje ne vracaju nista. Metoda Uplati prima iznos 
i povecava balans za taj iznos, dok metoda Isplati prima isti argument, ali smanjuje balans za taj iznos.
Obratiti paznju da iznos koji se isplacuje ne moze biti veci od samog balansa. Ako je to slucaj, 
baciti izuzetak sa porukom "Nemoguca transakcija!". Napisati i 3 jednostavne "get" metode za sva 3 argumenta,
te dodati jos i metodu Ispisi, koja ispisuje informacije klase u proizvoljnom formatu

Napisati i kratki testni main program koji ce testirati sve funkcionalnosti klase BankovniRacun*/

class BankovniRacun{
    std::string ime;
    int brojRacuna;
    double balans;
    Vektor3D v1;
public:

    BankovniRacun(std::string ime, int racun, int balans) : v1(1,1,1), ime(ime){
        Postavi(ime, racun, balans);
    }

    BankovniRacun &Postavi(const std::string &novo_ime, int novi_broj, double novi_balans) {
        ime = novo_ime;
        brojRacuna = novi_broj;
        balans = novi_balans;
        return *this;
    }

    std::string dajIme() const {return ime;}
    int dajBrojRacuna() const {return brojRacuna;}
    double dajBalans() const {return balans;}

    void Uplati(double iznos) {
        if (iznos < 0)
            throw std::domain_error("Iznos ne moze bii negativan!");
        else
            balans += iznos;
    }

    void Isplati(double iznos) {
        if (iznos < 0)
            std::cout << "Iznos isplate ne moze biti negativan!" << std::endl;
        else if (iznos > balans)
            std::cout << "Nemoguca transakcija! Nedovoljno sredstava." << std::endl;
        else
            balans -= iznos;
    }

    void Ispisi() {
        std::cout << "Vlasnik: " << ime << std::endl;
        std::cout << "Broj racuna: " << brojRacuna << std::endl;
        std::cout << "Balans: " << balans << " KM" << std::endl;
    }
};

int Vektor3D::brojvektora = 0;

Vektor3D ZbirVektora(const Vektor3D &v1, const Vektor3D &v2){
    return {v1.x+v2.x, v1.y+v2.y, v1.z+v2.z};
}

int main(){
    
    std::vector<int> v(10, 9), w(v);
    Vektor3D v3d(1,1,1);
    std::cout<<v3d.DajX();

    Vektor3D v1(v3d), v2 = Vektor3D(1,1,1);
    int a{};

    Vektor3D* vpok = new Vektor3D(0,0,30);
    vpok = new Vektor3D(5,5,5);

    vpok->Ispisi();

    return 0;
}