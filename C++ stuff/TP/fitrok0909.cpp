#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum Pojas { BIJELI, ZUTI, NARANDZASTI, ZELENI, PLAVI, SMEDJI, CRNI };
const int brojTehnika = 6;
const char* NIJE_VALIDNA = "<VRIJEDNOST_NIJE_VALIDNA>";

char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr) return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy(temp, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}

template<class T1, class T2, int max = 10>
class Kolekcija {

	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };

	int* _trenutno;
    
public:
	Kolekcija() { _trenutno = new int(0); }

	~Kolekcija() {
		
		for (size_t i = 0; i < *_trenutno; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		delete _trenutno; _trenutno = nullptr;
	}

    Kolekcija(const Kolekcija &obj){

        _trenutno = new int(obj.getTrenutno());

        for(int i=0; i<getTrenutno(); i++){
            _elementi1[i] = new T1(obj.getElement1(i));
            _elementi2[i] = new T2(obj.getElement2(i));
        }
    }

    Kolekcija &operator=(const Kolekcija &obj){
        if(this != &obj){
            for(int i=0; i<getTrenutno(); i++){
                delete _elementi1[i];
                delete _elementi2[i];
            }
            *_trenutno = obj.getTrenutno();

            for(int i=0; i<getTrenutno(); i++){
                _elementi1[i] = new T1(obj.getElement1(i));
                _elementi2[i] = new T2(obj.getElement2(i));
            }
        }
        return *this;
    }

    void AddElement(const T1 &obj1, const T2 &obj2){
        if(getTrenutno() == max) throw domain_error("Dostignut maksimalni broj elemenata");

        _elementi1[getTrenutno()] = new T1(obj1);
        _elementi2[(*_trenutno)++] = new T2(obj2);
    }

    Kolekcija &InsertAt(int index, const T1 &obj1, const T2 &obj2){
        if(index > getTrenutno() || index<0 || getTrenutno() == max) 
            throw std::domain_error("Nemoguc zahtjev!");

        for(int i = getTrenutno(); i>index; i--){
            _elementi1[i] = _elementi1[i-1];
            _elementi2[i] = _elementi2[i-1];
        }

        //8 9 565 100 54 6 51 5

        _elementi1[index] = new T1(obj1);
        _elementi2[index] = new T2(obj2);

        (*_trenutno)++;

        return *this;
    }

    Kolekcija* RemoveRange(int OD, int DO) {
        if (OD > DO || OD < 0 || DO >= getTrenutno())
            throw std::out_of_range("Nevazeci indeks!");

        //5 9 545 62 652 62 6

        Kolekcija* removed = new Kolekcija<T1, T2, max>;
        int newIndex = 0;

        for (int i = 0; i < *_trenutno; i++) {
            if (i >= OD && i <= DO) {
                removed->AddElement(getElement1(i), getElement2(i));

                delete _elementi1[i]; _elementi1[i] = nullptr;
                delete _elementi2[i]; _elementi2[i] = nullptr;

            } else {
                _elementi1[newIndex] = _elementi1[i];
                _elementi2[newIndex] = _elementi2[i];
                if (newIndex != i) { 
                    _elementi1[i] = nullptr;
                    _elementi2[i] = nullptr;
                }
                newIndex++;
            }
        }

        *_trenutno = newIndex;

        return removed;
    }

	T1& getElement1(int lokacija)const { 
        return *_elementi1[lokacija]; 
    }
	T2& getElement2(int lokacija)const { 
        if(lokacija < 0) throw std::domain_error("Ilegalan opseg!");
        if(!_elementi2[lokacija]) throw std::domain_error("Pokazivac je nulpokazivac");
        return *_elementi2[lokacija];
     }
	int getTrenutno() { return *_trenutno; }

	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
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
		_godina = new int (godina);
	}

	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}

    Datum(const Datum &d){
        _dan = new int (*d._dan);
        _mjesec = new int (*d._mjesec);
        _godina = new int (*d._godina);
    }    

    Datum &operator=(const Datum &d){
        if(this!=&d){
            delete _dan; _dan = nullptr;
            delete _mjesec; _mjesec = nullptr;
            delete _godina; _godina = nullptr;
        
            _dan = new int (*d._dan);
            _mjesec = new int (*d._mjesec);
            _godina = new int (*d._godina);
        }
        return *this;
    }

    int GetDan() const { return *_dan;}
    int GetMjesec() const {return *_mjesec;}
    int GetGodina() const {return *_godina;}

	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
};

bool DovoljnoStariji(const Datum &d1, const Datum &d2){
    int dani1 = d1.GetDan() + d1.GetMjesec()*30 + d1.GetGodina()*365;
    int dani2 = d2.GetDan() + d2.GetMjesec()*30 + d2.GetGodina()*365;

    return dani2-dani1>3;
}

class Tehnika {
	char* _naziv;
	//int se odnosi na ocjenu u opsegu od 1 – 5, a Datum na datum kada je ocijenjena odredjena tehnika 
	Kolekcija<int, Datum*, brojTehnika>* _ocjene;
public:

	Tehnika(const char* naziv) {
		_naziv = GetNizKaraktera(naziv);
		_ocjene = nullptr;
	}

	~Tehnika() {
		delete[] _naziv; _naziv = nullptr;
		delete _ocjene; _ocjene = nullptr;
	}

    bool AddOcjena(int ocjena, Datum &datumPolaganja){

        if(!_ocjene) _ocjene = new Kolekcija<int, Datum*, brojTehnika>();

        int indeks = _ocjene->getTrenutno();

        if(indeks != 0 && !DovoljnoStariji(*_ocjene->getElement2(indeks-1), datumPolaganja)) return false;

        _ocjene->AddElement(ocjena, new Datum(datumPolaganja));
        return true;
    }

	char* GetNaziv() { return _naziv; }
	Kolekcija<int, Datum*, brojTehnika>& GetOcjene() { return *_ocjene; }
};



int main(){

    Datum   datum19062020(19, 6, 2020),
		datum20062020(20, 6, 2020),
		datum30062020(30, 6, 2020),
		datum05072020(5, 7, 2020);

    Tehnika choku_zuki("choku_zuki"),
		gyaku_zuki("gyaku_zuki"),
		kizami_zuki("kizami_zuki"),
		oi_zuki("oi_zuki");

    /*svaka tehnika moze imati vise ocjena tj. moze se polagati u vise navrata.
		-   razmak izmedju polaganja dvije tehnike mora biti najmanje 3 dana
		-   nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec evidentirane (bez obzira sto je stariji od 3 dana)
	*/

	if (choku_zuki.AddOcjena(1, datum19062020))
		cout << "Ocjena evidentirana!" << endl;
	if (!choku_zuki.AddOcjena(5, datum20062020))
		cout << "Ocjena NIJE evidentirana!" << endl;
	if (choku_zuki.AddOcjena(5, datum30062020))
		cout << "Ocjena evidentirana!" << endl;
    

    return 0;
}