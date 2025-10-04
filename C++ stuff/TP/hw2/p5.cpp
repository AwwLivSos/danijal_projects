//TP 2023/2024: Zadaća 2, Zadatak 5
#include <exception>
#include <iostream>
#include <cmath>
#include <iterator>
#include <stdexcept>
#include <vector>
#include <deque>

int faktorijelsabiranja(int a){
    int suma{};
    for(int i=0; i<=a; i++) suma+=i;
    return suma;
}

template <typename cont1, typename cont2>
auto KreirajTablicuMnozenja(cont1 a, cont2 b){

    int vel_a{};
    for(auto p=std::begin(a); p!=std::end(a); p++) vel_a++;
    int vel_b{};
    for(auto p=std::begin(b); p!=std::end(b); p++) vel_b++;

    if(vel_a!=vel_b) throw std::range_error("Kontejneri nisu iste duzine");

    using tip=decltype((*std::begin(a))*(*std::begin(b)));

    tip **matrix=nullptr;
    try{
        matrix=new tip*[vel_a];
        matrix[0]= new tip[faktorijelsabiranja(vel_a)];
        for(int i=1; i<vel_a; i++) matrix[i] = matrix[i-1]+i;
    }
    
    catch(...){
        delete[] matrix;
        matrix=nullptr;
        throw std::range_error("Nema dovoljno memorije");
    }

    auto p1=std::begin(a);
    for(int i=0; i<vel_a; i++){
        auto p2=std::begin(b);
        for(int j=0; j<=i; j++){

            if(((*p1)*(*p2))!=((*p2)*(*p1))) {
                delete[] matrix[0];
                delete[] matrix;
                matrix=nullptr;
                throw std::logic_error("Nije ispunjena pretpostavka o komutativnosti");
            }

            matrix[i][j]=(*p1)*(*p2);
            p2++;
        }
        p1++;
    }

    return matrix;

}

int main ()
{
    int n{};
    std::cout<<"Duzina sekvenci: ";
    std::cin>>n;
    std::vector<double> v(n);
    std::deque<double> d(n);
    std::cout<<"Elementi prve sekvence: ";
    for(auto &i : v) std::cin>>i;
    std::cout<<"Elementi druge sekvence: ";
    for(auto &i : d) std::cin>>i;

    double **matrica(nullptr);

    try{
        matrica=KreirajTablicuMnozenja(v, d);
        std::cout<<"Tablica mnozenja:\n";
        for(int i=0; i<n; i++){
            for(int j=0; j<=i; j++){
                std::cout<<matrica[i][j]<<" ";
            }
            std::cout<<std::endl;
        }
        delete[] matrica[0];
        delete[] matrica;
        matrica=nullptr;
    }
    catch(std::exception &e){
        std::cout<<e.what();
        
    }

	return 0;
}
