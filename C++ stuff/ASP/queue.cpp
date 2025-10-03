#include <iostream>

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
    virtual void obrisi() = 0; // treba da obrise trenutni i da trenutni postane sljedeci element po redu, ako se izbrise zadnji onda je trenutni prethodni ako je lista prazna, prethodnih 5 metoda bacaju izuzetak
    virtual void dodajIspred(const Tip &el) = 0;
    virtual void dodajIza(
    const Tip &el) = 0; // ove dvije metode dodaju element u odnosu na trenutnog, ali bez da se trenutni mijenja, ako je lista prazna onda novi element je prvi element
    virtual Tip &operator[](int i) = 0; // koristi se za citanje i pisanje, ako je i<=0 ili veci od broja elemenata, baca izuzetak
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
  JednostrukaLista(): glava(nullptr), trenutni_pok(nullptr), rep(nullptr), velicina(0) {}

  ~JednostrukaLista() { Dislociraj(); }

  JednostrukaLista(const JednostrukaLista<Tip> &jlist) : glava(nullptr), rep(nullptr), trenutni_pok(nullptr), velicina(jlist.velicina) {
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

template <typename Tip> class Red{
    JednostrukaLista<Tip> q;
public: 
    void brisi(){
        while (q.brojElemenata() > 0) {
            q.obrisi();
        }
    }
    void stavi(const Tip &el){
        if(brojElemenata()!=0)q.kraj();
        q.dodajIza(el);
    }
    Tip skini(){
        q.pocetak();
        Tip el = q.trenutni();
        q.obrisi();
        return el;
    }
    Tip &celo(){
        q.pocetak();
        return q.trenutni();
    }
    int brojElemenata(){
        return q.brojElemenata();
    }
};

void testStavi() {
    Red<int> s;
    s.stavi(10);
    if(s.celo()!=10) {
        std::cout<<"Ne valja"<<std::endl;
        return;
    }
    s.stavi(20);
    if(s.celo()!=10) {
        std::cout<<"Ne valja"<<std::endl;
        return;
    }
    std::cout << "test za stavljanje je ok" << std::endl;
}

void testSkini() {
    Red<int> s;
    s.stavi(5);
    s.stavi(15);
    s.stavi(25);

    bool test = true;
    //dva odjednom odmah
    if (s.skini()!=5 || s.celo()!=15) test = false;
    if (s.skini()!=15 || s.celo()!=25) test = false;
    if (s.skini()!=25) test = false;

    try {
        s.skini(); 
        test = false;
    } catch (std::exception &e) {
        std::cout << e.what() << ", za prazan red!\n";
    }

    std::cout << (test ? "testSkini prosao\n" : "testSkini pao\n");
}

void testPrazanRed() {
    Red<int> s;
    try {
        s.celo();
        std::cout << "testPrazanRed pao!\n";
    } catch (std::exception &) {
        std::cout << "testPrazanRed prosao!\n";
    }
}

void testBrisi() {
    Red<int> s;
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);

    bool bul = true;

    if (s.brojElemenata() != 3) bul = false;
    s.brisi();
    if (s.brojElemenata() != 0) bul = false;

    try {
        s.celo();
        bul = false;
    } catch (std::exception &e) {
        std::cout << e.what() << ", za biranje na praznom redu!\n";
    }

    std::cout << (bul ? "testBrisi prosao!\n" : "testBrisi pao!\n");
}

void testVelicina(){
    Red<double> s;
    s.stavi(3.1415926538);
    if(s.brojElemenata()!=1) exit(EXIT_FAILURE);
    s.stavi(-0.0000000008);
    if(s.brojElemenata()!=2) exit(EXIT_FAILURE);

    s.brisi();
    if(s.brojElemenata()!=0) std::abort();
    std::cout<<"Izbjegao si critical failure, testVelicina prosao!\n";
}

using namespace std;

int main(){
    /*Red<int> r;
    for(int i=0; i<7; i++) r.stavi(i);
    for(int i=0; i<7; i++) std::cout<<r.skini()<<" ";*/

    testVelicina();
    testBrisi();
    testPrazanRed();
    testSkini();
    testStavi();
}