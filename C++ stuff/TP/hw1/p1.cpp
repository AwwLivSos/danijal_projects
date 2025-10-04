//TP 2023/2024: Zadaća 1, Zadatak 1
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <array>

enum class TretmanNegativnih {IgnorirajZnak, Odbaci, TretirajKao0, PrijaviGresku};

int BrojKvadrata(int n){
    if(n<0) throw std::domain_error("Broj mora biti nenegativan");
    if(n==0) return 0;
    if(int(std::sqrt(n))==std::sqrt(n)) return 1;   //odmah vrati ako je 0 ili savrseni kvadrat, tim redoslijedom

    for(int i=0; i*i<n; i++){                       //provjera da nije suma 2 kvadrata
        int ostatak=n-i*i;
        int korijen=std::sqrt(ostatak);
        if(korijen*korijen==ostatak) return 2;
    }

    for(int i=0; i*i*i<n; i++){                     //provjera da nije suma 3 kvadrata
        for(int j=i; j*j+i*i<n; j++){
            int ostatak=n-i*i-j*j;
            int korijen=std::sqrt(ostatak);
            if(korijen*korijen==ostatak) return 3;
        }
    }

    return 4;                                       //ako nije nista dosad, moze samo bit 4
}

std::array<std::vector<int>, 5> RazvrstajPoBrojuKvadrata(std::vector<int> &v, TretmanNegativnih t){
    std::array<std::vector<int>, 5> rez;

    for(int i=0; i<v.size(); i++){
        if(v.at(i)<0){
            switch (t) {
                case TretmanNegativnih::IgnorirajZnak :                                 //trebaju postati pozitibni, ili samo se praviti da minus nije tu?
                    rez.at(BrojKvadrata(abs(v.at(i)))).push_back(v.at(i));
                    continue;
                    break;
    
                case TretmanNegativnih::Odbaci : 
                    v.erase(v.begin()+i);
                    break;

                case TretmanNegativnih::TretirajKao0 :                                  //trebaju ici u kategoriju 0, a ne postati 0!
                    rez.at(0).push_back(v.at(i));
                    continue;
                    break;

                case TretmanNegativnih::PrijaviGresku :
                    throw std::domain_error("Nije predvidjeno razvrstavanje negativnih brojeva");
                    break;
            }
        }

        int sqrnum = BrojKvadrata(v.at(i));
        rez.at(sqrnum).push_back(v.at(i));
    }
    return rez;
}


int main ()
{
    std::cout<<"Unesite brojeve (bilo koji ne-broj oznacava kraj): ";
    std::vector<int> cijelibrojevi;
    int n;
    while(1){
        std::cin>>n;
        if(!std::cin) break;
        cijelibrojevi.push_back(n);
    }
    for(int i: cijelibrojevi){
        if(i<0){
            std::cout<<"Nije podrzano razvrstavanje negativnih brojeva!";
            return 0;
        }
    }

    std::cout<<std::endl<<std::endl<<"Rezultati razvrstavanja po broju kvadrata: "<<std::endl;
    std::array<std::vector<int>, 5> a=RazvrstajPoBrojuKvadrata(cijelibrojevi, TretmanNegativnih::IgnorirajZnak);

    for(int i=0; i<5; i++){
        if(a.at(i).size()==0) continue;
        std::cout<<i<<": ";
        for(int j: a.at(i)) std::cout<<j<<" ";
        std::cout<<std::endl;
    }


	return 0;
}
