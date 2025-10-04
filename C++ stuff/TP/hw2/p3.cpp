//TP 2023/2024: Zadaća 2, Zadatak 3
#include <iostream>
#include <cmath>
#include <iterator>
#include <limits>
#include <vector>
#include <deque>
#include <algorithm>

template <typename prvi_iter, typename drugi_iter, typename neki_mix>
auto UnijaBlokova(prvi_iter p1, prvi_iter p2, drugi_iter p3, drugi_iter p4, neki_mix p5){
    auto pomocni=p5;

    for(auto p=p1; p!=p2; p++){
        if(std::find(p5, pomocni, *p) == pomocni){
            *pomocni=*p;
            pomocni++;
        }
    }

    for(auto p=p3; p!=p4; p++){
        if(std::find(p5, pomocni, *p) == pomocni){
            *pomocni=*p;
            pomocni++;
        }
    }

    return pomocni;
}

int main ()
{
    std::cout<<"Unesite broj elemenata niza (max. 100): ";
    int n{};
    std::cin>>n;
    double niz[100];
    std::cout<<"Unesite elemente niza: ";
    for(int i=0; i<n; i++) std::cin>>niz[i];

    std::cout<<"Unesite broj elemenata deka: ";
    int m{};
    std::cin>>m;
    std::cout<<"Unesite elemente deka: ";
    std::deque<int> d(m);
    for(int &i: d) std::cin>>i;

    std::cout<<"Njihova unija glasi: ";
    std::vector<double> v(100);
    auto kraj=UnijaBlokova(std::begin(niz), (niz+n), d.begin(), d.end(), v.begin());
    v.resize(std::distance(v.begin(), kraj));
    for(const auto &i : v) std::cout<<i<<" ";

	return 0;
}