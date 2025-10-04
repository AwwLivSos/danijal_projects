//TP 2023/2024: Zadaća 2, Zadatak 2
#include <algorithm>
#include <deque>
#include <exception>
#include <iostream>
#include <cmath>
#include <iterator>
#include <stdexcept>
#include <vector>
#include <iomanip>

template <template <typename...> class cont1, typename cont2>
cont1<cont2> Matrica(int m, int n){
    return cont1<cont2> (m, cont2(n));
}

double najveci(const std::vector<std::vector<double>> &m){
    double big=m.at(0).at(0);
    for(int i=0; i<m.size(); i++){
        for(int j=0; j<m.at(i).size(); j++){
            if(fabs(m.at(i).at(j))>fabs(big)) big=m.at(i).at(j);
        }
    }
    return big;
}

int broj_cifara(int n){
    int br{};
    if(n<0) br++;
    while(n!=0){
        n/=10;
        br++;
    }
    return br;
}

template <typename c1, typename c2, typename funkcija>
auto GeneraliziraniKroneckerovProizvod(c1 &a, c2 &b, funkcija fun){

    int redovi_a{};
    for(auto p=std::begin(a); p!=std::end(a); p++) redovi_a++;

    std::vector<int> brojevi_elemenata_redova_a;
    for(int i=0; i<redovi_a; i++) {
        int elementi_reda_a{};

        for(auto p=std::begin(a[i]); p!=std::end(a[i]); p++){
            elementi_reda_a++;
        }

        brojevi_elemenata_redova_a.push_back(elementi_reda_a);
    }
    bool ispravnost_redova_a=std::all_of(brojevi_elemenata_redova_a.begin(), brojevi_elemenata_redova_a.end(), [&](int n) {return n==brojevi_elemenata_redova_a.at(0);});

    int redovi_b{};
    for(auto p=std::begin(b); p!=std::end(b); p++) redovi_b++;

    std::vector<int> brojevi_elemenata_redova_b;
    for(int i=0; i<redovi_b; i++) {
        int elementi_reda_b{};

        for(auto p=std::begin(b[i]); p!=std::end(b[i]); p++){
            elementi_reda_b++;
        }

        brojevi_elemenata_redova_b.push_back(elementi_reda_b);
    }
    bool ispravnost_redova_b=std::all_of(brojevi_elemenata_redova_b.begin(), brojevi_elemenata_redova_b.end(), [&](int n) {return n==brojevi_elemenata_redova_b.at(0);});

    bool full_ispravnost_a=(redovi_a!=0 && ispravnost_redova_a);
    bool full_ispravnost_b=(redovi_b!=0 && ispravnost_redova_b);

    if(!full_ispravnost_a && !full_ispravnost_b)
        throw std::domain_error("Parametri nemaju formu matrice");
    else if(!full_ispravnost_b) throw std::domain_error("Drugi parametar nema formu matrice");
    else if(!full_ispravnost_a) throw std::domain_error ("Prvi parametar nema formu matrice");

    int kolone_a=brojevi_elemenata_redova_a.at(0);
    int kolone_b=brojevi_elemenata_redova_b.at(0);

    auto konj = Matrica<std::vector, std::vector<decltype((a[0][0])*(b[0][0]))>> (redovi_a*redovi_b, kolone_a*kolone_b);

    for(int i=0; i<redovi_a; i++){
        for(int j=0; j<redovi_b; j++){
            for(int k=0; k<kolone_a; k++){
                for(int l=0; l<kolone_b; l++){
                    try{
                        konj[i*redovi_b+j][k*kolone_b+l]=fun(a[i][k], b[j][l]);
                    }
                    catch(...){
                        throw std::runtime_error("Neocekivani problemi pri racunanju");
                    }
                }
            }
        }
    }

    return konj;

}

int main ()
{
    std::cout<<"Unesite dimenzije prve matrice: ";
    int m, n;
    std::cin>>m>>n;
    auto a=Matrica<std::vector, std::deque<double>> (m, n);
    std::cout<<"Unesite elemente prve matrice: "<<std::endl;
    for(int i=0; i<a.size(); i++){
        for (int j=0; j<a.at(0).size(); j++) {
            std::cin>>a.at(i).at(j);
        }
    }
    
    std::cout<<"Unesite dimenzije druge matrice: ";
    int p, q;
    std::cin>>p>>q;
    auto b=Matrica<std::deque, std::vector<double>> (p, q);
    std::cout<<"Unesite elemente druge matrice: "<<std::endl;
    for(int i=0; i<b.size(); i++){
        for (int j=0; j<b.at(0).size(); j++) {
            std::cin>>b.at(i).at(j);
        }
    }

    try {
        auto proizvod=GeneraliziraniKroneckerovProizvod(a, b, [](double x, double y) {return x*y;});
        int sirina=broj_cifara(najveci(proizvod));
        std::cout<<"Njihov Kroneckerov proizvod glasi:"<<std::endl;
        for(int i=0; i<proizvod.size(); i++){
            for(int j=0; j<proizvod.at(i).size(); j++){
                std::cout<<std::setw(sirina+1)<<proizvod.at(i).at(j)<<" ";
            }
            std::cout<<std::endl;
        }
    }
    catch(std::exception &e){
        std::cout<<e.what();
    }


	return 0;
}