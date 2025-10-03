#include <string>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <set>
#include <type_traits>
#include <stack>
#include <queue>
#include <algorithm>

//Austriija

void Ruraliziraj(std::string &s){
    for(int i=0; i<s.length(); i++){
        char pom = s.at(i);
        if(std::string("AEIOUaeiou").find(pom) != std::string::npos){
            s.insert(s.begin() + i + 1, std::tolower(pom));
            i++;
        }
    }
}

double* Tabeliraj(std::function<double(double)> f, double xmin, double xmax, int n){
    if(n <= 2 || xmin > xmax) throw std::domain_error("Neispravni parametri");

    double* rez = new double[n];

    double korak = (xmax-xmin)/(n-1);

    for(int i=0; i<n; i++) 
        rez[i] = f(xmin + i*korak);

    return rez;
}

/* int SumaDjelilaca(int n){
    int suma{1};
    for(int i=2; i<=std::sqrt(n); i++){
        if(n%i == 0) suma+=i;
    }
    return suma;
} */

int BrojPrijateljskihParova(const std::vector<int> &v){
    int brojac{};

    /* auto SumaDjelilaca = [](int n){
        int suma{1};
        for(int i=2; i<n; i++){
            if(n%i == 0) suma+=i;
        }
        return suma;
    }; */

    for(int i=0; i<v.size(); i++){
        for(int j=i+1; j<v.size(); j++){

            int suma1{1},suma2{1};
            for(int i=2; i<v.at(i); i++) 
                if(v.at(i)%i == 0) suma1+=i;

            for(int i=2; i<v.at(j); i++) 
                if(v.at(j)%i == 0) suma2+=i;

            if(suma1 == v.at(j) && suma2 == v.at(i))
                brojac++;
        }
    }
    return brojac;
}

void Minimax(const std::vector<std::vector<double>> &m, int &minimax, int &maximin){
    if(m.empty() || m.at(0).empty()) throw std::domain_error("Neispravna matrica!");

    for(const auto &red : m)
        if(red.size() != m.at(0).size()) throw std::domain_error("Parametar nema formu matrice!");

    minimax = 999999;
    maximin = 0.000001;
    for(int i=0; i<m.size(); i++){
        int najveciReda = m.at(i).at(0);
        int najmanjiReda = m.at(i).at(0);

        for(int j=0; j<m.at(i).size(); j++){
            if(m.at(i).at(j) > najveciReda) najveciReda = m.at(i).at(j);
            if(m.at(i).at(j) < najmanjiReda) najmanjiReda= m.at(i).at(j);
        }

        if(najveciReda < minimax) minimax = najveciReda;
        if(najmanjiReda > maximin) maximin = najmanjiReda;
    }
}

//i corava koka
//

template<typename Tip1, typename Tip2>
auto KopirajBezDuplikata(Tip1 p1, Tip1 p2, Tip2 p3){
    std::set<std::remove_reference_t<decltype(*p1)>> skup;

    for(auto it=p1; it!= p2; it++){
        if(skup.insert(*it).second){
            *p3 = *it;
            p3++;
        }
    }

    return p3;
}

/*Napišite funkciju koja kao jedan parametar prima neki stek realnih brojeva, a kao drugi parametar
neki realni broj. Funkcija kao rezultat treba da vrati novi stek realnih brojeva čiji su elementi jednaki
elementima izvornog steka pomnoženi sa zadanim brojem. Na primjer, ukoliko se u steku koji je
proslijeđen kao parametar nalaze brojevi 2, 5, 3 i 4 (tim redom), i ukoliko se kao drugi parametar
proslijedi broj 3, stek vraćen kao rezultat treba da sadrži brojeve 6, 15, 9 i 12 (tim redom). Podsjetimo
se da je stek kontejnerska struktura koja ne podržava ništa drugo osim operacija “push” (dodaj novi
element na vrh steka), “top” (daj element koji je na vrhu steka), “pop” (ukloni element sa vrha steka),
“size” (daj broj elemenata u steku) i “empty” (daj informaciju da li je stek prazan ili ne). Za realizaciju
ove funkcije ne smijete koristiti nikakvu pomoćnu funkciju, osim eventualno jednog pomoćnog steka
ukoliko smatrate da Vam je potreban.*/

std::stack<double> PomnoziStek(std::stack<double> stek, double faktor){
    std::stack<double> pomocni;

    while(!stek.empty()){
        double a = stek.top();
        pomocni.push(a);
        stek.pop();
    }

    while(!pomocni.empty()){
        double b = pomocni.top();
        stek.push(b*faktor);
        pomocni.pop();
    }
    
    return stek;
}

int main(){

    /* std::string recenica;
    std::getline(std::cin, recenica);
    Ruraliziraj(recenica);
    std::cout<<std::endl<<recenica; */

    /* try{
        auto func = [](double x){return x*x*x;};

        double* niz = Tabeliraj(func, 0.0, 5.0, 11);

        for(int i=0; i<=10; i++)
            std::cout<<niz[i]<<" ";

        delete[] niz;

    }
    catch(std::domain_error &e){
        std::cout<<"Doslo je do greske: "<<e.what();
    } */

    /* int n;
    std::cout<<"Koliko ima brojeva? ";
    std::cin>>n;
    std::vector<int> v(n);
    std::cout<<"Unesite brojeve: ";
    for(int &x : v) std::cin>>x;
    std::cout<<"Broj prijateljskih parova u vektoru je "<<BrojPrijateljskihParova(v); */

    /* try{
        int m, n;
        std::cout<<"Unesite dimenzije matrice: ";
        std::cin>>m>>n;
        std::vector<std::vector<double>> matrica(m, std::vector<double>(n));
        std::cout<<"Unesite elemente: ";
        for(auto &red : matrica){
            for(auto &element : red) std::cin>>element;
        }
        int maxm, minm;
        Minimax(matrica, minm, maxm);
        std::cout<<"Minimax vrijednost po redovima: "<<minm<<std::endl<<"Maximin vrijednost po redovima: "<<maxm;
    }
    catch(std::domain_error e){
        std::cout<<e.what();
    } */

    /* std::string s = "I corava koka gozdena vrata otvara";
    std::string odrediste;
    KopirajBezDuplikata(s.begin(), s.end(), std::back_inserter(odrediste));
    std::cout<<odrediste; */

    /* std::stack<double> s;
    s.push(4);
    s.push(3);
    s.push(5);
    s.push(2);
    auto s2 = PomnoziStek(s, 3);
    while(!s2.empty()){
        std::cout<<s2.top()<<" ";
        s2.pop();
    } */

    std::set<int> s;
s.insert(6); s.insert(3); s.insert(9); s.insert(6); s.insert(1); s.insert(8);
for(auto i = s.rbegin(); i != s.rend(); i++) std::cout << (*i) << " ";

    return 0;
}