#include <iostream>

template <typename Tip>
struct Cvor{
    Tip element;
    Cvor* veza;
};

template <typename Tip> class Stek{
    Cvor<Tip> *vrh;
public:
    Stek() : vrh(nullptr) {}

    ~Stek(){
        while(vrh){
            Cvor<Tip> *temp = vrh;
            vrh = vrh->veza;
            delete temp;
            temp = nullptr;
        }
    }

    Stek(const Stek &s) = delete;
    Stek &operator=(const Stek &s) = delete;

    Stek(Stek &&s) : vrh(s.vrh) {s.vrh = nullptr;}
    Stek &operator=(Stek &&s){
        std::swap(vrh, s.vrh);
        return *this;
    }

    int DajVelicinu() const{
        int broj{};
        for(Cvor<Tip>* temp = vrh; temp!=nullptr; temp=temp->veza) broj++;
        return broj;
    }

    bool DaLiJePrazan() {return DajVelicinu() == 0;}

    Tip &DajVrh() {
        if(DaLiJePrazan()) throw std::domain_error("Prazan stek!");
        return vrh->element;
    }

    const Tip DajVrh() const{
        if(DaLiJePrazan()) throw std::domain_error("Prazan stek!");
        return vrh->element;
    }

    void SkiniSaVrha(){
        if(DaLiJePrazan()) throw std::domain_error("Prazan stek!");
        Cvor<Tip>* temp = vrh;
        vrh=vrh->veza;
        delete temp;
        temp=nullptr;
    }
    //1 2 3 4 65 6 x
    void DodajNaVrh(const Tip &x){
        Cvor<Tip>* temp = new Cvor<Tip>{x, vrh};
        vrh=temp;
    }

};

int main(){

    Stek<int> s;
    s.DodajNaVrh(3); s.DodajNaVrh(5); s.DodajNaVrh(0); s.DodajNaVrh(2);
    while(!s.DaLiJePrazan()){
        std::cout<<s.DajVrh()<<" ";
        s.SkiniSaVrha();
    }

    return 0;
}