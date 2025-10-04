//TP 2023/2024: Zadaća 1, Zadatak 2
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <iomanip>

typedef std::vector<std::vector<double>> matrix;

matrix makematrix(int red, int kolona){
    return matrix(red, std::vector<double>(kolona));
}

void bablsort(std::vector<double> &v){              //sortiranje vektora po velicini, neefikasno al Boze moj ¯\_('/')_/¯
    int n=v.size();
    for(int i=0; i<n-1; i++){
        for(int j=0; j<n-i-1; j++){
            if(v.at(j)>v.at(j+1)){
                double temp=v.at(j);
                v.at(j)=v.at(j+1);
                v.at(j+1)=temp;
            }
        }
    }
}

double medijan(std::vector<double> &v){              //pronalazak medijana prozora elementa
    bablsort(v);

    int n=v.size();
    if(n%2!=0) return v.at(n/2);
    else return (v.at(n/2-1) + v.at(n/2))/2.0;
}

matrix MedijanskiFilter(matrix m, int n){
    if(n<0) throw std::domain_error("Neispravan red filtriranja");
    if(n==0) return m;

    matrix f;

    for(int i=0; i<m.size(); i++){
        f.push_back(std::vector<double>());

        for(int j=0; j<m.at(i).size(); j++){                                //3. i 4. petlja za prolazak kroz prozor zadate velicine
            std::vector<double> prozor;

            for(int k=i-n; k<=i+n; k++){
                for(int l=j-n; l<=j+n; l++){

                    if(k<0 || l<0 || k>=m.size() || l>=m.at(k).size()) continue;

                    prozor.push_back(m.at(k).at(l));

                }
            }

            f.at(i).push_back(medijan(prozor));

        }
    }

    return f;
}

int main ()
{
    std::cout<<"Unesite broj redova i kolona matrice: ";
    int row, col;
    std::cin>>row>>col;
    auto m = makematrix(row, col);
    std::cout<<"Unesite elemente matrice: ";
    for(int i=0; i<m.size(); i++){
        for (int j=0; j<m.at(0).size(); j++) {
            std::cin>>m.at(i).at(j);
        }
    }
    int filter;
    std::cout<<"Unesite red filtriranja: ";
    std::cin>>filter;

    matrix z;
    try{
        z=MedijanskiFilter(m, filter);
    }
    catch(std::domain_error e){
        std::cout<<"GRESKA: Neispravan red filtriranja!";
        return 0;
    }
    
    std::cout<<"Matrica nakon filtriranja: "<<std::endl;
    for(int i=0; i<z.size(); i++){
        for(int j=0; j<z.at(i).size(); j++){
            std::cout<<std::setw(7)<<std::fixed<<std::setprecision(2)<<z.at(i).at(j);
        }
        std::cout<<std::endl;
    }
	return 0;
}