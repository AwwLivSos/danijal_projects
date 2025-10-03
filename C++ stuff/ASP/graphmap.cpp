#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <utility>
#include <vector>

using namespace std;

template <typename sppk, typename sppv> class Mapa {

public:
  Mapa<sppk, sppv>() {}
  virtual ~Mapa<sppk, sppv>() {}
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const sppk &kljuc) = 0;
  virtual sppv &operator[](sppk i) = 0;
  virtual const sppv operator[](sppk i) const = 0;
};


template <typename sppk, typename sppv>
class HashMapaLan : public Mapa<sppk, sppv> {
protected:
  int br_ele;
  std::vector<std::list<std::pair<sppk, sppv> *>> p;
  unsigned int (*funkcija)(sppk, unsigned int);

public:
  HashMapaLan<sppk, sppv>() : Mapa<sppk, sppv>(), br_ele(0), funkcija(nullptr) {
    p.resize(10000);
  }

  ~HashMapaLan<sppk, sppv>() { obrisi(); }

  HashMapaLan<sppk, sppv>(const HashMapaLan<sppk, sppv> &a) {

    br_ele = a.br_ele;
    funkcija = a.funkcija;

    for (int i = 0; i < a.p.size(); i++) {

      p.push_back(std::list<std::pair<sppk, sppv> *>());
      auto it = a.p[i].begin();
      while (it != a.p[i].end()) {
        p[i].push_back(new std::pair<sppk, sppv>((*it)->first, (*it)->second));
        it++;
      }
    }
  }

  HashMapaLan<sppk, sppv> &operator=(const HashMapaLan<sppk, sppv> &a) {

    if (this == &a)
      return *this;

    obrisi();

    while (p.size() < a.p.size())
      p.push_back(std::list<std::pair<sppk, sppv> *>());

    br_ele = a.br_ele;
    funkcija = a.funkcija;

    for (int i = 0; i < a.p.size(); i++) {

      auto it = a.p[i].begin();
      while (it != a.p[i].end()) {
        p[i].push_back(new std::pair<sppk, sppv>((*it)->first, (*it)->second));
        it++;
      }
    }

    return *this;
  }

  void obrisi() {

    if (!funkcija)
      throw std::domain_error("Hash funkcija?");
    if (br_ele == 0)
      throw std::range_error("Nema elemenata");

    for (int i = 0; i < p.size(); i++) {

      auto it = p[i].begin();
      while (it != p[i].end()) {
        delete *it;
        it++;
      }

      p[i].resize(0);
    }

    br_ele = 0;
  }

  void obrisi(const sppk &kljuc) {

    if (!funkcija)
      throw std::domain_error("Hash funkcija?");

    int in = funkcija(kljuc, p.size());
    typename std::list<std::pair<sppk, sppv> *>::iterator it = p[in].begin();

    while (it != p[in].end()) {

      if ((*it)->first == kljuc) {
        delete *it;
        p[in].erase(it);
        br_ele--;
        return;
      }
      it++;
    }

    throw std::logic_error("Nema kljuca");
  }

  int brojElemenata() const { return br_ele; }

  int TraziHash(const sppk &k) const {

    int in = funkcija(k, p.size());
    typename std::list<std::pair<sppk, sppv> *> l = p[in];
    typename std::list<std::pair<sppk, sppv> *>::iterator it = l.begin();
    int i = 0;

    while (it != l.end()) {

      if ((*it)->first == k)
        return i;

      i++;
      it++;
    }

    return -1;
  }

  sppv &operator[](sppk kljuc) {

    if (!funkcija)
      std::__throw_domain_error("Hash funkcija?");

    int in = funkcija(kljuc, p.size());
    typename std::list<std::pair<sppk, sppv> *>::iterator it = p[in].begin();

    if (p[in].empty()) {
      p[in].push_back(new std::pair<sppk, sppv>(kljuc, sppv()));
      br_ele++;
      it = p[in].begin();
      return (*it)->second;
    }

    if ((*it)->first == kljuc)
      return (*it)->second;
    else {
      int q = TraziHash(kljuc);
      if (q == -1) {
        br_ele++;
        p[in].push_back(new std::pair<sppk, sppv>(kljuc, sppv()));
        p[in].sort([](const std::pair<sppk, sppv> *a,
                      const std::pair<sppk, sppv> *b) -> bool {
          return a->first < b->first;
        });
        q = TraziHash(kljuc);
        it = p[in].begin();
      }
      while (q > 0) {
        it++;
        q--;
      }
    }

    return (*it)->second;
  }

  const sppv operator[](sppk kljuc) const {

    if (!funkcija)
      std::__throw_domain_error("Hash funkcija?");

    int in = funkcija(kljuc, p.size());
    typename std::list<std::pair<sppk, sppv> *> l = p[in];
    typename std::list<std::pair<sppk, sppv> *>::iterator it = l.begin();

    if (!p[in].empty() && (*it)->first == kljuc)
      return (*it)->second;

    else if (p[in].empty())
      return sppv();
    else {
      in = TraziHash(kljuc);
      if (in == -1)
        return sppv();
      else
        while (in > 0) {
          it++;
          in--;
        }
    }

    return (*it)->second;
  }

  void definisiHashFunkciju(unsigned int (*func)(sppk, unsigned int)) {
    funkcija = func;
    return;
  }
};

template <typename spp> class GranaIterator;
template <typename spp> class Grana;
template <typename spp> class Cvor;
template <typename spp> class ListaGraf;

template <typename spp> class UsmjereniGraf {
public:
  UsmjereniGraf() {}
  UsmjereniGraf(int a) {}
  virtual ~UsmjereniGraf() {}
  
  virtual int dajBrojCvorova() const = 0;
  virtual void postaviBrojCvorova(int a)  = 0;
  virtual void dodajGranu(int p, int d, float t)  = 0;
  virtual void obrisiGranu(int p, int d)  = 0;
  virtual bool  postojiGrana(int p, int d)  = 0;
  virtual void postaviTezinuGrane(int p, int d, float t) = 0;
  virtual spp dajOznakuGrane(int p, int d)  = 0;
  virtual spp dajOznakuCvora(int b)  = 0;
  virtual float dajTezinuGrane(int p, int d) = 0;
  virtual void postaviOznakuCvora(int b, spp a)  = 0;
  virtual void postaviOznakuGrane(int p, int d, spp a)  = 0;
   Grana<spp> dajGranu(int p, int d) {return Grana<spp>(this,p,d);} 
   Cvor<spp> dajCvor(int b) { return Cvor<spp>(this,b); }

   GranaIterator<spp> dajGranePocetak() {
    GranaIterator<spp> it(this,0,-1);
    return ++it;
  }

   GranaIterator<spp> dajGraneKraj() {
    GranaIterator<spp> it(this,-1,-1);
    return it;
  }
  virtual GranaIterator<spp> dajSljedecuGranu(int p, int d) = 0;
};

template <typename spp> class Cvor  {

  UsmjereniGraf<spp> *u;
  spp oznaka;
  int broj;
  std::vector<Grana<spp>> grane;
  std::vector<Cvor<spp>> susjedi;

public:

  Cvor(UsmjereniGraf<spp> *a, int b) : u(a), broj(b) {}

  int dajRedniBroj() const { return broj; }

  void postaviOznaku(spp a) { u->postaviOznakuCvora(broj, a); }

  spp dajOznaku() const { return u->dajOznakuCvora(broj); }

  void dodajVezu(Grana<spp> g, Cvor<spp> s) {
    grane.push_back(g);
    susjedi.push_back(s);
  }
  void obrisiVezu(Grana<spp> g, Cvor<spp> s) {

    for (int i = 0; i < susjedi.size(); i++)
      if (s == susjedi[i]) {

        for (int j = i; j < susjedi.size() - 1; j++) {
          susjedi[j] = susjedi[j + 1];
          grane[j] = grane[j + 1];
        }
        break;
      }
    susjedi.pop_back();
    grane.pop_back();
  }

  std::vector<Grana<spp>> dajGrane() const { return grane; }
  std::vector<Cvor<spp>> dajSusjede() const { return susjedi; }
};

template <typename spp> class Grana  {

  UsmjereniGraf<spp> *u;
  spp oznaka;
  float tezina;
  int p, d;

public:
  Grana(UsmjereniGraf<spp> *ug, int polazni, int dolazni)
      : u(ug), p(polazni), d(dolazni){ }
  float dajTezinu() const { return u->dajTezinuGrane(p,d); }
  void postaviTezinu(float a) {u->postaviTezinuGrane(
      p, d, a);}
  void postaviOznaku(spp a) {
   u->postaviOznakuGrane(p, d, a); }
  spp dajOznaku() const { return u->dajOznakuGrane(p,d); }
  Cvor<spp> dajPolazniCvor() const { return u->dajCvor(p); };
  Cvor<spp> dajDolazniCvor() const { return u->dajCvor(d); };
};

template <typename spp> class GranaIterator {

  int p, d;
  UsmjereniGraf<spp> *u;

public:
  GranaIterator( UsmjereniGraf<spp> *us, int a, int b):p(a),d(b),u(us){};

 Grana<spp> operator*(){return u->dajGranu(p,d);}
 bool operator==(const GranaIterator &it)const{
     return it.u == u && it.p == p && it.d == d;
 }
 bool operator!=(const GranaIterator &it)const{
     return it.u != u || it.p != p || it.d != d;
 }
 GranaIterator &operator++(){
     GranaIterator<spp> sljedeci = u->dajSljedecuGranu(p,d);
     p  = sljedeci.p;
     d = sljedeci.d;
     return *this;
 }
 GranaIterator operator++(int){
     GranaIterator<spp> ovaj = *this;
     ++(*this);
     return ovaj;
 }

};


template <typename spp> class ListaGraf : public UsmjereniGraf<spp> {

  struct grana {
      spp oznaka;
      float tezina;
      bool postoji;
  };
    std::vector<std::vector<grana>> lista;
    std::vector<spp> oznake;

public:
  ListaGraf(int b) : UsmjereniGraf<spp>(b) {
    postaviBrojCvorova(b);
  }
     int dajBrojCvorova() const override{
        return lista.size();}


   void postaviBrojCvorova(int a) override{
       grana g;
       g.postoji = false;
       for(int i = 0 ; i < lista.size() ; i++)
       lista[i].resize(a,g);
       std::vector<grana> ubaci(a,g);
       lista.resize(a,ubaci);
       oznake.resize(a);
   }
   bool postojiGrana(int p, int d) override{
       return lista[p][d].postoji;
   }

   void dodajGranu(int p, int d, float t) override {
       grana g; g.tezina = t; g.postoji = true; lista[p][d] = g;
   } 
   void obrisiGranu(int p, int d) override {
        lista[p][d].postoji = false;
   }
   void postaviTezinuGrane(int p, int d, float t) override {
       lista[p][d].tezina = t;
   } 
   spp dajOznakuCvora(int b) override {
       return oznake[b];
  }
  
   spp dajOznakuGrane(int p, int d) override {
      return lista[p][d].oznaka;
  }
   float dajTezinuGrane(int p, int d) override {
       return lista[p][d].tezina;
   }
   void postaviOznakuCvora(int b, spp a) override{
       oznake[b] = a;
   }
   void postaviOznakuGrane(int p, int d, spp a)override {
      lista[p][d].oznaka = a; 
  }
  GranaIterator<spp> dajGranePocetak() { GranaIterator<spp> it(this, 0, -1); ++it; return it; } 
  GranaIterator<spp> dajGraneKraj() { GranaIterator<spp> it(this, -1, -1); return it; } 
  
  GranaIterator<spp> dajSljedecuGranu(int p, int d)override{
      for(int i=p;i<lista.size();i++){
          for(int j=0;j<lista.size();j++){
              if(i == p && j <= d)continue;

              if(lista[i][j].postoji)
              return GranaIterator<spp>(this,i,j);
          }
      }
        return GranaIterator<spp>(this,-1,-1);
  }
  
};

template <typename spp>
void bfs(UsmjereniGraf<spp> *g, std::vector<Cvor<spp>> &b, Cvor<spp> c) {
  bool dodati = false;
  for (int i = 0; i < b.size(); i++) {
    if (b[i].dajRedniBroj() == c.dajRedniBroj()) {
      dodati = true;
      break;
    }
  }
  if (!dodati)
    b.push_back(c);
  int br = g->dajBrojCvorova();
  for (int i = 0; i < br; i++)
    if (!dodati)
      bfs(g, b, g->dajCvor(i));
}

template <typename spp>
void dfs(UsmjereniGraf<spp> *g, std::vector<Cvor<spp>> &b, Cvor<spp> c) {
  bool dodati = false;
  for (int i = 0; i < b.size(); i++) {
    if (c.dajRedniBroj() == b[i].dajRedniBroj()) {
      dodati = true;
      break;
    }
  }
  if (!dodati)
    b.push_back(c);
  int brojCvorova = g->dajBrojCvorova();
  for (int i = 0; i < brojCvorova; i++)
    if (g->postojiGrana(c.dajRedniBroj(), i) && !dodati)
      dfs(g, b, g->dajCvor(i));
}

int main() {
UsmjereniGraf<bool> *g = new ListaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 0, 1.2);
    g->dodajGranu(1, 2, 0.1);
    g->dodajGranu(0, 0, 3.14);
    for (GranaIterator<bool> iter = g->dajGranePocetak();
         iter != g->dajGraneKraj(); ++iter)
      cout << "(" << (*iter).dajPolazniCvor().dajRedniBroj() << ","
           << (*iter).dajDolazniCvor().dajRedniBroj() << ") -> "
           << (*iter).dajTezinu() << "; ";
  delete g;

  return 0;
}