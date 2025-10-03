#include <iostream>
#include <new>
#include <stdexcept>

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
    virtual void dodajIza(const Tip &el) = 0; // ove dvije metode dodaju element u odnosu na trenutnog, ali bez da se trenutni mijenja, ako je lista prazna onda novi element je prvi element
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

  void fakatIspisiUnazad(Cvor* cvor){
      if(cvor == nullptr) return;
      fakatIspisiUnazad(cvor->next);
      std::cout<< cvor->element<<" ";
  }

  void ispisiUnazad(){
      fakatIspisiUnazad(glava);
  }
};

template<typename Tip1, typename Tip2> class Iterator;
#include <utility>

template <typename tip1, typename tip2> class Mapa{
public:
    Mapa() = default;
    virtual ~Mapa() = default;
    virtual tip2 &operator[](tip1 kljuc) = 0; 
    virtual const tip2 &operator[](tip1 kljuc) const = 0;
    virtual int brojElemenata() const =0;
    virtual void obrisi(const tip1& kljuc) = 0;
    virtual void obrisi() =0;
};

template <typename tipk, typename tipv> class NizMapa : public Mapa<tipk, tipv>{
    JednostrukaLista<std::pair<tipk, tipv>> lista;
public:
    NizMapa() = default;
    ~NizMapa() = default;

    tipv &operator[](tipk kljuc) override{
        for(int i=0; i<lista.brojElemenata(); i++){
            if(lista[i].first == kljuc) return lista[i].second;
        }
        lista.dodajIza({kljuc, tipv{}});
        lista.sljedeci();
        return lista.trenutni().second;
    }

    const tipv &operator[](tipk kljuc) const override{
        for(int i=0; i<lista.brojElemenata(); i++){
            if(lista[i].first == kljuc) return lista[i].second;
        }
        //ovo ne valja, da li da vratim neku staticku default varijablu pa da sve valja?
        static tipv v{};
        return v;
        throw std::domain_error("Kljuc ne postoji");
    }

    int brojElemenata() const override{
        return lista.brojElemenata();
    }

    void obrisi() override{
        while (lista.brojElemenata() > 0) {
            lista.pocetak();
            lista.obrisi();
        }
    }

    void obrisi(const tipk& kljuc) override {
        for(int i=0; i<lista.brojElemenata(); i++){
            if(lista[i].first == kljuc){
                lista.pocetak();
                for (int j = 0; j < i; j++) lista.sljedeci();
                lista.obrisi();
                return;
            }
        }
        throw std::domain_error("Kljuc ne postoji");
    }

    friend class Iterator<tipk, tipv>;
};

template <typename Tip1, typename Tip2> class Iterator{
    const NizMapa<Tip1, Tip2>*  mapa;
    int idx;
public:
    Iterator(const NizMapa<Tip1, Tip2>& m) : mapa(&m), idx(0){}

    std::pair<Tip1, Tip2> trenutni() const{
        if (mapa->brojElemenata() == 0 || idx < 0 || idx >= mapa->brojElemenata())
            throw std::domain_error("Iterator van opsega");
        return (*mapa).lista[idx];
    }

    bool prethodni() {
        if (idx <= 0) return false;
        idx--;
        return true;
    }

    bool sljedeci() {
        if (idx >= mapa->brojElemenata() - 1) return false;
        idx++;
        return true;
    }

    void pocetak() {
        if (mapa->brojElemenata() == 0)
            throw std::domain_error("Mapa je prazna");
        idx = 0;
    }

    void kraj() {
        if (mapa->brojElemenata() == 0)
            throw std::domain_error("Mapa je prazna");
        idx = mapa->brojElemenata() - 1;
    }
};

void testDodavanjeElemenata() {
    NizMapa<int, std::string> mapa;
    mapa[1] = "jedan";
    mapa[2] = "dva";
    mapa[3] = "tri";

    std::cout<<mapa.brojElemenata()<<std::endl; //3
    for(int i=0; i<mapa.brojElemenata(); i++) std::cout<<mapa[i]<<" ";

    std::cout << "testDodavanjeElemenata prosao" << std::endl;
}

void testAzuriranjePostojecihElemenata() {
    NizMapa<int, std::string> mapa;
    mapa[1] = "jedan";
    mapa[1] = "novi jedan";

    std::cout<<mapa.brojElemenata()<<std::endl; //1
    for(int i=0; i<mapa.brojElemenata(); i++) std::cout<<mapa[i]<<" ";

    std::cout << "testAzuriranjePostojecihElemenata prosao" << std::endl;
}

void testObrisiJedanElement() {
    NizMapa<int, std::string> mapa;
    mapa[1] = "jedan";
    mapa[2] = "dva";
    mapa.obrisi(1);

    try {
        mapa.obrisi(1);
    } catch (const std::domain_error& e) {
        std::cout<<"Izuzetak za brisanje\n";
    }

    std::cout << "testObrisiJedanElement prosao" << std::endl;
}

void testObrisiSveElemente() {
    NizMapa<int, std::string> mapa;
    mapa[1] = "jedan";
    mapa[2] = "dva";
    mapa[3] = "tri";

    mapa.obrisi();
    std::cout<<mapa.brojElemenata()<<std::endl; //0

    mapa[1] = "nesto drugo";
    std::cout<<mapa[1]<<std::endl;

    std::cout << "testObrisiSveElemente prosao" << std::endl;
}

void testKljucNePostoji() {
    const NizMapa<int, std::string> mapa;

    try {
        mapa[42];
    } catch (const std::domain_error& e) {
        std::cout<<"Izuzetak za const verziju\n";
    }

    std::cout << "testKljucNePostoji prosao" << std::endl;
}


using namespace std;

int main(){
    /* testAzuriranjePostojecihElemenata();
    std::cout<<std::endl;
    testDodavanjeElemenata();
    std::cout<<std::endl;
    testKljucNePostoji();
    std::cout<<std::endl;
    testObrisiJedanElement();
    std::cout<<std::endl;
    testObrisiSveElemente();
    std::cout<<std::endl; */

    NizMapa<int, std::string> mapa;
    mapa[1] = "jen";
    mapa[2] = "dva";
    mapa[3] = "tri";

    Iterator<int, std::string> it(mapa);
    it.pocetak();
    while (true) {
        auto par = it.trenutni();
        std::cout << par.first << ": " << par.second << std::endl;
        if (!it.sljedeci()) break;
    }

    return 0;
}