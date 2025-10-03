#include <iostream>

struct Osoba{
    int broj;
    Osoba* next;
};

int SigurnaPozicija(int n, int m){

    Osoba* prva = new Osoba{1, nullptr};
    Osoba* prethodna{};

    prethodna = prva;

    for(int i=2; i<=n; i++){
        Osoba* nova = new Osoba{i, nullptr};
        prethodna->next = nova;
        prethodna = nova;
    }

    prethodna->next = prva;

    

}