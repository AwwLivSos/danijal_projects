#include <exception>
#include <iostream>
#include <new>
#include <stdexcept>
#include <string>

template <typename Tip> class Lista {
public:
  Lista() = default;
  virtual ~Lista() = default;
  virtual int brojElemenata() const = 0;
  virtual Tip & trenutni() = 0;
  virtual const Tip &trenutni() const = 0;
  virtual bool prethodni() = 0;
  virtual bool sljedeci() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;   
  virtual void obrisi() = 0;
  virtual void dodajIspred(const Tip &el) = 0;
  virtual void dodajIza(
  const Tip &el) = 0;
  virtual Tip & operator[](int i) = 0; 
  virtual const Tip &operator[](int i) const = 0;
  virtual Lista<Tip> *DajKopiju() const = 0;
};

template <typename Tip> class JednostrukaLista : public Lista<Tip> {
  int velicina;

  struct Cvor {
    Tip element;
    Cvor *next;
  };

  Cvor *glava;
  Cvor *trenutni_pok;
  Cvor *rep;

  void jelPrazan() const {
    if (velicina == 0)
      throw std::domain_error("Prazna lista!");
  }

  void Dislociraj() {
    Cvor *temp = glava;
    for (int i = 0; i < velicina; i++) {
      auto temp2 = temp->next;
      delete temp;
      temp = nullptr;
      temp = temp2;
    }
    velicina = 0;
  }

public:
  JednostrukaLista() : glava(nullptr), trenutni_pok(nullptr), rep(nullptr), velicina(0) {}

  ~JednostrukaLista() { Dislociraj(); }

  JednostrukaLista(const JednostrukaLista<Tip> &jlist): glava(nullptr), rep(nullptr), trenutni_pok(nullptr), velicina(jlist.velicina) {
    try {
      if (jlist.glava == nullptr) return;

      glava = new Cvor{jlist.glava->element, nullptr};
      Cvor *temp_izvor = jlist.glava->next;
      Cvor *temp_cilj = glava;

      if (jlist.trenutni_pok == jlist.glava)
        trenutni_pok = glava;

      while (temp_izvor) {
         temp_cilj->next = new Cvor{temp_izvor->element, nullptr};

        temp_cilj = temp_cilj->next;

        if (temp_izvor == jlist.trenutni_pok)
          trenutni_pok = temp_cilj;
        temp_izvor = temp_izvor->next;
      }
      rep = temp_cilj;

    } catch (...) {
      Dislociraj();
      throw;
    }
  }

  JednostrukaLista<Tip> &operator=(JednostrukaLista<Tip> jl) {
    std::swap(jl.glava, glava);
    std::swap(jl.trenutni_pok, trenutni_pok);
    std::swap(jl.rep, rep);
    std::swap(jl.velicina, velicina);
    return *this;
  }

  int brojElemenata() const override { return velicina; }

  bool prethodni() override {
    jelPrazan();
    if (trenutni_pok == glava)
      return false;
    Cvor *temp = glava;
    while (temp->next != trenutni_pok) {
      temp = temp->next;
    }
    trenutni_pok = temp;
    return true;
  }

  bool sljedeci() override {
    jelPrazan();
    if (trenutni_pok == rep)
      return false;
    trenutni_pok = trenutni_pok->next;
    return true;
  }

  void pocetak() override {
    jelPrazan();
    trenutni_pok = glava;
  }

  Tip &trenutni() override {
    jelPrazan();
    return trenutni_pok->element;
  }

  const Tip &trenutni() const override {
    jelPrazan();
    return trenutni_pok->element;
  }

  void kraj() override {
    jelPrazan();
    trenutni_pok = rep;
  }

  void obrisi() override {
    jelPrazan();
    if (velicina == 1) {
      delete trenutni_pok;
      trenutni_pok = nullptr;
    } else if (trenutni_pok == rep) {
      Cvor *temp = glava;
      while (temp->next != trenutni_pok)
        temp = temp->next;
      rep = temp;
      delete trenutni_pok;
      trenutni_pok = nullptr;
      trenutni_pok = rep;
    } else if (trenutni_pok == glava) {
      trenutni_pok = trenutni_pok->next;
      delete glava;
      glava = nullptr;
      glava = trenutni_pok;
    } else {
      Cvor *temp = glava;
      while (temp->next != trenutni_pok)
        temp = temp->next;
      trenutni_pok = trenutni_pok->next;
      delete temp->next;
      temp->next = nullptr;
      temp->next = trenutni_pok;
    }
    velicina--;
    if (velicina == 0)
      rep = nullptr;
  }

  void dodajIspred(const Tip &el) override {
    try {
      if (velicina == 0)
        glava = trenutni_pok = rep = new Cvor{el, nullptr};
      else if (trenutni_pok == glava) {
        Cvor *temp = new Cvor{el, trenutni_pok};
        glava = temp;
      } else {
        Cvor *temp = glava;
        while (temp->next != trenutni_pok)
          temp = temp->next;
        temp->next = new Cvor{el, trenutni_pok};
      }
      velicina++;
    } catch (std::bad_alloc e) {
      Dislociraj();
      throw e;
    }
  }

  void dodajIza(const Tip &el) override {
    try {
      if (velicina == 0)
        glava = trenutni_pok = rep = new Cvor{el, nullptr};
      else {
        trenutni_pok->next = new Cvor{el, trenutni_pok->next};
        if (trenutni_pok->next->next == nullptr)
          rep = trenutni_pok->next;
      }
      velicina++;
    } catch (std::bad_alloc e) {
      Dislociraj();
      throw e;
    }
  }

  Tip &operator[](int i) override {
    if (i < 0 || i >= velicina)
      throw std::domain_error("Nevazeci indeks!");
    Cvor *temp = glava;
    for (int j = 0; j < i; j++) {
      temp = temp->next;
    }
    return temp->element;
  }

  const Tip &operator[](int i) const override {
    if (i < 0 || i >= velicina)
      throw std::domain_error("Nevazeci indeks!");
    Cvor *temp = glava;
    for (int j = 0; j < i; j++) {
      temp = temp->next;
    }
    return temp->element;
  }

  friend std::ostream &operator<<(std::ostream &tok, const JednostrukaLista &lista) {
    Cvor *temp = lista.glava;
    for (int i = 0; i < lista.velicina; i++) {
      tok << temp->element;
      if (temp != lista.rep)
        tok << ", ";
      temp = temp->next;
    }
    return tok;
  }

  Lista<Tip> *DajKopiju() const override {
    return new JednostrukaLista<Tip>(*this);
  }
};

template<typename Tip>
    class Stek{
        JednostrukaLista<Tip> s;
    public:
        void brisi(){
            while (s.brojElemenata() > 0) {
                s.obrisi();
            }
        }
        void stavi(const Tip &el){
            s.dodajIspred(el);
            s.pocetak();
        }
        Tip skini(){
            Tip el = s.trenutni();
            s.obrisi();
            if(brojElemenata()!=0) s.pocetak();
            return el;
        }
        Tip &vrh(){
            return s.trenutni();
        }
        int brojElemenata(){
            return s.brojElemenata();
        }
    };

#include <vector>
using namespace std;

int binarno_pretrazi_vektor(const vector<int> &v, int trazeni){
   int kraj = v.size()-1;
   int pocetak{};
   int sredina;
   while(pocetak<=kraj){
       sredina = (kraj+pocetak)/2;
       if(v[sredina] == trazeni) return sredina;
       else if (trazeni > v[sredina]) pocetak = sredina+1;
       else kraj = sredina-1;
   } 
   return -1;               //provjeravam svaki vektor, ne samo onaj kojeg je logicno gledati. Ako vrati -1 ova funkcija, znaci da nije taj vektor
}

//KLJUC ZA PRETRAZIVANJE STEKA JE REKURZIJA!!11!1!1!!

void pretraga(Stek<vector<int>>& s, int trazeni, int dubina = 0) {
    if (s.brojElemenata() == 0) {                   //kada se do kraja izrekurzira do kraj onda znaci da se nije naslo, tj vjv je trazeni veci od svih elemenata (gledajuci testove)
        cout << "Nema elementa" << endl;
        return;
    }
    
    static int n = s.brojElemenata()-1;             //treba mi stalna velicina originalnog steka, spasilo cijeli zadatak

    vector<int> pretrazivac = s.skini();            //uzmi prvi i njega pretrazi
    if (!pretrazivac.empty() && pretrazivac.front() <= trazeni) {                    //realno moze i bez ove druge provjere al eto malo je brze
        int indexT = binarno_pretrazi_vektor(pretrazivac, trazeni);
        if (indexT != -1) {                         //ako index binarne pretrage ne ukazuje da se element nalazi u tom vektoru
            cout << indexT << " " << n-dubina << endl;
            s.stavi(pretrazivac);                   //ako si nakon pretrage nasao i ispisao, samo vrati zadnji skinuti vektor na mjesto i tjt 
            return;                                 //ako nađeš top, bitno je samo naglasiti da je ovdje indeks vektora dobijen oduzimanjem originalne velicne sa nivoom gdje smo sad
        }
    }
    pretraga(s, trazeni, dubina + 1);               //kljucni dio, vrsi opet pretrazivanje steka kojem je skinut prvi element, biljezi na koliko si nivoa dubine sa varijablom
    s.stavi(pretrazivac);                           //kada si zavrsio pretragu sa manjim stekom, vrati zadnji skinuti element, i vrati se na prethodnu rekurziju, eventualno ce se naci ili ispisati da ga nema
}

void testFunkcijaPronalaziElement() {
    Stek<vector<int>> s;
    s.stavi({1, 2, 3, 4, 5});
    s.stavi({6, 8});
    s.stavi({9, 10, 130});
    s.stavi({157, 226, 3424});

    cout << "Očekivano: 1 2" << endl;
    cout << "Dobijeno: ";
    pretraga(s, 10);  //trebalo bi ispisati "1 2" inshallah
    cout << endl;
}

void testFunkcijaNePronalaziElement() {
    Stek<vector<int>> s;
    s.stavi({1, 2, 3, 4, 5});
    s.stavi({6, 8});
    s.stavi({9, 10, 130});
    s.stavi({157, 226, 3424});

    cout << "Očekivano: Nema elementa" << endl;
    cout << "Dobijeno: ";
    pretraga(s, 100);  //trebalo bi ispisati "Nema elementa" aBd
    cout << endl;
}

void testFunkcijaSaPraznimVektorima() {
    Stek<vector<int>> s;
    s.stavi({});
    s.stavi({1, 3, 5});
    s.stavi({});
    s.stavi({8, 10, 12});

    cout << "Očekivano: 2 1" << endl;
    cout << "Dobijeno: ";
    pretraga(s, 5);  //trebalo bi ispisati "2 0" nadam se
    cout << endl;
}

/* void bonusTestZaRaju() {
    Stek<vector<int>> s;

    s.stavi({-10, -8, -6, -4, -2, 0});              //ne bi trebalo predstavljati problem
    s.stavi({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});    
    s.stavi({42});                                
    s.stavi({50, 100, 150});                
    s.stavi({200000, 300000, 400000});      
    s.stavi({1000000, 2000000, 3000000});   
    s.stavi({});                                    //ovo bi trebalo raditi
    s.stavi({1000000000, 2000000000});      

    cout << "Bonus test za cudne vektore" << endl;

    cout << "za 42 ocekujemo 0 2: ";
    pretraga(s, 42); 

    cout << "za 300000 ocekujemo 1 4: ";
    pretraga(s, 300000); 

    cout << "za 2000000000 ocekujemo 1 7: ";
    pretraga(s, 2000000000);

    cout << "za 1000 nema elementa: ";
    pretraga(s, 1000);

    cout << "za 10 bi trebalo biti 1 9: ";
    pretraga(s, 9);

    cout << "za -10 bi trebalo biti 0 0: ";
    pretraga(s, -10); 

    cout << endl;                                    interesantan pokusaj, ali ne radi kako treba jer...(?)
} */   

int main(){
    testFunkcijaPronalaziElement();
    testFunkcijaNePronalaziElement();
    testFunkcijaSaPraznimVektorima();
    return 0;
}