//TP 2023/2024: Primjer parcijalnog ispita 3, neki zadaci
#include <algorithm>
#include <functional>
#include <iostream>
#include <cmath>
#include <complex>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <deque>

//SESTI ZADATAK
std::string Izdvoji(std::string s, int n){
    std::istringstream iss(s);
    std::string rijec;
    for(int i=0; i<n; i++) iss>>rijec;
    return rijec;
}
//KRAJ

//TRECI ZADATAK
std::function<int(int)> IteriranaFunkcija(std::function<int(int)> f, int n){
    return [n, f](int x){
        for(int i=0; i<n; i++) x=f(x);
        return x;
    };
}
//KRAJ

//PETI ZADATAK
template <typename Tip>
    std::vector<Tip> ZajednickiElementi(std::vector<Tip> v1, std::vector<Tip> v2){
        std::set<Tip> skup;
        for(const auto &x : v1) if(std::find(v2.begin(), v2.end(), x) != v2.end()) skup.insert(x);

        return std::vector<Tip>(skup.begin(), skup.end());          //konstruktor vektora koji prihvata dva iteratora (°o°)
    }
//KRAJ

//SEDMI ZADATAK
int PeriodListe(const std::list<double>& lista) {
    int n = lista.size();
    if (n == 0) return 0;

    for (int T = 1; T < n; T++) {
        bool periodic = true;
        auto it1 = lista.begin();
        auto it2 = std::next(lista.begin(), T);

        while (it2 != lista.end()) {
            if (*it1 != *it2) { 
                periodic = false;
                break;
            }
            it1++;
            it2++;
        }

        if (periodic) return T;
    }

    return 0;
}
//KRAJ

//OSMI ZADATAK
bool DaLiJeCirkularna(const std::deque<std::deque<char>>& matrica) {
    int redovi = matrica.size();
    if (redovi == 0) return false;

    int kolone = matrica[0].size();
    for (int i = 0; i < redovi; ++i) {
        if (matrica[i].size() != kolone) return false; 
    }

    for (int i = 1; i < redovi; ++i) {
        for (int j = 0; j < kolone; ++j) {
            int prev_index = (j - 1 + kolone) % kolone;
            if (matrica[i][j] != matrica[i - 1][prev_index]) {
                return false;
            }
        }
    }
    return true;
}
//KRAJ

int main ()
{   
    //MAIN ZA SESTI ZADATAK
    std::cout<<Izdvoji("   Riba     Ribi      grize   rep      i    tako    dalje  ", 3);
    //KRAJ

    //MAIN ZA TRECI ZADATAK
    std::cout<<IteriranaFunkcija([](int x){return 3*x-5;}, 6)(0);
    //KRAJ   

    //MAIN ZA PETI ZADATAK
    std::vector<std::string> vec1 = {"ovdje", "nesto", "pise"};
    std::vector<std::string> vec2 = {"a", "i", "ovdje", "isto"};
    auto s = ZajednickiElementi(vec1, vec2);
    for(auto x : s) std::cout<<x<<" ";
    //KRAJ

    //CETVRTI ZADATAK
    std::string** niz;
    std::cout<<"Unesite broj recenica: ";
    int n;
    std::cin>>n;
    std::cin.clear();
    std::cin.ignore(99999, '\n');
    std::cout<<"Unesite recenice: ";
    try{
        niz = new std::string*[n]{};
        std::string recenica;
        for(int i=0; i<n; i++){
            std::getline(std::cin, recenica);
            niz[i] = new std::string(recenica);
        }
        std::sort(niz, niz+n, [](std::string* s1, std::string* s2){return *s1<*s2;});
        for(int i = 0; i<n; i++) std::cout<<*niz[i]<<std::endl;
    }
    catch(...){
        for(int i=0; i<n; i++) delete niz[i];
        delete[] niz;
        std::cout<<"No memory :(";
    }
    //KRAJ

    //MAIN ZA SEDMI ZADATAK
    std::list<double> v = {5, 9, 7, 2, 5, 9, 7, 2};
    std::cout<< PeriodListe(v);
    //KRAJ

    //MAIN ZA OSMI ZADATAK
    std::deque<std::deque<char>> mat = {
        {'a', 'b', 'c', 'd'},
        {'d', 'a', 'b', 'c'},
        {'c', 'd', 'a', 'b'},
        {'b', 'c', 'd', 'a'}
    };
    std::cout<<DaLiJeCirkularna(mat);
    //KRAJ

    return 0;
}
