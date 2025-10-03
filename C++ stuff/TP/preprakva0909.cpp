#include <iostream>
#include <vector>
#include <cstring>
#include <exception>
#include <regex>
using namespace std;


const char* crt = "\n-------------------------------------------\n";
enum Pojas { BIJELI, ZUTI, NARANDZASTI, ZELENI, PLAVI, SMEDJI, CRNI };
const int brojTehnika = 6;
const char* NIJE_VALIDNA = "<VRIJEDNOST_NIJE_VALIDNA>";


char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}

/*
	za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi:
	-   najmanje 7 znakova
	-   velika i mala slova
	-   najmanje jedan broj
	-   najmanje jedan specijalni znak
	za provjeru validnosti lozinke koristiti globalnu funkciju ValidirajLozinku, a unutar nje regex metode.
	validacija lozinke se vrsi unutar konstruktora klase Korisnik, a u slucaju da nije validna
	postaviti je na podrazumijevanu vrijednost: <VRIJEDNOST_NIJE_VALIDNA>
	*/

bool ValidirajLozinku(const string lozinka) {

	string brojZnakova = "(?=.{7,})";//najmanje 7 znakova
	string slovaVelika = "(?=.*[A-Z])";//velika slova
	string slovaMala = "(?=.*[a-z])";//mala slova
	string brojevi = "(?=.*[0-9]{1,})";//najmanje jedan broj
	string specZnakovi = "(?=.*[!?@_?.&#$]{1,})";//najmanje jedan specijalni znak
	
	string pattern = brojZnakova + slovaMala+ slovaVelika + brojevi + specZnakovi;

	regex rule(pattern);

	return (regex_search(lozinka, rule));
		
}

template<class T1, class T2, int max = 10>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int* _trenutno;
public:
	Kolekcija() { 
		_trenutno = new int(0);
	}
	~Kolekcija() {

		for (size_t i = 0; i < *_trenutno; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		delete _trenutno; _trenutno = nullptr;
	}
	Kolekcija(const Kolekcija& obj) {
		
		_trenutno = new int(obj.getTrenutno());
		for (int i = 0; i < getTrenutno(); i++) {
		
			_elementi1[i] = new T1(obj.getElement1(i));
			_elementi2[i] = new T2(obj.getElement2(i));
		}
	
	}
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != *obj) {
			for (size_t i = 0; i < *_trenutno; i++) {
				delete _elementi1[i]; _elementi1[i] = nullptr;
				delete _elementi2[i]; _elementi2[i] = nullptr;
			}
			delete _trenutno; _trenutno = nullptr;
		}
		_trenutno = new int(obj.getTrenutno());
		for (int i = 0; i < getTrenutno(); i++) {

			_elementi1[i] = new T1(obj.getElement1(i));
			_elementi2[i] = new T2(obj.getElement2(i));
		}
		return *this;
	}


	T1& getElement1(int lokacija)const { return *_elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return *_elementi2[lokacija]; }
	int getTrenutno() const { return *_trenutno; }
	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	bool AddElement(const T1& obj1, const T2& obj2) {
		if (getTrenutno() == max)
			throw exception("Greska u dodavanju elemenata u kolekciju!\n");
		_elementi1[getTrenutno()] = new T1(obj1);
		_elementi2[getTrenutno()] = new T2(obj2);
		(*_trenutno)++;
		return true;
	}

	bool AddElement(const T1& obj1, const T2& obj2, const int index) {
		if (getTrenutno() == max || index < 0 || index > max) {
			cout << "Greska u dodavanju elementa u kolekciju!\n";
			return false;
		}

		for (int i = getTrenutno(); i > index; i--) {
			_elementi1[i] = _elementi1[i - 1];
			_elementi2[i] = _elementi2[i - 1];
		}
		_elementi1[index] = new T1(obj1);
		_elementi2[index] = new T2(obj2);
		(*_trenutno)++;
		return true;
	}

	bool RemoveAt(const int index) {
		if (index < 0 || index > getTrenutno()) {
			cout << "Greska u opsegu index-a!\n";
			return false;
		}
		if (_elementi1[index] != nullptr) {
			delete _elementi1[index]; _elementi1[index] = nullptr;
		}
		if (_elementi2[index] != nullptr) {
			delete _elementi2[index]; _elementi2[index] = nullptr;
		}
		
		for (int i = index; i < getTrenutno() - 1; i++) {
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		_elementi1[getTrenutno() - 1] = nullptr;
		_elementi2[getTrenutno() - 1] = nullptr;
		(*_trenutno)--;
		return true;
	}
	
	T2& operator [] (const T1& value)  {
		int lokacija;
		for (int i = 0; i < getTrenutno(); i++) {
			if (*_elementi1[i] == value) {
				lokacija = i;
				*_elementi2[lokacija] = i;
				return *_elementi2[i];
			}
				
		}
		throw exception("Greska u operatoru == \n");
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
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	int getDan() const { return (_dan == nullptr) ? 1 : *_dan; }
	int getMjesec() const { return (_mjesec == nullptr) ? 1 : *_mjesec; }
	int getGodina() const { return (_godina == nullptr) ? 2000 : *_godina; }

	Datum(const Datum& obj) {
		_dan = new int(obj.getDan());
		_mjesec = new int(obj.getMjesec());
		_godina = new int(obj.getGodina());
	}
	Datum& operator=(const Datum& obj) {
		if (this != &obj) {
			delete _dan; _dan = nullptr;
			delete _mjesec; _mjesec = nullptr;
			delete _godina; _godina = nullptr;

			_dan = new int(obj.getDan());
			_mjesec = new int(obj.getMjesec());
			_godina = new int(obj.getGodina());
		}
		return *this;
	}
	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
};

int ToDays(const Datum& obj) {

	return obj.getDan() + obj.getMjesec()*30 + obj.getGodina() * 365;
}

class Tehnika {
	char* _naziv;
	//int se odnosi na ocjenu u opsegu od 1 – 5, a Datum na datum kada je ocijenjena odredjena tehnika 
	Kolekcija<int, Datum*, brojTehnika>* _ocjene;
public:
	Tehnika(const char* naziv) {
		_naziv = GetNizKaraktera(naziv);
		_ocjene = new Kolekcija<int, Datum*, brojTehnika>();
	}
	~Tehnika() {
		delete[] _naziv; _naziv = nullptr;
		delete _ocjene; _ocjene = nullptr;
	}
	bool AddOcjena(const int ocjena, const Datum& datum) {
		if(_ocjene->getTrenutno() == 0)
			_ocjene->AddElement(ocjena, new Datum(datum));
		else {
			int razlika = ToDays(datum) - ToDays(*_ocjene->getElement2(_ocjene->getTrenutno() - 1));
			if (razlika >= 3)
				_ocjene->AddElement(ocjena, new Datum(datum));
			else
				return false;
		}
		return true;
	}
	char* GetNaziv() { return _naziv; }
	Kolekcija<int, Datum*, brojTehnika>& GetOcjene() { return *_ocjene; }

	/* ispisuje: naziv tehnike, ocjene (zajedno sa datumom) i prosjecnu ocjenu za tu tehniku
	   ukoliko tehnika nema niti jednu ocjenu prosjecna treba biti 0*/
	friend ostream& operator<< (ostream& COUT, const Tehnika& obj) {
		float suma = 0.0f;
		int brojOcjena = obj._ocjene->getTrenutno();
		COUT << "Naziv tehnike:\t" << obj._naziv << endl;
		for (size_t i = 0; i < obj._ocjene->getTrenutno(); i++) {
			COUT << "Ocjena:\t" << obj._ocjene->getElement1(i) << " datum " << *obj._ocjene->getElement2(i) << endl;
			suma += obj._ocjene->getElement1(i);
		}
		if (obj._ocjene->getTrenutno() > 0)
			COUT << "Prosjecna ocjena tehnike:\t" << suma / obj._ocjene->getTrenutno() << endl;
		else
			COUT << "Prosjecna ocjena tehnike:\t" << 0 << endl;
		return COUT;
	}
};

class Polaganje {
	Pojas _pojas;
	vector<Tehnika*> _polozeneTehnike;
public:
	Polaganje(Pojas pojas = BIJELI) {
		_pojas = pojas;
		
	}
	~Polaganje() {
		for (size_t i = 0; i < _polozeneTehnike.size(); i++) {
			delete _polozeneTehnike[i];
			_polozeneTehnike[i] = nullptr;
		}
	}
	vector<Tehnika*>& GetTehnike() { return _polozeneTehnike; }
	Pojas GetPojas() { return _pojas; }

	friend ostream& operator<< (ostream& COUT, const Polaganje& obj) {
		COUT << obj._pojas << endl;
		for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
			COUT << *obj._polozeneTehnike[i];
		return COUT;
	}
};

class Korisnik {
	char* _imePrezime;
	string _emailAdresa;
	string _lozinka;
public:
	Korisnik(const char* imePrezime, string emailAdresa, string lozinka){
		_imePrezime = GetNizKaraktera(imePrezime);
		_emailAdresa = emailAdresa;
		_lozinka = ValidirajLozinku(lozinka) ? lozinka : NIJE_VALIDNA;
	}

	virtual ~Korisnik() { delete[] _imePrezime; _imePrezime = nullptr; }

	string GetEmail() { return _emailAdresa; }
	string GetLozinka() { return _lozinka; }
	char* GetImePrezime() { return _imePrezime; }
};

class KaratePolaznik : public Korisnik {
	vector<Polaganje> _polozeniPojasevi;
public:
	KaratePolaznik(const char* imePrezime, string emailAdresa, string lozinka) : Korisnik(imePrezime, emailAdresa, lozinka){}

	~KaratePolaznik() {
		cout << crt << "DESTRUKTOR -> KaratePolaznik" << crt;
	}

    bool AddTehniku(Pojas pojas, Tehnika &tehnika) {

        // 1. Provjera da li postoji Polaganje za zadati pojas
        for (auto& polaganje : _polozeniPojasevi) {

            if (polaganje.GetPojas() == pojas) {
                
                // Provjera da li je tehnika ve? dodana
                for (auto tehnikaPostojeca : polaganje.GetTehnike()) {
                    if (strcmp(tehnikaPostojeca->GetNaziv(), tehnika.GetNaziv()) == 0)
                        return false; // Ve? postoji ta tehnika
                }

                // Ako nije, dodaj tehniku
                polaganje.GetTehnike().push_back(new Tehnika(tehnika));
                return true;
            }
        }

        // 2. Ako pojas još nije polagan:
        // Provjeri prethodni pojas (ako postoji) i da li ima najmanje 3 tehnike
        if (pojas != BIJELI) {
            Pojas prethodni = static_cast<Pojas>(pojas - 1);

            auto it = std::find_if(_polozeniPojasevi.begin(), _polozeniPojasevi.end(),
                [prethodni](Polaganje& p) { return p.GetPojas() == prethodni; });

            if (it == _polozeniPojasevi.end() || it->GetTehnike().size() < 3)
                return false; // Nema prethodni pojas ili nema 3 tehnike

            int i=0;
            for(i=0; i<_polozeniPojasevi.size(); i++){
                if(_polozeniPojasevi[i].GetPojas() == prethodni) break;
            }

            if(i==_polozeniPojasevi.size() || _polozeniPojasevi[i].GetTehnike().size() < 3)
                return false;
        }

        // 3. Dodaj novi Polaganje za pojas, sa tom tehnikom
        Polaganje novoPolaganje(pojas);
        novoPolaganje.GetTehnike().push_back(new Tehnika(tehnika));
        _polozeniPojasevi.push_back(novoPolaganje);
        return true;
    }
	
	friend ostream& operator<< (ostream& COUT, KaratePolaznik& obj) {
		COUT << obj.GetImePrezime() << " " << obj.GetEmail() << " " << obj.GetLozinka() << endl;
		for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++)
			COUT << obj._polozeniPojasevi[i];
		return COUT;
	}
	vector<Polaganje>& GetPolozeniPojasevi() { return _polozeniPojasevi; }
};



int main() {

	Datum   datum19062020(19, 6, 2020),
		datum20062020(20, 6, 2020),
		datum30062020(30, 6, 2020),
		datum05072020(5, 7, 2020),
		datum25062024(25, 6, 2024),
		datum25062025(25, 6, 2025);

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
	if (choku_zuki.AddOcjena(4, datum25062025))
		cout << "Ocjena evidentirana!" << endl;
	if (!choku_zuki.AddOcjena(5, datum25062024))
		cout << "Ocjena NIJE evidentirana!" << endl;

	/* ispisuje: naziv tehnike, ocjene (zajedno sa datumom) i prosjecnu ocjenu za tu tehniku
	   ukoliko tehnika nema niti jednu ocjenu prosjecna treba biti 0*/
	cout << choku_zuki << endl;

	if (ValidirajLozinku("john4Do*e"))
		cout << "OK" << crt;
	if (!ValidirajLozinku("john4Doe"))
		cout << "Specijalni znak?" << crt;
	if (!ValidirajLozinku("jo*4Da"))
		cout << "7 znakova?" << crt;
	if (!ValidirajLozinku("4jo-hnoe"))
		cout << "Veliko slovo?" << crt;
	if (ValidirajLozinku("@john2Doe"))
		cout << "OK" << crt;

	/*
	za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi:
	-   najmanje 7 znakova
	-   velika i mala slova
	-   najmanje jedan broj
	-   najmanje jedan specijalni znak
	za provjeru validnosti lozinke koristiti globalnu funkciju ValidirajLozinku, a unutar nje regex metode.
	validacija lozinke se vrsi unutar konstruktora klase Korisnik, a u slucaju da nije validna
	postaviti je na podrazumijevanu vrijednost: <VRIJEDNOST_NIJE_VALIDNA>
	*/

	Korisnik* jasmin = new KaratePolaznik("Jasmin Azemovic", "jasmin@karate.ba", "j@sm1N*");
	Korisnik* adel = new KaratePolaznik("Adel Handzic", "adel@edu.karate.ba", "4Ade1*H");
	Korisnik* emailNijeValidan = new KaratePolaznik("John Doe", "john.doe@google.com", "johndoe");

	/*
	sve tehnike na nivou jednog pojasa (ZUTI, ZELENI ... ) se evidentiraju unutar istog objekta tipa Polaganje,
	tom prilikom onemoguciti:
	- dodavanje istih (moraju biti identicne vrijednosti svih atributa) tehnika na nivou jednog pojasa,
	- dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5
	(onemoguciti dodavanje tehnike za NARANDZASTI ako ne postoji najmanje 3 tehnike za ZUTI pojas ili njihov prosjek nije veci od 3.5)
	funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/

	//doraditi klase na nacin da omoguce izvrsenje naredne linije koda
	KaratePolaznik* jasminPolaznik = dynamic_cast<KaratePolaznik*>(jasmin);

	if (jasminPolaznik != nullptr) {
		if (jasminPolaznik->AddTehniku(ZUTI, gyaku_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;

		//ne treba dodati kizami_zuki jer ne postoje evidentirane 3 tehnike za ZUTI pojas
		if (!jasminPolaznik->AddTehniku(NARANDZASTI, kizami_zuki))
			cout << "Tehnika NIJE uspjesno dodana!" << crt;
		if (jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;

		//ne treba dodati choku_zuki jer je vec dodana za zuti pojas
		if (!jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
			cout << "Tehnika NIJE uspjesno dodana!" << crt;

		//ispisuje sve dostupne podatke o karate polazniku
		cout << *jasminPolaznik << crt;

		//osigurati da se u narednim linijama poziva i destruktor klase KaratePolaznik
		delete jasmin;
		delete adel;
		delete emailNijeValidan;
	}

	return 0;
}