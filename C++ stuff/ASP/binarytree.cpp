#include <stdexcept>
#include <utility>
#include <iostream>

template<typename Tip1, typename Tip2> class Iterator;

template <typename tip1, typename tip2> class Mapa{
public:
    Mapa() = default;
    virtual ~Mapa() = default;
    virtual tip2 &operator[](const tip1 &kljuc) = 0; 
    virtual const tip2 &operator[](const tip1 &kljuc) const = 0;
    virtual int brojElemenata() const =0;
    virtual void obrisi(const tip1& kljuc) = 0;
    virtual void obrisi() =0;
};

template <typename tipk, typename tipv> class BinStabloMapa : public Mapa<tipk, tipv>{
    struct Cvor{
        tipk kljuc;
        tipv value;
        Cvor* desni;
        Cvor* lijevi;
    };
    Cvor* korijen;
    int velicina;

    Cvor* Pretrazi(Cvor* c, tipk k) const{
        if(c==nullptr || k==c->kljuc) return c;

        if(k<c->kljuc) return Pretrazi(c->lijevi, k);
        else return Pretrazi(c->desni, k);
    }

    void obrisirekurzivno(Cvor* c){
        if(!c) return;
        obrisirekurzivno(c->desni);
        obrisirekurzivno(c->lijevi);
        delete c;
        c=nullptr;
    }
            //referenca na cvor da ne bude seg fault
    void Umetni(Cvor* &c, const tipk &kljuc){
        if(!c) c=new Cvor{kljuc, {}, nullptr, nullptr};
        else if (kljuc < c->kljuc) Umetni(c->lijevi, kljuc);
        else Umetni(c->desni, kljuc);
    }

    Cvor* max_elemenet(Cvor* c) const{
        while(c && c->desni) c=c->desni;
        return c;
    }

    Cvor* min_element(Cvor* c) const{           //min i max element primaju cvor radi brisanja jednog cvora 
        while(c && c->lijevi) c=c->lijevi;
        return c;
    }

    Cvor* obrisiJedanCvor(Cvor* c, const tipk &kljuc){          //ova metoda vraca vrijednost jer treba na neki nacin da se osigura roditelj o izbrisanog cvora
        if(!c) return nullptr;
        if(kljuc<c->kljuc) c->lijevi = obrisiJedanCvor(c->lijevi, kljuc);
        else if(kljuc>c->kljuc) c->desni = obrisiJedanCvor(c->desni, kljuc);        //provjeri da li je veci ili manji u dva ifa, pa nakon njih kreni sa brisanjem (kljucevi su jednaki)
        else{
            if(c->desni && c->lijevi){
                auto zamjena = min_element(c->desni);
                std::swap(zamjena->kljuc, c->kljuc);
                std::swap(zamjena->value, c->value);        //ako je cvor sa dvoje djece, samo ga swappaj sa najmanjim zdesna (ili najmanjim slijeva), pa onda imas za obrisat cvor bez djece ili sa jednim djetetom
                c->desni = obrisiJedanCvor(c->desni, zamjena->kljuc);
            }
            else{       //u suprtonom je cvor za brisanje bez djece ili sa jednim djetetom, sto je lakse za obrisat
                Cvor* temp = c->lijevi ? c->lijevi : c->desni;
                delete c;
                velicina--;
                return temp;
            }
        }
        return c;
    }

    Cvor* kopiraj(Cvor* c) {            //potencijalno se moze koristit van konstruktora
        if (!c) return nullptr;
        Cvor* novi = new Cvor{c->kljuc, c->value, nullptr, nullptr};
        novi->lijevi = kopiraj(c->lijevi);
        novi->desni = kopiraj(c->desni);
        return novi;
    }

public:
    BinStabloMapa() : korijen(nullptr), velicina(0) {}
    ~BinStabloMapa() {this->obrisi();}
    BinStabloMapa(const BinStabloMapa &drugo) : velicina(drugo.velicina), korijen(kopiraj(drugo.korijen)){}

    BinStabloMapa& operator=(BinStabloMapa drugo) {
        std::swap(korijen, drugo.korijen);
        std::swap(velicina, drugo.velicina);
        return *this;
    }
    
    const tipv &operator[](const tipk &kluc) const override{
        auto trazeni = Pretrazi(korijen, kluc);
        if(!trazeni){
            static tipv v{};
            return v;
        }
        return trazeni->value;
    }

    tipv &operator[](const tipk &kljuc) override{
        auto trazeni = Pretrazi(korijen, kljuc);
        if(!trazeni){
            Umetni(korijen, kljuc);
            trazeni = Pretrazi(korijen, kljuc);
            velicina++;
        }
        return trazeni->value;
    }

    int brojElemenata() const override {return velicina;}

    void obrisi() override {
        obrisirekurzivno(korijen);
        korijen=nullptr;
        velicina=0;
    }

    void obrisi(const tipk &kljuc) override{
        korijen=obrisiJedanCvor(korijen, kljuc);
    }
};

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

template <typename tipk, typename tipv> class NizMapa : public Mapa<tipk, tipv>{
    JednostrukaLista<std::pair<tipk, tipv>> lista;
public:
    NizMapa() = default;
    ~NizMapa() = default;

    tipv &operator[](const tipk &kljuc) override{
        for(int i=0; i<lista.brojElemenata(); i++){
            if(lista[i].first == kljuc) return lista[i].second;
        }
        lista.dodajIza({kljuc, tipv{}});
        lista.sljedeci();
        return lista.trenutni().second;
    }

    const tipv &operator[](const tipk &kljuc) const override{
        for(int i=0; i<lista.brojElemenata(); i++){
            if(lista[i].first == kljuc) return lista[i].second;
        }
        //ovo ne valja, da li da vratim neku staticku default varijablu pa da sve valja?
        static tipv v{};
        return v;
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
    }
};

const int N = 10000; //broj elemenata
const int M = 1000;  //broj pretraga

template <typename MapType>
void testMap(const std::string &mapName) {
    MapType map;

    clock_t start = clock();
    for (int i = 0; i < M; i++) {
        int key = rand() % N;
        volatile auto value = map[key];
    }
    clock_t end = clock();
    std::cout << mapName << " - Vrijeme za pretragu: "  << static_cast<double>(end - start) / CLOCKS_PER_SEC << " seconds\n";

    start = clock();
    for (int i = 0; i < N; i++) {
        map.obrisi(i); 
    }
    end = clock();
    std::cout << mapName << " - Vrijeme za brisanje: " << static_cast<double>(end - start) / CLOCKS_PER_SEC << " seconds\n";

    start = clock();
    for (int i = 0; i < N; i++) {
        map[i] = i;
    }
    end = clock();
    std::cout << mapName << " - Vrijeme za insertanje: " << static_cast<double>(end - start) / CLOCKS_PER_SEC << " seconds\n";
}

using namespace std;

int main(){
    std::cout << "Test BinStabloMapa...\n";
    testMap<BinStabloMapa<int, int>>("BinStabloMapa");

    std::cout << "\nTest ListaMapa...\n";
    testMap<NizMapa<int, int>>("ListaMapa");
    //dok stablo zavrsi za oko sekundu insert, a za ostalo mu ne treba ni 10e-4 sekudni, 
    //nizmapa zavrsi pretragu za pola sekunde, brisanje za oko 4/5 sekundi, a insert se ne uspije ni zavrsiti!
    return 0;
}