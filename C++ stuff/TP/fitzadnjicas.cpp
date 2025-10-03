#include <iostream>
#include <exception>
#include <regex>

using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum Kriteriji { CISTOCA, USLUGA, LOKACIJA, UDOBNOST };

void IspisiEnum(Kriteriji k){
    string niz[] = {"Cistoca", "Usluga", "Lokacija", "Udobnost"};
    cout<<niz[static_cast<int>(k)];
}

void IspisiSaSwitch(Kriteriji k){
    switch (k){

        case CISTOCA:
            cout<<"Cistoca";
            break;
        
        case USLUGA:
            cout<<"Usluga";
            break;

    
    }
}

bool ValidirajBrojPasosa(std::string brojPasosa){

    /* - pocetak sadrzi jedan ili dva velika slova
    - nakon toga slijede tri ili cetiri broja
    - nakon toga moze, a ne mora, doci znak crtica (-) ili razmak ( )
    - nakon toga slijede dva do cetiri broja */

    regex pattern("^[A-Z]{1,2}[0-9]{3,4}[- ]?[0-9]{2,4}$");
    return regex_match(brojPasosa, pattern);
}

template<class T1, class T2>
class Kolekcija {
    T1* _elementi1;
    T2* _elementi2;
    int* _trenutno;
    bool _omoguciDupliranje;
public:

    Kolekcija(bool omoguciDupliranje = true) {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _trenutno = new int(0);
        _omoguciDupliranje = omoguciDupliranje;
    }

    ~Kolekcija() {
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
        delete _trenutno; _trenutno = nullptr;
    }

    Kolekcija(const Kolekcija& obj) {
        _trenutno = new int(obj.getTrenutno());
        _elementi1 = new T1[getTrenutno()];
        _elementi2 = new T2[getTrenutno()];

        for (int i = 0; i < getTrenutno(); i++) {
            _elementi1[i] = obj.getElement1(i);
            _elementi2[i] = obj.getElement2(i);
        }
    }

    Kolekcija& operator=(const Kolekcija& obj) {
        if (this != &obj) {
            delete[]_elementi1; _elementi1 = nullptr;
            delete[]_elementi2; _elementi2 = nullptr;
            delete _trenutno; _trenutno = nullptr;
        
            _trenutno = new int(obj.getTrenutno());
            _elementi1 = new T1[getTrenutno()];
            _elementi2 = new T2[getTrenutno()];
            for (int i = 0; i < getTrenutno(); i++) {
                _elementi1[i] = obj.getElement1(i);
                _elementi2[i] = obj.getElement2(i);
            }
        }
        return *this;
    }

    bool AddElement(const T1& obj1, const T2& obj2) {
       if(!_omoguciDupliranje)
           for (int i = 0; i < getTrenutno(); i++) 
               if (_elementi1[i] == obj1 && _elementi2[i] == obj2) {
                   throw domain_error("Vec postoji ovaj par elemenata u kolekciji, dupliranje onemoguceno!\n");
                   return false;
               }
                
       
       T1* temp1 = _elementi1;
       T2* temp2 = _elementi2;

       _elementi1 = new T1[getTrenutno() + 1];
       _elementi2 = new T2[getTrenutno() + 1];

       for (int i = 0; i < getTrenutno(); i++) {
           _elementi1[i] = temp1[i];
           _elementi2[i] = temp2[i];
       }

       _elementi1[getTrenutno()] = obj1;
       _elementi2[getTrenutno()] = obj2;

       (*_trenutno)++;

       delete[] temp1; temp1 = nullptr;
       delete[] temp2; temp2 = nullptr;

       return true;
    }

    Kolekcija& InsertAt(const int index, const T1& obj1, const T2& obj2) {
        if (index < 0 || index > getTrenutno()) {
            cout << "Pozicija je izvan opsega kolekcije!\n";
            return *this;
        }

        T1* temp1 = _elementi1;
        T2* temp2 = _elementi2;

        _elementi1 = new T1[getTrenutno() + 1];
        _elementi2 = new T2[getTrenutno() + 1];

        for (int i = 0; i < index; i++) {
            _elementi1[i] = temp1[i];
            _elementi2[i] = temp2[i];
        }
        _elementi1[index] = obj1;
        _elementi2[index] = obj2;
        
        (*_trenutno)++;

        for (int i = index+1; i < getTrenutno(); i++) {
            _elementi1[i] = temp1[i-1];
            _elementi2[i] = temp2[i-1];
        }
        
        delete[] temp1; temp1 = nullptr;
        delete[] temp2; temp2 = nullptr;
        
        return *this;
    }

    T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno() const { return *_trenutno; }

    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (int i = 0; i < *obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
};

class Datum {
    int* _dan, * _mjesec, * _godina;
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }
    Datum(const Datum& obj) {
        _dan = new int(*obj._dan);
        _mjesec = new int(*obj._mjesec);
        _godina = new int(*obj._godina);
    }
    Datum& operator=(const Datum& obj) {
        if (this != &obj) {
            delete _dan; _dan = nullptr;
            delete _mjesec; _mjesec = nullptr;
            delete _godina; _godina = nullptr;

            _dan = new int(*obj._dan);
            _mjesec = new int(*obj._mjesec);
            _godina = new int(*obj._godina);
        }
        return *this;
    }
    ~Datum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << obj._dan << "." << obj._mjesec << "." << obj._godina;
        return COUT;
    }
};

class Komentar {
    char* _sadrzajKomentara;
    //uz komentar, gosti mogu ocijeniti svaki od kriterija zasebno, a int se odnosi na ocjenu u rasponu od 1 - 10.
    //onemoguciti dupliranje kriterija tj. svaki kriterij se moze ocijeniti samo jednom...u suprotnom baciti objekat tipa exception
    Kolekcija<Kriteriji, int>* _ocjeneKriterija;
public:
    Komentar(const char* sadrzajKomentara="") {
        //kreirajte funkciju GetNizKaraktera ili na drugi adekvatan nacin izvrsite inicijalizaciju pokazivaca
        _sadrzajKomentara = GetNizKaraktera(sadrzajKomentara);
        _ocjeneKriterija = nullptr;       
    }
    ~Komentar() {
        delete[] _sadrzajKomentara; _sadrzajKomentara = nullptr;
        delete _ocjeneKriterija; _ocjeneKriterija = nullptr;
    }
    char* GetSadrzajKomentara() { return _sadrzajKomentara; }
    Kolekcija<Kriteriji, int>*  GetOcjeneKriterija() { return _ocjeneKriterija; }
};

int main() {

    Datum
        datum19062022(19, 6, 2022),
        datum20062022(20, 6, 2022),
        datum30062022(30, 6, 2022),
        datum05072022(5, 7, 2022);

    int kolekcijaTestSize = 9;
    Kolekcija<int, int> kolekcija1(false);
    for (int i = 0; i <= kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);//dodaje vrijednosti u kolekciju

    try {
        //ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 i T2), metoda AddElement treba baciti objekat tipa exception
        kolekcija1.AddElement(3, 3);
    }
    catch (exception& err) {
        cout << err.what() << crt;
    }

    cout << kolekcija1 << crt;

    /* metoda InsertAt treba da doda vrijednosti drugog i treceg parametra na lokaciju koja je definisana prvim parametrom. Povratna vrijednost metode
   * je objekat (pozivaoc metode, u konkretnom slucaju objekat kolekcija1) u okviru koga su, na definisanu lokaciju, dodati zahtijevani parametri.
   * Nakon izvrsenja metode InsertAt, oba objekta, kolekcija1 i kolekcija2, bi trebali posjedovati sljedeci sadrzaj:
   * 10 10
   * 0  0
   * 1  1
   * 2  2
   * ....
   */
    Kolekcija<int, int> kolekcija2 = kolekcija1.InsertAt(0, 10, 10);
    cout << kolekcija2 << crt;

    /*
    * za validaciju broja pasosa koristiti funkciju ValidirajBrojPasosa koja treba, koristeci regex, osigurati postivanje
    sljedeceg formata:
    - pocetak sadrzi jedan ili dva velika slova
    - nakon toga slijede tri ili cetiri broja
    - nakon toga moze, a ne mora, doci znak crtica (-) ili razmak ( )
    - nakon toga slijede dva do cetiri broja

    pomenutu funkciju iskoristiti u konstruktoru klase Gost, te ukoliko broj pasosa nije u validnom formatu onda njegova vrijednost treba biti postavljena na NOT VALID
    */

    if (ValidirajBrojPasosa("BH235-532"))
        cout << "Broj pasosa BH235-532 validan" << endl;
    if (ValidirajBrojPasosa("B123321"))
        cout << "Broj pasosa B123321 validan" << endl;
    if (ValidirajBrojPasosa("B1252 521"))
        cout << "Broj pasosa B1252 521 validan" << endl;

    if (!ValidirajBrojPasosa("H4521"))
        cout << "Broj pasosa H4521 NIJE validan" << endl;
    if (!ValidirajBrojPasosa("b1252 521"))
        cout << "Broj pasosa b1252 521 NIJE validan" << endl;

    return 0;

}