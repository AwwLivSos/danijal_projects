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

template<typename Tip> class DvostraniRed{
    DvostrukaLista<Tip> dq;
public:
    void brisi(){
        while (dq.brojElemenata() > 0) {
            dq.obrisi();
        }
    }
    int brojElemenata(){
        return dq.brojElemenata();
    }

    void staviNaVrh(const Tip &el){
        if(brojElemenata()!=0)dq.kraj();
        dq.dodajIza(el);
    }
    Tip skiniSaVrha(){
        dq.kraj();
        Tip el = dq.trenutni();
        dq.obrisi();
        return el;
    }
    Tip &vrh(){
        dq.kraj();
        return dq.trenutni();
    }

    void staviNaCelo(const Tip &el){
        if(brojElemenata()!=0)dq.pocetak();
        dq.dodajIspred(el);
    }
    Tip skiniSaCela(){
        dq.pocetak();
        Tip el = dq.trenutni();
        dq.obrisi();
        return el;
    }
    Tip &celo(){
        dq.pocetak();
        return dq.trenutni();
    }
};

void testStaviNaVrh() {
    DvostraniRed<int> dr;
    dr.staviNaVrh(10);
    if (dr.vrh() != 10) {
        std::cout << "testStaviNaVrh pao!" << std::endl;
        return;
    }
    dr.staviNaVrh(20);
    if (dr.vrh() != 20) {
        std::cout << "testStaviNaVrh pao!" << std::endl;
        return;
    }
    std::cout << "testStaviNaVrh prosao!" << std::endl;
}

void testStaviNaCelo() {
    DvostraniRed<int> dr;
    dr.staviNaCelo(10);
    if (dr.celo() != 10) {
        std::cout << "testStaviNaCelo pao!" << std::endl;
        return;
    }
    dr.staviNaCelo(5);
    if (dr.celo() != 5) {
        std::cout << "testStaviNaCelo pao!" << std::endl;
        return;
    }
    std::cout << "testStaviNaCelo prosao!" << std::endl;
}

void testSkiniSaVrha() {
    DvostraniRed<int> dr;
    dr.staviNaVrh(5);
    dr.staviNaVrh(15);
    dr.staviNaVrh(25);

    bool test = true;
    if (dr.skiniSaVrha() != 25 || dr.vrh() != 15) test = false;
    if (dr.skiniSaVrha() != 15 || dr.vrh() != 5) test = false;
    if (dr.skiniSaVrha() != 5) test = false;

    try {
        dr.skiniSaVrha();
        test = false;
    } catch (std::exception &e) {
        std::cout << e.what() << ", za prazan red!" << std::endl;
    }

    std::cout << (test ? "testSkiniSaVrha prosao!" : "testSkiniSaVrha pao!") << std::endl;
}

void testSkiniSaCela() {
    DvostraniRed<int> dr;
    dr.staviNaCelo(5);
    dr.staviNaCelo(15);
    dr.staviNaCelo(25);

    bool test = true;
    if (dr.skiniSaCela() != 25 || dr.celo() != 15) test = false;
    if (dr.skiniSaCela() != 15 || dr.celo() != 5) test = false;
    if (dr.skiniSaCela() != 5) test = false;

    try {
        dr.skiniSaCela();
        test = false;
    } catch (std::exception &e) {
        std::cout << e.what() << ", za prazan red!" << std::endl;
    }

    std::cout << (test ? "testSkiniSaCela prosao!" : "testSkiniSaCela pao!") << std::endl;
}

void testBrisi() {
    DvostraniRed<int> dr;
    dr.staviNaVrh(1);
    dr.staviNaCelo(2);
    dr.staviNaVrh(3);

    bool test = true;
    if (dr.brojElemenata() != 3) test = false;
    dr.brisi();
    if (dr.brojElemenata() != 0) test = false;

    try {
        dr.celo();
        test = false;
    } catch (std::exception &e) {
        std::cout << e.what() << ", za biranje na praznom redu!" << std::endl;
    }

    std::cout << (test ? "testBrisi prosao!" : "testBrisi pao!") << std::endl;
}

void testBrojElemenata() {
    DvostraniRed<double> dr;
    dr.staviNaVrh(3.14);
    if (dr.brojElemenata() != 1) exit(EXIT_FAILURE);
    dr.staviNaCelo(-0.8);
    if (dr.brojElemenata() != 2) exit(EXIT_FAILURE);

    dr.brisi();
    if (dr.brojElemenata() != 0) std::abort();

    std::cout << "testBrojElemenata prosao!" << std::endl;
}

void testPrazanRed() {
    DvostraniRed<int> dr;
    try {
        dr.celo();
        std::cout << "testPrazanRed pao!" << std::endl;
    } catch (std::exception &) {
        std::cout << "testPrazanRed prosao!" << std::endl;
    }

    try {
        dr.vrh();
        std::cout << "testPrazanRed pao!" << std::endl;
    } catch (std::exception &) {
        std::cout << "testPrazanRed prosao!" << std::endl;
    }
}

int main(){
    testStaviNaVrh();
    testStaviNaCelo();
    testSkiniSaVrha();
    testSkiniSaCela();
    testBrisi();
    testBrojElemenata();
    testPrazanRed();
    return 0;
}