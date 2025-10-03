#include <iostream>

class Bazna{
public:
    virtual void Ispis(){
        std::cout<<"Pozdrav iz bazne klase";
    }
};

class Nasljedjena : public Bazna{
public:
    void Ispis() override {
        std::cout<<"Pozdrav iz nasljedjene klase";
    }
};

int main(){

    Bazna* b = new Nasljedjena;
    b->Ispis();

    return 0;
}