#include <iostream>
#include <new>
#include <stdexcept>

template<typename Tip> class Iterator;

template <typename Tip> class Lista {
public:
    Lista() = default;
    virtual ~Lista() = default; // virtualni destruktor je uvijek obavezan
    virtual int brojElemenata() const = 0;
    virtual Tip &trenutni() = 0; // ova metoda vraca trenutni po redu atribut koji se pamti, treba se koristiti i za citanje i za izmjenu
    virtual const Tip &trenutni() const = 0; // CONST VERZIJA
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0; // ove dvije metode trebaju izmjeniti stanje trenutnog,  vracaju false ako je element prvi ili zadnji
    virtual void pocetak() = 0;
    virtual void kraj() = 0;   // postavljaju trenutni na pocetak ili kraj liste
    virtual void obrisi() = 0; // treba da obrise trenutni i da trenutni postane sljedeci element po redu, ako se izbrise zadnji onda je trenutni prethodni 
    //ako je lista prazna, prethodnih 5 metoda bacaju izuzetak
    virtual void dodajIspred(const Tip &el) = 0;
    virtual void dodajIza(
    const Tip &el) = 0; // ove dvije metode dodaju element u odnosu na trenutnog, ali bez da se trenutni mijenja, ako je lista prazna onda novi element je prvi element
    virtual Tip &operator[](int i) = 0; // koristi se za citanje i pisanje, ako je i<=0 ili veci od broja elemenata, baca izuzetak
    virtual const Tip &operator[](int i) const = 0;
    virtual Lista<Tip> *DajKopiju() const = 0;
};

template <typename Tip> class DvostrukaLista : public Lista<Tip> {
    int velicina;
    
    struct Cvor {
        Tip element;
        Cvor *next;
        Cvor *prev;
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
  DvostrukaLista(): glava(nullptr), trenutni_pok(nullptr), rep(nullptr), velicina(0) {}

  ~DvostrukaLista() { Dislociraj(); }

  DvostrukaLista(const DvostrukaLista<Tip> &dlist) : glava(nullptr), rep(nullptr), trenutni_pok(nullptr), velicina(dlist.velicina) {
    try {
      if (dlist.glava == nullptr)
        return;

      glava = new Cvor{dlist.glava->element, nullptr};
      Cvor *temp_izvor = dlist.glava->next;
      Cvor *temp_cilj = glava;

      if (dlist.trenutni_pok == dlist.glava)
        trenutni_pok = glava;

      while (temp_izvor) {
        temp_cilj->next = new Cvor{temp_izvor->element, nullptr, nullptr};
        auto temp_cilj_prev = temp_cilj;
        temp_cilj = temp_cilj->next;
        temp_cilj->prev = temp_cilj_prev;

        if (temp_izvor == dlist.trenutni_pok)
          trenutni_pok = temp_cilj;

        temp_izvor = temp_izvor->next;
      }
      rep = temp_cilj;

    } catch (...) {
      Dislociraj();
      throw;
    }
  }

  DvostrukaLista<Tip> &operator=(DvostrukaLista<Tip> DL) {
    std::swap(DL.glava, glava);
    std::swap(DL.trenutni_pok, trenutni_pok);
    std::swap(DL.rep, rep);
    std::swap(DL.velicina, velicina);
    return *this;
  }

  int brojElemenata() const override { return velicina; }

  bool prethodni() override {
        jelPrazan();
        if (trenutni_pok == glava)
            return false;
        trenutni_pok=trenutni_pok->prev;
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
    } 
    else if (trenutni_pok == rep) {
      trenutni_pok=trenutni_pok->prev;
      delete rep;
      rep = nullptr;
      rep = trenutni_pok;
    } 
    else if (trenutni_pok == glava) {
      trenutni_pok = trenutni_pok->next;
      delete glava;
      glava = nullptr;
      glava = trenutni_pok;
    } 
    else {
      Cvor *temp = glava;
      while (temp->next != trenutni_pok)
        temp = temp->next;
      trenutni_pok = trenutni_pok->next;
      delete temp->next;
      temp->next = nullptr;
      temp->next = trenutni_pok;
      trenutni_pok->prev=temp;
    }
    velicina--;
    if (velicina == 0)
      rep = nullptr;
  }

  void dodajIspred(const Tip &el) override {
        try {
            if (velicina == 0)
                glava = trenutni_pok = rep = new Cvor{el, nullptr, nullptr};
            else if (trenutni_pok == glava) {
                Cvor *temp = new Cvor{el, trenutni_pok, nullptr};
                glava->prev = temp;
                glava = temp;
            } 
            else {
                Cvor *temp = new Cvor{el, trenutni_pok, trenutni_pok->prev};
                trenutni_pok->prev->next = temp;
                trenutni_pok->prev = temp;  
            }
            velicina++;
        } 
        catch (std::bad_alloc e) {
            Dislociraj();
            throw e;
        }
    }

    void dodajIza(const Tip &el) override {
        try {
            if (velicina == 0)
                glava = trenutni_pok = rep = new Cvor{el, nullptr, nullptr};
            else {
                Cvor* temp = new Cvor{el, trenutni_pok->next, trenutni_pok};
                trenutni_pok->next = temp;

                if (temp->next != nullptr) temp->next->prev = temp;
                else rep = temp;
            }
            velicina++;
        } 
        catch (std::bad_alloc e) {
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

  friend std::ostream &operator<<(std::ostream &tok, const DvostrukaLista &lista) {
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
    return new DvostrukaLista<Tip>(*this);
  }

  friend class Iterator<Tip>;
};


template<typename Tip> class Iterator{
    const DvostrukaLista<Tip> *lista; 
    typename DvostrukaLista<Tip>::Cvor *trenutniLista;
public:
    Iterator(const DvostrukaLista<Tip> &l) : lista(&l), trenutniLista(l.glava) {}

    Tip& trenutni() {
        if(!trenutniLista) throw std::domain_error("Prazna lista!");
        return trenutniLista->element;
    }

    bool prethodni() {
        if (!trenutniLista || trenutniLista == lista->glava) return false;
        trenutniLista = trenutniLista->prev;
        return true;
    }

    bool sljedeci() {
        if (!trenutniLista || trenutniLista == lista->rep) return false;
        trenutniLista = trenutniLista->next;
        return true;
    }

    void pocetak() {trenutniLista=lista->glava;}
    void kraj() {trenutniLista=lista->rep;}
};

template <typename Tip> Tip dajMaksimum(const Lista<Tip>& n){           //funkcija je ovdje primila objekat Listu, bez obzira sta je poslano
    if(n.brojElemenata()==0) throw std::domain_error("Prazna lista!");

    //Iterator<Tip> it(n);  ova inicijalizacija je pogresna jer iterator ocekuje dvostruku listu, a n je ovdje downgradean u Listu
    //umjesto toga, koristiti dynamic cast
    Iterator <Tip> it(dynamic_cast<const DvostrukaLista<Tip>&>(n));        

    Tip max = it.trenutni();

    while(it.sljedeci()) 
        if(it.trenutni() > max) max = it.trenutni();

    return max;
}

void testBrojElemenataDL() {
  DvostrukaLista<int> lista;

  std::cout << "Test brojElemenata (prazno) " << (lista.brojElemenata() == 0 ? "prosao" : "pao") << std::endl;

  lista.dodajIza(10);
  std::cout << "Test brojElemenata (jedan element) " << (lista.brojElemenata() == 1 ? "prosao" : "pao") << std::endl;

  lista.dodajIza(20);
  std::cout << "Test brojElemenata (dva elementa) " << (lista.brojElemenata() == 2 ? "prosao" : "pao") << std::endl;
}

void testTrenutniDL() {
  DvostrukaLista<int> lista;
  lista.dodajIza(30);
  lista.dodajIspred(40);
  lista.dodajIza(50);

  std::cout << lista << std::endl;

  lista.pocetak();
  std::cout << "Test trenutni (prvi element) " << (lista.trenutni() == 40 ? "prosao" : "pao") << std::endl;

  lista.sljedeci();
  std::cout << "Test trenutni (drugi element) " << (lista.trenutni() == 30 ? "prosao" : "pao") << std::endl;

  lista.kraj();
  std::cout << "Test trenutni (zadnji element) " << (lista.trenutni() == 50 ? "prosao" : "pao") << std::endl;

  lista.prethodni();
  std::cout << "Test trenutni (predzadnji element) " << (lista.trenutni() == 30 ? "prosao" : "pao") << std::endl;
}

void testDodajIspredDL() {
  DvostrukaLista<int> lista;
  lista.dodajIza(10);
  lista.dodajIspred(5);

  lista.pocetak();
  std::cout << "Test dodajIspred " << (lista.trenutni() == 5 ? "prosao" : "pao") << std::endl;
}

void testDodajIzaDL() {
  DvostrukaLista<int> lista;
  lista.dodajIza(10);
  lista.dodajIza(20);

  lista.kraj();
  std::cout << "Test dodajIza " << (lista.trenutni() == 20 ? "prosao" : "pao") << std::endl;
}

void testObrisiDL() {
  DvostrukaLista<int> lista;
  lista.dodajIza(10);
  lista.dodajIza(20);
  lista.dodajIza(30);

  lista.kraj();
  lista.obrisi();

  std::cout << "Test obrisi (trenutni) " << (lista.trenutni() == 30 ? "prosao" : "pao") << std::endl;

  std::cout << "Test obrisi (broj elemenata) " << (lista.brojElemenata() == 2 ? "prosao" : "pao") << std::endl;
}

void testOperatorIndexDL() {
  DvostrukaLista<int> lista;
  lista.dodajIza(10);
  lista.dodajIza(20);

  std::cout << "Test operator[] (index 0) " << (lista[0] == 10 ? "prosao" : "pao") << std::endl;
  std::cout << "Test operator[] (index 1) " << (lista[1] == 20 ? "prosao" : "pao") << std::endl;

  try {
    lista[2];
    std::cout << "Test operator[] (out of bounds) pao" << std::endl;
  } 
  catch (std::domain_error &) {
    std::cout << "Test operator[] (out of bounds) prosao" << std::endl;
  }
}

void testDajMaksimum() {
    DvostrukaLista<int> lista;
    lista.dodajIza(5);
    lista.dodajIza(20);
    lista.dodajIza(10);
    lista.dodajIza(30);

    try {
        int maxElement = dajMaksimum(lista);
        std::cout << "Maksimum je: " << maxElement << std::endl;

        if (maxElement == 30)
            std::cout << "Test maksimum prosao" << std::endl;
        else
            std::cout << "Netacan maksimum, test pao" << std::endl;
    } 
    catch (const std::exception &e) {
        std::cerr << "Belaj: " << e.what() << std::endl;
    }
}

using namespace std;

int main(){
    testBrojElemenataDL();
    testTrenutniDL();
    testDodajIspredDL();
    testDodajIzaDL();
    testObrisiDL();
    testOperatorIndexDL();
    testDajMaksimum();
}