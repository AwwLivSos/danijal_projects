#include <iostream>
#include <stdexcept>
#include <utility>

template <typename TipK, typename TipV>
class Mapa {
public:
    virtual ~Mapa() = default;
    virtual TipV &operator[](const TipK &kljuc) = 0;
    virtual const TipV &operator[](const TipK &kljuc) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi(const TipK &kljuc) = 0;
    virtual void obrisi() = 0;
};

template <typename TipK, typename TipV>
class AVLStabloMapa : public Mapa<TipK, TipV> {
    struct Cvor {
        TipK kljuc;
        TipV vrijednost;
        int balans;
        Cvor *lijevi, *desni, *roditelj;
        Cvor(const TipK &kljuc, Cvor *roditelj = nullptr)
            : kljuc(kljuc), vrijednost(TipV{}), balans(0), lijevi(nullptr), desni(nullptr), roditelj(roditelj) {}
    };

    Cvor *korijen;
    int velicina;

    Cvor *Pretrazi(Cvor *c, const TipK &kljuc) const {
        if (!c || c->kljuc == kljuc)
            return c;
        if (kljuc < c->kljuc)
            return Pretrazi(c->lijevi, kljuc);
        return Pretrazi(c->desni, kljuc);
    }

    int visina(Cvor *c) const {
        if (!c)
            return 0;
        return 1 + std::max(visina(c->lijevi), visina(c->desni));
    }

    void azuriraj_balans(Cvor *c) {
        while (c) {
            c->balans = visina(c->lijevi) - visina(c->desni);
            if (c->balans < -1 || c->balans > 1)
                balansiraj(c);
            c = c->roditelj;
        }
    }

    void balansiraj(Cvor *c) {
        if (c->balans > 1) {
            if (c->lijevi->balans < 0)
                rotiraj_lijevo(c->lijevi);
            rotiraj_desno(c);
        } else if (c->balans < -1) {
            if (c->desni->balans > 0)
                rotiraj_desno(c->desni);
            rotiraj_lijevo(c);
        }
    }

    void rotiraj_lijevo(Cvor *c) {
        Cvor *temp = c->desni;
        c->desni = temp->lijevi;
        if (temp->lijevi)
            temp->lijevi->roditelj = c;
        temp->roditelj = c->roditelj;
        if (!c->roditelj)
            korijen = temp;
        else if (c->roditelj->lijevi == c)
            c->roditelj->lijevi = temp;
        else
            c->roditelj->desni = temp;
        temp->lijevi = c;
        c->roditelj = temp;
    }

    void rotiraj_desno(Cvor *c) {
        Cvor *temp = c->lijevi;
        c->lijevi = temp->desni;
        if (temp->desni)
            temp->desni->roditelj = c;
        temp->roditelj = c->roditelj;
        if (!c->roditelj)
            korijen = temp;
        else if (c->roditelj->desni == c)
            c->roditelj->desni = temp;
        else
            c->roditelj->lijevi = temp;
        temp->desni = c;
        c->roditelj = temp;
    }

    void obrisiSve(Cvor *c) {
        if (!c)
            return;
        obrisiSve(c->lijevi);
        obrisiSve(c->desni);
        delete c;
    }

    Cvor *obrisiJedanCvor(Cvor *c, const TipK &kljuc) {
        if (!c)
            return nullptr;

        if (kljuc < c->kljuc) {
            c->lijevi = obrisiJedanCvor(c->lijevi, kljuc);
        } else if (kljuc > c->kljuc) {
            c->desni = obrisiJedanCvor(c->desni, kljuc);
        } else {
            if (!c->lijevi && !c->desni) {
                delete c;
                velicina--;
                return nullptr;
            } else if (!c->lijevi || !c->desni) {
                Cvor *temp = c->lijevi ? c->lijevi : c->desni;
                temp->roditelj = c->roditelj;
                delete c;
                velicina--;
                return temp;
            } else {
                Cvor *zamjena = c->desni;
                while (zamjena->lijevi)
                    zamjena = zamjena->lijevi;
                c->kljuc = zamjena->kljuc;
                c->vrijednost = zamjena->vrijednost;
                c->desni = obrisiJedanCvor(c->desni, zamjena->kljuc);
            }
        }

        return c;
    }

    void umetni(Cvor *&c, const TipK &kljuc, Cvor *roditelj) {
        if (!c) {
            c = new Cvor(kljuc, roditelj);
            velicina++;
        } else if (kljuc < c->kljuc) {
            umetni(c->lijevi, kljuc, c);
        } else if (kljuc > c->kljuc) {
            umetni(c->desni, kljuc, c);
        }
        azuriraj_balans(c);
    }
    void preorder_recursive(Cvor* c) const{
        if(!c) return;
        std::cout<<c->vrijednost<<",";
        preorder_recursive(c->lijevi);
        preorder_recursive(c->desni);
    }
    
    Cvor *kopiraj(Cvor *c, Cvor *roditelj = nullptr) {
        if (!c)
            return nullptr;
        Cvor *novi = new Cvor(c->kljuc, roditelj);
        novi->vrijednost = c->vrijednost;
        novi->balans = c->balans;
        novi->lijevi = kopiraj(c->lijevi, novi);
        novi->desni = kopiraj(c->desni, novi);
        return novi;
    }

public:
    AVLStabloMapa() : korijen(nullptr), velicina(0) {}
    ~AVLStabloMapa() { obrisi(); }
    AVLStabloMapa(const AVLStabloMapa &drugi) : korijen(nullptr), velicina(0) {
        korijen = kopiraj(drugi.korijen);
        velicina = drugi.velicina;
    }
    
    AVLStabloMapa& operator=(AVLStabloMapa drugo) {
        std::swap(korijen, drugo.korijen);
        std::swap(velicina, drugo.velicina);
        return *this;
    }

    TipV &operator[](const TipK &kljuc) override {
        auto c = Pretrazi(korijen, kljuc);
        if (!c) {
            umetni(korijen, kljuc, nullptr);
            c = Pretrazi(korijen, kljuc);
        }
        return c->vrijednost;
    }

    const TipV &operator[](const TipK &kljuc) const override {
        auto c = Pretrazi(korijen, kljuc);
        if (!c){
            static TipV v{};
            return v;
        }
            
        return c->vrijednost;
    }

    int brojElemenata() const override { return velicina; }

    void obrisi(const TipK &kljuc) override { korijen = obrisiJedanCvor(korijen, kljuc); }

    void obrisi() override {
        obrisiSve(korijen);
        korijen = nullptr;
        velicina = 0;
    }
    void Preorder() const{
        preorder_recursive(korijen);
        std::cout<<std::endl;
    }
};

using namespace std;
void testSequentialInsert() {
    AVLStabloMapa<int, int> avl;
    for (int i = 1; i <= 10; i++) {
        avl[i] = i;
    }
    avl.Preorder();
}

void testReverseInsert() {
    AVLStabloMapa<int, int> avl;
    for (int i = 10; i >= 1; i--) {
        avl[i] = i;
    }
    avl.Preorder();
}

void testRandomInsert() {
    AVLStabloMapa<int, int> avl;
    int keys[] = {5, 2, 8, 6, 33, 57, 1, 10, 40, 9};
    for (int key : keys) {
        avl[key] = key;
    }
    avl.Preorder();
}

void testDuplicateInsert() {
    AVLStabloMapa<int, int> avl;
    for (int i = 1; i <= 5; i++) {
        avl[i] = i;
        avl[i] = i * 10;
    }
    avl.Preorder();
}

void testMixedOperations() {
    AVLStabloMapa<int, int> avl;
    avl[5] = 5;
    avl[2] = 2;
    avl[8] = 8;
    avl[6] = 6;
    avl[3] = 3;
    avl[10] = 10;
    avl.obrisi(8);   
    avl[9] = 9;      
    avl[4] = 4;      
    avl.Preorder();
}

int main(){
    testDuplicateInsert();
    testMixedOperations();
    testRandomInsert();
    testReverseInsert();
    testSequentialInsert();
    
    return 0;
}