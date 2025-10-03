#include <iostream>
#include <new>
#include <stdexcept>

template <typename Tip> class Lista {
public:
  Lista() = default;
  virtual ~Lista() = default; // virtualni destruktor je uvijek obavezan
  virtual int brojElemenata() const = 0;
  virtual Tip &
  trenutni() = 0; // ova metoda vraca trenutni po redu atribut koji se pamti,
                  // treba se koristiti i za citanje i za izmjenu
  virtual const Tip &trenutni() const = 0; // CONST VERZIJA
  virtual bool prethodni() = 0;
  virtual bool
  sljedeci() = 0; // ove dvije metode trebaju izmjeniti stanje trenutnog,
                  // vracaju false ako je element prvi ili zadnji
  virtual void pocetak() = 0;
  virtual void kraj() = 0;   // postavljaju trenutni na pocetak ili kraj liste
  virtual void obrisi() = 0; // treba da obrise trenutni i da trenutni postane
                             // sljedeci element po redu, ako se izbrise zadnji
                             // onda je trenutni prethodni
  // ako je lista prazna, prethodnih 5 metoda bacaju izuzetak
  virtual void dodajIspred(const Tip &el) = 0;
  virtual void dodajIza(
      const Tip &el) = 0; // ove dvije metode dodaju element u odnosu na
                          // trenutnog, ali bez da se trenutni mijenja, ako je
                          // lista prazna onda novi element je prvi element
  virtual Tip &
  operator[](int i) = 0; // koristi se za citanje i pisanje, ako je i<=0 ili
                         // veci od broja elemenata, baca izuzetak
  virtual const Tip &operator[](int i) const = 0;
  virtual Lista<Tip> *DajKopiju() const = 0;
};

template <typename Tip> class NizLista : public Lista<Tip> {
  int velicina;
  int kapacitet;
  int trenutni_index;
  Tip **element;

  void prosiri() {
    Tip **novi = new Tip *[int(kapacitet * 2)];
    for (int i = 0; i < velicina; i++) {
      novi[i] = element[i];
    }
    delete[] element;
    element = novi;
    kapacitet *= 2;
  }

  void suzi() {
    Tip **novi = new Tip *[int(kapacitet / 2)];
    for (int i = 0; i < velicina; i++) {
      novi[i] = element[i];
    }
    delete[] element;
    element = novi;
    kapacitet /= 2;
  }

  void jelPrazan() const {
    if (velicina == 0)
      throw std::domain_error("Prazna lista!");
  }

  void Dislociraj() {
    if (element != nullptr) {
      for (int i = 0; i < velicina; i++) {
        delete element[i];
      }
      delete[] element;
      element = nullptr;
    }
  }

public:
  NizLista() : kapacitet(10000), velicina(0), trenutni_index(0) {
    element = new Tip *[kapacitet];
  }

  NizLista(const NizLista<Tip> &druga)
      : kapacitet(druga.kapacitet), velicina(druga.velicina),
        trenutni_index(druga.trenutni_index) {
    element = new Tip *[kapacitet];
    for (int i = 0; i < velicina; i++) {
      element[i] = new Tip(*druga.element[i]);
    }
  }

  ~NizLista() { Dislociraj(); }

  NizLista<Tip> &operator=(NizLista<Tip> druga) {
    std::swap(element, druga.element);
    std::swap(velicina, druga.velicina);
    std::swap(kapacitet, druga.kapacitet);
    std::swap(trenutni_index, druga.trenutni_index);
    return *this;
  }

  int brojElemenata() const override { return velicina; }

  Tip &trenutni() override {
    jelPrazan();
    return *element[trenutni_index];
  }

  const Tip &trenutni() const override {
    jelPrazan();
    return *element[trenutni_index];
  }

  bool prethodni() override {
    jelPrazan();
    if (trenutni_index == 0)
      return false;
    trenutni_index--;
    return true;
  }

  bool sljedeci() override {
    jelPrazan();
    if (trenutni_index == velicina - 1)
      return false;
    trenutni_index++;
    return true;
  }

  void pocetak() override {
    jelPrazan();
    trenutni_index = 0;
  }

  void kraj() override {
    jelPrazan();
    trenutni_index = velicina - 1;
  }

  void obrisi() override {
    jelPrazan();

    delete element[trenutni_index];

    if (trenutni_index == velicina - 1) {
      element[trenutni_index] = nullptr;
      if (trenutni_index != 0)
        trenutni_index--;
    } else {
      for (int i = trenutni_index; i < velicina - 1; i++)
        element[i] = element[i + 1];
      element[velicina - 1] = nullptr;
    }

    velicina--;
    if (velicina == 0)
      Dislociraj();
  }

  void dodajIspred(const Tip &el) override {
    if(!element){
        element=new Tip*[kapacitet];
        trenutni_index=0;
    }

    if (velicina == 0) {
      element[0] = new Tip(el);
    } else {
      for (int i = velicina; i > trenutni_index; i--)
        element[i] = element[i - 1];

      element[trenutni_index] = new Tip(el);
      trenutni_index++;
    }

    velicina++;
    if (velicina > 0.7 * kapacitet)
      prosiri();
  }

  void dodajIza(const Tip &el) override {
    if(!element){
      element=new Tip*[kapacitet];
      trenutni_index=0;
    }

    if (velicina == 0) {
      element[0] = new Tip(el);
    } else {
      for (int i = velicina; i > trenutni_index; i--)
        element[i] = element[i - 1];

      element[trenutni_index + 1] = new Tip(el);
    }

    velicina++;
    if (velicina > 0.7 * kapacitet)
      prosiri();
  }

  Tip &operator[](int i) override {
    if (i < 0 || i >= velicina)
      throw std::domain_error("Nevazeci indeks!");
    return *element[i];
  }

  const Tip &operator[](int i) const override {
    if (i < 0 || i >= velicina)
      throw std::domain_error("Nevazeci indeks!");
    return *element[i];
  }

  friend std::ostream &operator<<(std::ostream &tok,
                                  const NizLista<Tip> &lista) {
    for (int i = 0; i < lista.velicina; i++) {
      tok << *lista.element[i];
      if (i != lista.velicina - 1)
        tok << ", ";
    }
    return tok;
  }

  Lista<Tip> *DajKopiju() const override { return new NizLista<Tip>(*this); }
};

void testBrojElemenata() {
  NizLista<int> lista;

  std::cout << "Test brojElemenata (prazno) "
            << (lista.brojElemenata() == 0 ? "prosao" : "pao") << std::endl;

  lista.dodajIza(10);
  std::cout << "Test brojElemenata (jedan element) "
            << (lista.brojElemenata() == 1 ? "prosao" : "pao") << std::endl;

  lista.dodajIza(20);
  std::cout << "Test brojElemenata (dva elementa) "
            << (lista.brojElemenata() == 2 ? "prosao" : "pao") << std::endl;
}

void testTrenutni() {
  NizLista<int> lista;
  lista.dodajIza(30);
  lista.dodajIspred(40);
  lista.dodajIza(50);
  lista.dodajIza(10);

  std::cout << lista << std::endl;

  lista.pocetak();
  std::cout << "Test trenutni (prvi element) "
            << (lista.trenutni() == 40 ? "prosao" : "pao") << std::endl;

  lista.sljedeci();
  std::cout << "Test trenutni (drugi element) "
            << (lista.trenutni() == 30 ? "prosao" : "pao") << std::endl;

  lista.kraj();
  std::cout << "Test trenutni (zadnji element) "
            << (lista.trenutni() == 50 ? "prosao" : "pao") << std::endl;

  lista.prethodni();
  std::cout << "Test trenutni (predzadnji element) "
            << (lista.trenutni() == 10 ? "prosao" : "pao") << std::endl;
}

void testDodajIspred() {
  NizLista<int> lista;
  lista.dodajIza(10);
  lista.dodajIspred(5);

  std::cout << lista << std::endl;

  lista.pocetak();
  std::cout << "Test dodajIspred " << (lista.trenutni() == 5 ? "prosao" : "pao")
            << std::endl;
}

void testDodajIza() {
  NizLista<int> lista;
  lista.dodajIza(10);
  lista.dodajIza(20);

  std::cout << lista << std::endl;

  lista.sljedeci();
  std::cout << "Test dodajIza " << (lista.trenutni() == 20 ? "prosao" : "pao")
            << std::endl;
}

void testObrisi() {
  NizLista<int> lista;
  lista.dodajIza(10);
  lista.dodajIza(20);
  lista.dodajIza(30);
  lista.dodajIza(40);
  lista.dodajIza(50);

  std::cout << lista << std::endl;

  lista.kraj();
  lista.prethodni();
  lista.obrisi();

  std::cout << lista << std::endl;

  std::cout << "Test obrisi (trenutni) "
            << (lista.trenutni() == 20 ? "prosao" : "pao") << std::endl;

  std::cout << "Test obrisi (broj elemenata) "
            << (lista.brojElemenata() == 4 ? "prosao" : "pao") << std::endl;
}

void testOperatorIndex() {
  NizLista<int> lista;
  lista.dodajIza(10);
  lista.dodajIza(20);

  std::cout << "Test operator[] (index 0) "
            << (lista[0] == 10 ? "prosao" : "pao") << std::endl;
  std::cout << "Test operator[] (index 1) "
            << (lista[1] == 20 ? "prosao" : "pao") << std::endl;

  try {
    lista[2];
    std::cout << "Test operator[] (out of bounds) pao" << std::endl;
  } catch (std::domain_error &) {
    std::cout << "Test operator[] (out of bounds) prosao" << std::endl;
  }
}

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
  JednostrukaLista()
      : glava(nullptr), trenutni_pok(nullptr), rep(nullptr), velicina(0) {}

  ~JednostrukaLista() { Dislociraj(); }

  JednostrukaLista(const JednostrukaLista<Tip> &jlist)
      : glava(nullptr), rep(nullptr), trenutni_pok(nullptr),
        velicina(jlist.velicina) {
    try {
      if (jlist.glava == nullptr)
        return;

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

  friend std::ostream &operator<<(std::ostream &tok,
                                  const JednostrukaLista &lista) {
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

void testBrojElemenataJL() {
  JednostrukaLista<int> lista;

  std::cout << "Test brojElemenata (prazno) "
            << (lista.brojElemenata() == 0 ? "prosao" : "pao") << std::endl;

  lista.dodajIza(10);
  std::cout << "Test brojElemenata (jedan element) "
            << (lista.brojElemenata() == 1 ? "prosao" : "pao") << std::endl;

  lista.dodajIza(20);
  std::cout << "Test brojElemenata (dva elementa) "
            << (lista.brojElemenata() == 2 ? "prosao" : "pao") << std::endl;
}

void testTrenutniJL() {
  JednostrukaLista<int> lista;
  lista.dodajIza(30);
  lista.dodajIspred(40);
  lista.dodajIza(50);

  std::cout << lista << std::endl;

  lista.pocetak();
  std::cout << "Test trenutni (prvi element) "
            << (lista.trenutni() == 40 ? "prosao" : "pao") << std::endl;

  lista.sljedeci();
  std::cout << "Test trenutni (drugi element) "
            << (lista.trenutni() == 30 ? "prosao" : "pao") << std::endl;

  lista.kraj();
  std::cout << "Test trenutni (zadnji element) "
            << (lista.trenutni() == 50 ? "prosao" : "pao") << std::endl;

  lista.prethodni();
  std::cout << "Test trenutni (predzadnji element) "
            << (lista.trenutni() == 30 ? "prosao" : "pao") << std::endl;
}

void testDodajIspredJL() {
  JednostrukaLista<int> lista;
  lista.dodajIza(10);
  lista.dodajIspred(5);

  lista.pocetak();
  std::cout << "Test dodajIspred " << (lista.trenutni() == 5 ? "prosao" : "pao")
            << std::endl;
}

void testDodajIzaJL() {
  JednostrukaLista<int> lista;
  lista.dodajIza(10);
  lista.dodajIza(20);

  lista.kraj();
  std::cout << "Test dodajIza " << (lista.trenutni() == 20 ? "prosao" : "pao")
            << std::endl;
}

void testObrisiJL() {
  JednostrukaLista<int> lista;
  lista.dodajIza(10);
  lista.dodajIza(20);
  lista.dodajIza(30);

  lista.kraj();
  lista.obrisi();

  std::cout << "Test obrisi (trenutni) "
            << (lista.trenutni() == 30 ? "prosao" : "pao") << std::endl;

  std::cout << "Test obrisi (broj elemenata) "
            << (lista.brojElemenata() == 2 ? "prosao" : "pao") << std::endl;
}

void testOperatorIndexJL() {
  JednostrukaLista<int> lista;
  lista.dodajIza(10);
  lista.dodajIza(20);

  std::cout << "Test operator[] (index 0) "
            << (lista[0] == 10 ? "prosao" : "pao") << std::endl;
  std::cout << "Test operator[] (index 1) "
            << (lista[1] == 20 ? "prosao" : "pao") << std::endl;

  try {
    lista[2];
    std::cout << "Test operator[] (out of bounds) pao" << std::endl;
  } catch (std::domain_error &) {
    std::cout << "Test operator[] (out of bounds) prosao" << std::endl;
  }
}

template <typename Tip> class PolimorfnaLista {
  Lista<Tip> *list;
  void Test() const {
    if (!list)
      throw std::logic_error("Nespecificirana lista");
  }

public:
  PolimorfnaLista() : list(nullptr) {}
  ~PolimorfnaLista() { delete list; }
  Tip &trenutni() {
    Test();
    return list->trenutni();
  }
  bool sljedeci() {
    Test();
    return list->sljedeci()();
  }
  void pocetak() {
    Test();
    list->pocetak();
  }
  void kraj() {
    Test();
    list->kraj();
  }
  void obrisi() {
    Test();
    list->obrisi();
  }
  void dodajIspred(const Tip &el) {
    Test();
    list->dodajIspred(el);
  }
  void dodajIza(const Tip &el) {
    Test();
    list->dodajIza(el);
  }
  Tip &operator[](int i) {
    Test();
    return list->operator[](i);
  }
};

int main() {
  testBrojElemenata();
  testTrenutni();
  testDodajIspred();
  testDodajIza();
  testObrisi();
  testOperatorIndex();
  NizLista<int> l;
  l.dodajIza(10);
  l.dodajIza(20);
  std::cout << "Lista izlaz: " << l << std::endl<< std::endl;

  testBrojElemenataJL();
  testTrenutniJL();
  testDodajIspredJL();
  testDodajIzaJL();
  testObrisiJL();
  testOperatorIndexJL();

  return 0;
}