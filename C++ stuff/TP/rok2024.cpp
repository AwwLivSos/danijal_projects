#include <iostream>
#include <utility>
#include <string>
#include <cstring>
#include <regex>
#include <vector>
#include <exception>
#include <stdexcept>
#include <thread>

using namespace std;

const char* PORUKA = "\n-------------------------------------------------------------------------------\n";
const char* crt = "\n-------------------------------------------\n";

enum Drzava {
	ENGLESKA, SPANIJA, HOLANDIJA, FRANCUSKA, BOSNA_I_HERCEGOVINA
};

char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr) return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy(temp, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}

template<class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max];
	T2* _elementi2[max];
	int* _trenutno;

public:
	Kolekcija() {
        _trenutno = new int(0);
	}
    
	~Kolekcija() {
		for (size_t i = 0; i < *_trenutno; i++)
		{
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

	T1& getElement1(int lokacija)const {
		return *_elementi1[lokacija];
	}

	T2& getElement2(int lokacija)const {
		return *_elementi2[lokacija];
	}

    bool AddElement(const T1 &obj1, const T2 &obj2){
        if(getTrenutno() == max) throw domain_error("Dostignut maksimalni broj elemenata");

        _elementi1[getTrenutno()] = new T1(obj1);
        _elementi2[(*_trenutno)++] = new T2(obj2);
        return true;
    }

    Kolekcija &InsertAt(int index, const T1 &obj1, const T2 &obj2){
        if(index > getTrenutno() || index<0 || getTrenutno() == max) 
            throw std::domain_error("Nemoguc zahtjev!");

        for(int i = getTrenutno(); i>index; i--){
            _elementi1[i] = _elementi1[i-1];
            _elementi2[i] = _elementi2[i-1];
        }

        _elementi1[index] = new T1(obj1);
        _elementi2[index] = new T2(obj2);

        (*_trenutno)++;

        return *this;
    }

    Kolekcija* RemoveRange(int OD, int DO) {
        if (OD > DO || OD < 0 || DO >= getTrenutno())
            throw std::out_of_range("Nevazeci indeks!");

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

	int getTrenutno() const { return *_trenutno; }

	friend ostream& operator<< (ostream& COUT, Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
};

std::string GenerisiID(int id){
    std::string prefix = "ID#";

    if(id<10) prefix+="000-" + std::to_string(id);
    else if (id<100) prefix+="00-" + std::to_string(id);
    else if (id<1000) prefix+="0-" + std::to_string(id);
    else prefix += std::to_string(id);

    return prefix;
}

bool ValidirajID(const string &s){
    string pattern1 = "ID#[0-9]{4}";
    string pattern2 = "ID#[0-9]{3}-{1}";
    string pattern3 = "ID#[0-9]{2}-[0-9]{2}";
    string pattern4 = "ID#[0-9]{1}-[0-9]{3}";

    std::regex rule1(pattern1);
    std::regex rule2(pattern2);
    std::regex rule3(pattern3);
    std::regex rule4(pattern4);

    if(regex_match(s, rule1) || regex_match(s, rule2) || regex_match(s, rule3) || regex_match(s, rule4))
        return true;
    return false;
}

bool ValidacijaMaila(const string &email){
    string pattern = "\\w+[._]?\\w+@(edu.)?(fit | etf | mf).ba";
    regex rule(pattern);

    return regex_match(email, rule);
}

class Vrijeme {
	int* _sat, * _minuta, * _sekunda;
public:
	Vrijeme(int sat = 10, int minuta = 0, int sekunda = 0) {
		_sat = new int(sat);
		_minuta = new int(minuta);
		_sekunda = new int(sekunda);
	}

	~Vrijeme() {
		delete _sat; _sat = nullptr;
		delete _minuta; _minuta = nullptr;
		delete _sekunda; _sekunda = nullptr;
	}

    Vrijeme(const Vrijeme &v){
        _sat = new int(*v._sat);
        _minuta = new int(*v._minuta);
        _sekunda = new int (*v._sekunda);
    }

    Vrijeme &operator = (const Vrijeme &v){
        if(this != &v){
            delete _sat; _sat = nullptr;
            delete _minuta; _minuta = nullptr;
            delete _sekunda; _sekunda = nullptr;

            _sat = new int(*v._sat);
            _minuta = new int(*v._minuta);
            _sekunda = new int (*v._sekunda);
        }

        return *this;
    }

	friend ostream& operator<< (ostream& COUT, const Vrijeme& obj) {
		COUT << *obj._sat << ":" << *obj._minuta << ":" << *obj._sekunda;
		return COUT;
	}
};

class Pogodak {
	Vrijeme _vrijemePogotka;
	char* _napomena;
public:
	Pogodak(Vrijeme vrijeme, const char* napomena) : _vrijemePogotka(vrijeme){
		_napomena = GetNizKaraktera(napomena);
	}

	~Pogodak(){
		delete[] _napomena; _napomena = nullptr;
	}

    Pogodak(const Pogodak &p) : _vrijemePogotka(p._vrijemePogotka), _napomena(GetNizKaraktera(p._napomena)) {}

    Pogodak &operator= (const Pogodak &p){
        if(this != &p){
            _vrijemePogotka = p._vrijemePogotka;
            delete[] _napomena; _napomena = nullptr;
            _napomena = GetNizKaraktera(p._napomena);
        }
        return *this;
    }

	Vrijeme GetVrijemePogotka() { return _vrijemePogotka; }
	char* GetNapomena() { return _napomena; }

	friend ostream& operator<< (ostream& COUT, const Pogodak& obj) {
		COUT << obj._vrijemePogotka << " -> " << obj._napomena;
		return COUT;
	}
};

class Igrac {
	static int _id;
	char* _ID; // za inicijalizaciju _ID-a iskoristiti funkciju GenerisiID i vrijednost statickog clana _id
	char* _imePrezime;
	vector<Pogodak> _pogoci;

public:

	Igrac(const char* imePrezime){
		_imePrezime = GetNizKaraktera(imePrezime);

        _ID = GetNizKaraktera(GenerisiID(_id).c_str());
        _id++;
	}

	~Igrac(){
		delete[]_ID; _ID = nullptr;
		delete[]_imePrezime; _imePrezime = nullptr;
	}

    Igrac(const Igrac &drugi){
        _ID = GetNizKaraktera(drugi.GetID());
        _imePrezime = GetNizKaraktera(drugi.GetImePrezime());

        _pogoci = drugi.GetPogoci();
    }

    Igrac &operator= (const Igrac &drugi){
        if(this != &drugi){
            delete[]_ID; _ID = nullptr;
		    delete[]_imePrezime; _imePrezime = nullptr;

            _ID = GetNizKaraktera(drugi.GetID());
            _imePrezime = GetNizKaraktera(drugi.GetImePrezime());
            _pogoci = drugi.GetPogoci();
        }

        return *this;
    }

	char* GetImePrezime() const { return _imePrezime; }
	char* GetID() const { return _ID; }
	vector<Pogodak> GetPogoci() const { return _pogoci; }

	friend ostream& operator<< (ostream& COUT, Igrac& obj) {
		COUT << *obj._ID << " -> " << obj._imePrezime;
		for (size_t i = 0; i < obj._pogoci.size(); i++)
			cout << obj._pogoci[i] << endl;
		return COUT;
	}
};

class Reprezentacija {
	Drzava _drzava;
	vector<Igrac> _igraci;

    void sendMail(
		const Igrac& igrac,
		const Reprezentacija& reprezentacija1,
		const Reprezentacija& reprezentacija2
	) const {
		std::thread emailThread{
			[&]() {
				// DON'T FORGET TO UNCOMMENT THIS LINE BELOW
				std::this_thread::sleep_for(2s);
				std::cout << "\nTo: " << igrac.getID() << "@euro2024.com\n";
				std::cout << "From: info@euro2024.com\n";
				std::cout << "Subject: Informacija\n";
				std::cout << "Postovani,\n";
				std::cout << "U " << igrac.getZadnjiPogodak().getVrijemePogotka() << " sati igrac " << igrac.getImePrezime();
				std::cout << " je zabiljezio svoj " << igrac.getBrojPogodaka() << '\n';
				std::cout << "pogodak na ovoj utakmici.\n";
				std::cout << "Trenutni rezultat je:\n";
				std::cout << reprezentacija1.getDrzava() << ' ' << reprezentacija1.getBrojPogodaka();
				std::cout << " : ";
				std::cout << reprezentacija2.getDrzava() << ' ' << reprezentacija2.getBrojPogodaka();
				std::cout << "\nPuno srece u nastavku susreta.\n\n";
			}
		};

		emailThread.join();
	}

public:

	Reprezentacija(Drzava drzava) {
		_drzava = drzava;
	}

    ~Reprezentacija()=default;

    Reprezentacija(const Reprezentacija &r) : _drzava(r._drzava), _igraci(r._igraci){}
    Reprezentacija &operator=(const Reprezentacija &r) = default;

	Drzava GetDrzava() const { return _drzava; }
	vector<Igrac> GetIgraci() const { return _igraci; }

    void AddIgrac(const Igrac &igrac){
        for(const auto &igraci : _igraci) 
            if(strcmp(igrac.GetID(), igraci.GetID()) == 0) 
                throw domain_error("Igrac vec postoji!");

        _igraci.push_back(igrac);
    }

    friend bool operator==(const Reprezentacija &r1, const Reprezentacija &r2){
        return r1.GetDrzava() == r2.GetDrzava();
    }
};

class Prventstvo {
	Kolekcija<Reprezentacija, Reprezentacija, 20> _utakmice;
public:
	Kolekcija<Reprezentacija, Reprezentacija, 20>& GetUtakmice() {
		return _utakmice;
	}

    void AddUtakmicu(Reprezentacija r1, Reprezentacija r2){

        for(int i=0; i<_utakmice.getTrenutno(); i++){
            bool uslov1 = r1 == _utakmice.getElement1(i) && r2 == _utakmice.getElement2(i);
            bool uslov2 = r2 == _utakmice.getElement1(i) && r1 == _utakmice.getElement2(i);
            bool uslov3 = r1 == r2;

            if(uslov1 || uslov2 || uslov3) throw domain_error("Utakmica je vec odrzana!");
        }

        _utakmice.AddElement(r1, r2);
    }

    bool AddPogodak(Drzava zabila, Drzava primila, string identifikator, Pogodak pogodak){

    }
};

int Igrac::_id = 1;

int main() {

	/* cout << GenerisiID(3) << endl;//treba vratiti ID#000-3
	cout << GenerisiID(14) << endl;//treba vratiti ID#00-14
	cout << GenerisiID(156) << endl;//treba vratiti ID#0-156
	cout << GenerisiID(1798) << endl;//treba vratiti ID#1798

    cout<<endl<<endl;

	int kolekcijaTestSize = 9;
	Kolekcija<int, int, 10> kolekcija1;

	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);//dodaje vrijednosti u kolekciju
        
	cout << kolekcija1 << kolekcija1.getTrenutno()<<endl<<endl;

    Kolekcija<int, int, 10> kolekcija2 = kolekcija1.InsertAt(0, 10, 10);
	cout << kolekcija2 << crt<<endl<<endl;


	Kolekcija<int, int, 10>* kolekcija3 = kolekcija1.RemoveRange(1, 3);
	cout << *kolekcija3 << endl;
	cout << kolekcija1 << crt;
	
	kolekcija1 = *kolekcija3;
	cout << kolekcija1;
 */
    Vrijeme
		prviPogodak201633(20, 16, 33),
		drugiPogodak202319(20, 23, 19),
		treciPogodak205108(20, 51, 8),
		cetvrtiPogodak210654(21, 6, 54);

    Pogodak prviPogodak(prviPogodak201633, "podaci o prvom pogotku"),
		drugiPogodak(drugiPogodak202319, "podaci o drugom pogotku"),
		treciPogodak(treciPogodak205108, "podaci o trecem pogotku"),
		cetvrtiPogodak(cetvrtiPogodak210654, "podaci o cetvrtom pogotku");

    Igrac denis("Denis Music"), jasmin("Jasmin Azemovic"), goran("Goran Skondric"), adil("Adil Joldic");

	/* if (strcmp(denis.GetID(), "ID#000-1") == 0 && strcmp(jasmin.GetID(), "ID#000-2") == 0)
		cout << "ID se uspjesno generise!" << endl; */

    Reprezentacija BIH(BOSNA_I_HERCEGOVINA), ENG(ENGLESKA);
	BIH.AddIgrac(denis);
	BIH.AddIgrac(jasmin);
	ENG.AddIgrac(goran);
	ENG.AddIgrac(adil);
	/* try
	{
		//onemoguciti dodavanje istih igraca - provjeravati ID, baciti izuzetak
		BIH.AddIgrac(denis);
	}
	catch (exception& obj)
	{
		cout << obj.what();
	} */

    Prventstvo euro2024;

	euro2024.AddUtakmicu(BIH, ENG);
	try
	{
		//onemoguciti ponovne susrete drzave tokom istog prvenstva, baciti izuzetak
		euro2024.AddUtakmicu(BIH, ENG);
	}
	catch (exception& obj)
	{
		cout << obj.what();
	}

    if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "ID#000-1", prviPogodak))
		cout << "Pogodak uspjesno dodat" << endl;
	//onemoguciti dodavanje istih pogodaka
	if (!euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Denis Music", prviPogodak))
		cout << "Pogodak NIJE uspjesno dodat" << endl;
	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "ID#000-2", drugiPogodak))
		cout << "Pogodak uspjesno dodat" << endl;
	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Jasmin Azemovic", treciPogodak))
		cout << "Pogodak uspjesno dodat" << endl;
	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Goran Skondric", cetvrtiPogodak))
		cout << "Pogodak uspjesno dodat" << endl; 
	//nakon svakog evidentiranog pogotka, svim igracima te utakmice (pod pretpostavkom da su validne email adrese sa ID - ovima igraca),
	//u zasebnom thread-u, poslati email, u razmaku od 2 sekunde, sa sljedecim sadrzajem :
	

	/* To: ID#000-1@euro2024.com
	From: info@euro2024.com
	Subject: Informacija
	Postovani,
	U 20:35:16 sati igrac Jasmin Azemovic je zabiljezio svoj 1. pogodak na ovoj utakmici.
	Trenutni rezultat je:
	BOSNA_I_HERCEGOVINA 2 : 0 ENGLESKA
	Puno srece u nastavku susreta.
	Neka bolji tim pobijedi. */

	
	//ispisuje detaljnije informacije o susretu, kako je navedeno u narednom ispisu
	
    
    //cout << euro2024;
	
    
    /*
	-------------------------------------------
	BOSNA_I_HERCEGOVINA 3 : 1 ENGLESKA
	-------------------------------------------
	Denis Music Goran Skondric
	Jasmin Azemovic
	Jasmin Azemovic
	-------------------------------------------
	*/
	//vraca sve igrace koji su na takmicenju postigli pogodak u vremenu koje se nalazi izmedju proslijedjenih vrijednosti
	
    /* vector<Igrac*> igraci = euro2024(Vrijeme(20, 15, 15), Vrijeme(20, 24, 15));
	for (size_t i = 0; i < igraci.size(); i++)
		cout << igraci[i]->GetImePrezime() << endl; */

    return 0;
}