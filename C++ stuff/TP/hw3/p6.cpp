//TP 2023/2024: Zadaća 3, Zadatak 6
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <memory>

struct Distrikt{
    int broj_distrikta;
    std::shared_ptr<Distrikt> sljedeci;
};

std::vector<int> Razbrajanje(int n, int m){
    if(n<=0 || m<=0) throw std::domain_error("Broj blokova i redni broj bloka su pozitivni cijeli brojevi i redni broj bloka ne moze biti veci od broja blokova");

    std::shared_ptr<Distrikt> pocetak, prev;
    for(int i=1; i<=n; i++){
        auto novi = std::make_shared<Distrikt>();
        novi->broj_distrikta = i;
        novi->sljedeci = nullptr;
        if(!pocetak) pocetak=novi;
        else prev->sljedeci=novi;
        prev=novi;
    }
    prev->sljedeci=pocetak;

    std::vector<int> red;
    auto p = pocetak;

    red.push_back(p->broj_distrikta);
    prev->sljedeci = p->sljedeci;
    p=prev->sljedeci;

    while(p->sljedeci!=p){
        for(int i=0; i<m-1; i++){
            prev=p;
            p=p->sljedeci;
        }
        red.push_back(p->broj_distrikta);
        prev->sljedeci = p->sljedeci;
        p=prev->sljedeci;
    }
    red.push_back(p->broj_distrikta);
    p->sljedeci=nullptr;

    return red;
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
