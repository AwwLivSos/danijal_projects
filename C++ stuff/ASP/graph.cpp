#include <stdexcept>
#include <vector>
#include <iostream>

template <typename TipOznake> class Grana;
template <typename TipOznake> class Cvor;
template <typename TipOznake> class GranaIterator;

template <typename TipOznake> class UsmjereniGraf {
public:
    UsmjereniGraf(int p) {}
    virtual ~UsmjereniGraf() {}
    
    virtual void postaviBrojCvorova(int) = 0;
    virtual void postaviTezinuGrane(int, int, double) = 0;
    virtual void postaviOznakuCvora(int, TipOznake) = 0;
    virtual void postaviOznakuGrane(int, int, TipOznake) = 0;

    virtual void dodajGranu(int, int, double) = 0;
    virtual void obrisiGranu(int, int) = 0;

    virtual int dajBrojCvorova() const = 0;
    virtual double dajTezinuGrane(int, int)  = 0;
    virtual TipOznake dajOznakuCvora(int)  = 0;
    virtual TipOznake dajOznakuGrane(int, int)  = 0;
    virtual Grana<TipOznake> &dajGranu(int, int) = 0;
    virtual Cvor<TipOznake> &dajCvor(int) = 0;
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;

    virtual bool postojiGrana(int, int) = 0;
};

template <typename TipOznake> class Grana {
    int pocetak, kraj;
    double tezina;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake> *pripadajuciGraf;

public:
    Grana(int p, int k, UsmjereniGraf<TipOznake> *g, double t) : pocetak(p), kraj(k), pripadajuciGraf(g), tezina(t) {}

    TipOznake dajOznaku() const {return oznaka;}
    double dajTezinu() const {return tezina;}
    Cvor<TipOznake> dajPolazniCvor() {return pripadajuciGraf -> dajCvor(pocetak);}
    Cvor<TipOznake> dajDolazniCvor() {return pripadajuciGraf -> dajCvor(kraj);}

    void postaviOznaku(TipOznake o) {
        oznaka = o;
        pripadajuciGraf -> dajGranu(pocetak, kraj).oznaka = o;
    }
    void postaviTezinu(double t) {
        tezina = t;
        pripadajuciGraf -> dajGranu(pocetak, kraj).tezina = t;
    } 
};

template <typename TipOznake>
class Cvor {
    int redniBroj;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake> *pripadajuciGraf;

public:
    Cvor(UsmjereniGraf<TipOznake> *g, int rb) : pripadajuciGraf(g), redniBroj(rb) {}

    int dajRedniBroj() const {return redniBroj;}
    TipOznake dajOznaku() const {return oznaka;}
    void postaviOznaku(TipOznake o) {oznaka = o;}
};

template <typename TipOznake> class GranaIterator {

    int dolazniCvor, polazniCvor, pozicija;
    std::vector<Grana<TipOznake>> grane;
    std::vector<std::vector<bool>> vrijednosti;

    void trazi(int &brojac, int &red, int &kolona, bool uslov);
public:
    GranaIterator(std::vector<std::vector<bool>> v, std::vector<Grana<TipOznake>> g, int p, int k, int poz) : pozicija(poz), grane(g), vrijednosti(v), dolazniCvor(k), polazniCvor(p){}
    
    Grana<TipOznake> operator *() {
        int brojac = 0, red = 0, kolona = 0;
        trazi(brojac, red, kolona, false);
        for(int i = 0; i < grane.size(); i++) {
            if(grane[i].dajPolazniCvor().dajRedniBroj() == red && grane[i].dajDolazniCvor().dajRedniBroj() == kolona)
                return grane[i];
        }
        throw std::logic_error("Trazena grana ne postoji");
    }

    bool operator ==(const GranaIterator &it) const {
        int brojac = 0, red = 0, kolona = 0;
        trazi(brojac, red, kolona, false);
        if(vrijednosti[red][kolona] == it.vrijednosti[red][kolona]) return true;
        return false;
    }

    bool operator !=(GranaIterator it) {
        int brojac = 0, red = 0, kolona = 0;
        trazi(brojac, red, kolona, false);
        if(vrijednosti[red][kolona] != it.vrijednosti[red][kolona]) return false;
        if(pozicija == grane.size()) return false;
        return true;
    }

    bool operator !=(const GranaIterator &it) const {
        int brojac = 0, red = 0, kolona = 0;
        trazi(brojac, red, kolona, false);
        if(vrijednosti[red][kolona] != it.vrijednosti[red][kolona]) return false;
        if(pozicija == grane.size()) return false;
        return true;
    }

    GranaIterator &operator ++() {
        pozicija++;
        return *this;
    }
    GranaIterator operator ++(int) {
        GranaIterator temp = *this;
        ++*this;
        return temp;
    }
};

template<typename TipOznake>
void GranaIterator<TipOznake>::trazi(int &brojac, int &red, int &kolona, bool uslov)  {
    for(int i = 0; i < vrijednosti.size(); i++) {
        for(int j = 0; j < vrijednosti[i].size(); j++) {
            if(brojac <= pozicija && vrijednosti[i][j]) {
                brojac++;
                red = i;
                kolona = j;
            }
            else if(brojac > pozicija) {
                uslov = true;
                break;
            }
        }
        if(uslov) break;
    }
}

template <typename TipOznake> class MatricaGraf : public UsmjereniGraf<TipOznake> {
    std::vector<Grana<TipOznake>> grane;
    std::vector<Cvor<TipOznake>> cvorovi;
    std::vector<std::vector<bool>> vrijednosti;
public:
    MatricaGraf(int bc) : UsmjereniGraf<TipOznake>(bc) {
        if(bc < 0) throw std::logic_error("Broj cvorova ne smije biti negativan!");
        vrijednosti.resize(bc);

        for(int i = 0; i < bc; i++) {
            vrijednosti[i].resize(bc);
            cvorovi.push_back(Cvor<TipOznake>(this, i));
            for(int j = 0; j < bc; j++) vrijednosti[i][j] = false;
        }
    }

    ~MatricaGraf() {}

    MatricaGraf(const MatricaGraf<TipOznake> &mg) {
        grane = mg.grane;
        cvorovi = mg.cvorovi;
        vrijednosti = mg.vrijednosti;
    }

    MatricaGraf<TipOznake> &operator =(MatricaGraf<TipOznake> mg) {
        std::swap(grane, mg.grane);
        std::swap(cvorovi, mg.cvorovi);
        std::swap(vrijednosti, mg.vrijednosti);
        return *this;
    }

    Cvor<TipOznake> &dajCvor(int i) override {return cvorovi[i];}
    GranaIterator<TipOznake> dajGranePocetak() override {return GranaIterator<TipOznake>(vrijednosti, grane, grane[0].dajPolazniCvor().dajRedniBroj(), grane[0].dajDolazniCvor().dajRedniBroj(), 0);}
    GranaIterator<TipOznake> dajGraneKraj() override {return GranaIterator<TipOznake>(vrijednosti, grane, grane[grane.size() - 1].dajPolazniCvor().dajRedniBroj(), grane[grane.size() - 1].dajDolazniCvor().dajRedniBroj(), 0);}
    void postaviOznakuCvora(int rb, TipOznake o) override {cvorovi[rb].postaviOznaku(o);}
    bool postojiGrana(int p, int k) override {return vrijednosti[p][k];}
    int dajBrojCvorova() const override {return cvorovi.size();}

    void postaviBrojCvorova(int bc) override {
        if(bc < cvorovi.size())  throw std::logic_error("Ne smije broj cvorova biti manji od prethodnog");
        if(vrijednosti.size() == bc) return;

        for(int i = vrijednosti.size(); i < bc; i++) cvorovi.push_back(Cvor<TipOznake>(this, i));
        
        int staraDimenzijaMatrice = vrijednosti.size();
        vrijednosti.resize(bc);
        int ubaci = 0;

        for(int i = 0; i < bc; i++) {
            vrijednosti[i].resize(bc);
            if(i >= staraDimenzijaMatrice) ubaci = 0;
            else ubaci = staraDimenzijaMatrice;

            for(int j = ubaci; j < bc; j++) vrijednosti[i][j] = false;
        }
    }

    void postaviTezinuGrane(int p, int k, double t) override {
        for(int i = 0; i < grane.size(); i++) {
            if(p == grane[i].dajPolazniCvor().dajRedniBroj() && k == grane[i].dajDolazniCvor().dajRedniBroj()) {
                grane[i].postaviTezinu(t);
                return;
            }
        }
        throw std::logic_error("Ne mozete postaviti tezinu jer data grana ne postoji!");
    }

    void postaviOznakuGrane(int p, int k, TipOznake o) override {
        for(int i = 0; i < grane.size(); i++) {
            if(p == grane[i].dajPolazniCvor().dajRedniBroj() && k == grane[i].dajDolazniCvor().dajRedniBroj()) {
                grane[i].postaviOznaku(o);
                return;
            }
        }
        throw std::domain_error("Ne mozete postaviti oznaku jer data grana ne postoji!");
    }

    void dodajGranu(int p, int k, double t = 0) override {
        if(!postojiGrana(p, k)) {
            vrijednosti[p][k] = true;
            grane.push_back(Grana<TipOznake>(p, k, this, t));
        }
        else throw std::logic_error("Grana vec postoji!");
    }

    void obrisiGranu(int p, int k) override {
        for(int i = 0; i < grane.size(); i++) {
            if(grane[i].dajPolazniCvor().dajRedniBroj() == p && grane[i].dajDolazniCvor().dajRedniBroj() == k) {
                vrijednosti[p][k] = false;
                grane.erase(grane.begin() + i);
                return;
            }
        }
        throw std::logic_error("Granu koju zelite obrisati ne postoji!");
    }

    double dajTezinuGrane(int p, int k)  override {
        for(int i = 0; i < grane.size(); i++) {
            if(p == grane[i].dajPolazniCvor().dajRedniBroj() && k == grane[i].dajDolazniCvor().dajRedniBroj()) {
                return grane[i].dajTezinu();
            }
        }
        throw std::logic_error("Trazena grana ne postoji!");
    }

    TipOznake dajOznakuCvora(int rb)  override {
        return cvorovi[rb].dajOznaku();
        throw std::logic_error("Trazeni cvor ne postoji!");
    }

    TipOznake dajOznakuGrane(int p, int k)  override {
        for(int i = 0; i < grane.size(); i++) {
            if(p == grane[i].dajPolazniCvor().dajRedniBroj() && k == grane[i].dajDolazniCvor().dajRedniBroj()) {
                return grane[i].dajOznaku();
            }
        }
        throw std::logic_error("Trazena grana ne postoji!");
    }

    Grana<TipOznake> &dajGranu(int p, int k) override {
        for(int i = 0; i < grane.size(); i++) {
            if(grane[i].dajPolazniCvor().dajRedniBroj() == p && grane[i].dajDolazniCvor().dajRedniBroj() == k)
                return grane[i];
        }
        throw std::logic_error("Trazena grana ne postoji!");
    }
};

using namespace std;

int main(){
  try {
    UsmjereniGraf<bool> *g = new MatricaGraf<bool>(3);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 0, 1.2);
    g->dodajGranu(1, 2, 0.1);
    g->dodajGranu(0, 0, 3.14);
    for (GranaIterator<bool> iter = g->dajGranePocetak();
         iter != g->dajGraneKraj(); ++iter)
      cout << (*iter).dajPolazniCvor().dajRedniBroj() << " "
           << (*iter).dajDolazniCvor().dajRedniBroj() << " "
          << (*iter).dajTezinu() << endl;
    delete g;
  } catch (const char izuzetak[]) {
    cout << izuzetak << endl;
  }
  return 0;
}