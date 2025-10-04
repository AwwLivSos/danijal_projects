//TP 2023/2024: Zadaća 3, Zadatak 5
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

struct Distrikt{
    int broj_distrikta;
    Distrikt* sljedeci;
};

std::vector<int> Razbrajanje(int n, int m){
    if(n<=0 || m<=0) throw std::domain_error("Broj blokova i redni broj bloka su pozitivni cijeli brojevi i redni broj bloka ne moze biti veci od broja blokova");

    Distrikt *pocetak{}, *prev;
    try{
        for(int i=1; i<=n; i++){
            Distrikt *novi = new Distrikt{i, nullptr};
            if(!pocetak) pocetak=novi;
            else prev->sljedeci=novi;
            prev=novi;
        }
    }catch(...){
        Distrikt* p = pocetak;
        while (p) {
            Distrikt* temp = p;
            p = p->sljedeci;
            delete temp;
        }
        throw;
    }
    prev->sljedeci=pocetak;

    Distrikt* p=pocetak;
    std::vector<int> povratni;

    povratni.push_back(p->broj_distrikta);
        
    prev->sljedeci = p->sljedeci;
    delete p;
    p=prev->sljedeci;

    while(p->sljedeci!=p){
        for(int i=0; i<m-1; i++){
            prev=p;
            p=p->sljedeci;
        }
        povratni.push_back(p->broj_distrikta);
        
        prev->sljedeci = p->sljedeci;
        delete p;
        p=prev->sljedeci;
    }
    povratni.push_back(p->broj_distrikta);
    delete p;

    return povratni;
}

int OdabirKoraka(int n, int k){
    if(k>n || k<=0) throw std::domain_error("Broj blokova i redni broj bloka su pozitivni cijeli brojevi i redni broj bloka ne moze biti veci od broja blokova");
    if(n==1) return 1;
    int m{};
    std::vector<int> potencijali;
    for(int i=1; i<n*n; i++) 
        if(Razbrajanje(n, i).at(n-1)==k) potencijali.push_back(i);
    if(!potencijali.empty()) 
        return *std::min_element(potencijali.begin(), potencijali.end());
    return m;
}

int main ()
{
    int n, k;
    std::cout<<"Unesite broj distrikta u gradu: ";
    std::cin>>n;
    std::cout<<"Unesite redni broj distrikta u kojem se nalazi restoran: ";
    std::cin>>k;
    try{
        auto rez = OdabirKoraka(n, k);
        if(rez) std::cout<<"Trazeni korak: "<<rez;
        else std::cout<<"Ovo nazalost nije moguce.";
    }
    catch(std::domain_error e){
        std::cout<<e.what();
    }

	return 0;
}