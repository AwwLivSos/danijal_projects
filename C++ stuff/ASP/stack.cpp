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

bool palindrom(const Lista<char>& rijec){
    Stek<char> s;
    for(int i=0; i<rijec.brojElemenata(); i++){
        s.stavi(rijec[i]);
    }
    for(int i=0; i<rijec.brojElemenata(); i++){
        if(rijec[i] != s.vrh()) return false;
        s.skini();
    }
    return true;
}

bool provjeri_zagrade(const std::string& tekst) {
    Stek<char> s;
    for (char c : tekst) {
        if (c == '(' || c == '[' || c == '{') {
            s.stavi(c);                                 //ovdje uzmi svaku lijevu zagradu
        } else if (c == ')' || c == ']' || c == '}') {  //uzmi desnu zagradu
            if (s.brojElemenata() == 0) return false;   //ako, kada si uzeo desnu zagradu, nema nista u steku, znaci da nema match, jer mora imati bar neka lijeva da se spoje
            char top = s.vrh();                         //ovdje uzmi zadnju lijevu zagradu
            s.skini();                                  //morace eventualno uvijek biti lijeva i desna jedna do druge, sklanjanje se desava sa unutrasnje strane ka vanjskoj
            //kada smo uzeli jednu lijevu i jednu desnu testiramo da li se slazu, ako ne, odmah false
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) {
                return false;
            }
        }
    }
    return s.brojElemenata() == 0;
}

template<typename Tip>
    int presjek(Stek<Tip> s1, Stek<Tip> s2){
        int n{};
        while(s1.brojElemenata()>0 && s2.brojElemenata()>0){
            if(s1.vrh() == s2.vrh()){
                n++;
                s1.skini();
                s2.skini();
            }
            else if(s1.vrh()<s2.vrh()) s1.skini();
            else s2.skini();
        }
        return n;
    }

void spoji(Stek<int> s1, Stek<int> s2, Stek<int> &s3) {
    while(s1.brojElemenata()!=0){
        s3.stavi(s1.skini());
    }

    while(s3.brojElemenata()>0 && s2.brojElemenata()>0){
        if(s2.vrh()>s3.vrh()) s1.stavi(s2.skini());
        else s1.stavi(s3.skini());
    }
    if(s2.brojElemenata()!=0) s1.stavi(s2.skini());
    if(s3.brojElemenata()!=0) s1.stavi(s3.skini());

    while(s1.brojElemenata()!=0){
        s2.stavi(s1.skini());
    }
    while(s2.brojElemenata()!=0){
        s3.stavi(s2.skini());
    }
}

void testStaviVrh() {
    Stek<int> s;
    s.stavi(10);
    if(s.vrh()!=10) {
        std::cout<<"Ne valja"<<std::endl;
        return;
    }
    s.stavi(20);
    if(s.vrh()!=20) {
        std::cout<<"Ne valja"<<std::endl;
        return;
    }
    std::cout << "test za stavljanje i vrh je ok" << std::endl;
}

void testSkini() {
    Stek<int> s;
    s.stavi(5);
    s.stavi(15);
    s.stavi(25);

    bool test = true;
    //dva odjednom odmah
    if (s.skini()!=25 || s.vrh()!=15) test = false;
    if (s.skini()!=15 || s.vrh()!=5) test = false;
    if (s.skini()!=5) test = false;

    try {
        s.skini(); 
        test = false;
    } catch (std::exception &e) {
        std::cout << e.what() << ", za prazan stek!\n";
    }

    std::cout << (test ? "testSkini prosao\n" : "testSkini pao\n");
}

void testPrazanStek() {
    Stek<int> s;
    try {
        s.vrh();
        std::cout << "testPrazanStek pao!\n";
    } catch (std::exception &) {
        std::cout << "testPrazanStek prosao!\n";
    }
}

void testBrisi() {
    Stek<int> s;
    s.stavi(1);
    s.stavi(2);
    s.stavi(3);

    bool bul = true;

    if (s.brojElemenata() != 3) bul = false;
    s.brisi();
    if (s.brojElemenata() != 0) bul = false;

    try {
        s.vrh();
        bul = false;
    } catch (std::exception &e) {
        std::cout << e.what() << ", za biranje na praznom steku!\n";
    }

    std::cout << (bul ? "testBrisi prosao!\n" : "testBrisi pao!\n");
}

void testVelicina(){
    Stek<double> s;
    s.stavi(3.1415926538);
    if(s.brojElemenata()!=1) exit(EXIT_FAILURE);
    s.stavi(-0.0000000008); //dramaticno
    if(s.brojElemenata()!=2) exit(EXIT_FAILURE);

    s.brisi();
    if(s.brojElemenata()!=0) std::abort();
    std::cout<<"Izbjegao si critical failure, testVelicina prosao!\n";
}

using namespace std;

int main(){/*
    JednostrukaLista<char> l;
    std::string s = "RATAR";
    for(auto c : s) l.dodajIza(c), l.kraj();
    std::cout<<palindrom(l)<<std::endl;

    Stek<char> s1, s2;
    s1.stavi('F'); s1.stavi('D'); s1.stavi('C'); s1.stavi('A'); 
    s2.stavi('G'); s2.stavi('F'); s2.stavi('E'); s2.stavi('D'); s2.stavi('C'); s2.stavi('B');
    std::cout << "Broj zajednickih elemenata: " << presjek(s1, s2) << std::endl;

    std::string tekst = "[()]{[()()]()}";
    std::cout << provjeri_zagrade(tekst)<< std::endl;

    tekst = "[(])";
    std::cout << provjeri_zagrade(tekst)<< std::endl;

    Stek<int> st1;
    st1.stavi(7); st1.stavi(5); st1.stavi(3); st1.stavi(1); 
    Stek<int> st2;
    st2.stavi(2); st2.stavi(4); st2.stavi(6); st2.stavi(8);
    Stek<int> st3;
    spoji(st1, st2, st3);
    while(st3.brojElemenata()!=0) std::cout<<st3.skini()<<" ";*/

    testStaviVrh();
    testSkini();
    testPrazanStek();
    testBrisi();
    testVelicina();
}