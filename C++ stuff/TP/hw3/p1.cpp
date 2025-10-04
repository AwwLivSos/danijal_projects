//TP 2023/2024: Zadaća 3, Zadatak 1
#include <exception>
#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <functional>
#include <stdexcept>
#include <algorithm>

using vektorparova = std::vector<std::pair<double, double>>;

std::function<double(double)> KubnaInterpolacija(vektorparova cvorovi){
    int duzina=cvorovi.size();
    if(duzina<4) throw std::domain_error("Nedovoljan broj cvorova");

    std::sort(cvorovi.begin(), cvorovi.end());
    for(int i=0; i<duzina-1; i++){
        if(cvorovi.at(i).first==cvorovi.at(i+1).first)
            throw std::domain_error("Neispravni cvorovi");
    
    }
    return [cvorovi, duzina](double x){
        if (x < cvorovi.front().first || x > cvorovi.back().first) throw std::range_error("Argument izvan opsega");

        int idx = 0;
        while (idx < duzina - 1 && x > cvorovi.at(idx + 1).first) idx++;

        if (x == cvorovi.at(idx).first) return cvorovi.at(idx).second;

        if(idx==0) idx=1;
        if(idx>=duzina-2) idx=duzina-3;

        double y{};
        for(int i=idx-1; i<=idx+2; i++){
            double proizvod{1};
            for(int j=idx-1; j<=idx+2; j++){
                if(j==i) continue;
                proizvod*=((x - cvorovi.at(j).first) / (cvorovi.at(i).first - cvorovi.at(j).first));
            }
            y+=(proizvod*cvorovi.at(i).second);
        }
        return y;
    };
}

std::function<double(double)> KubnaInterpolacija(std::function<double(double)> f, double xmin, double xmax, double delta){
    if(xmin>xmax || delta<=0) throw std::domain_error("Nekorektni parametri");
    vektorparova cvor;
    for(double i=xmin; i<=xmax; i+=delta) cvor.push_back(std::make_pair(i, f(i)));
    if(cvor.back().first != xmax) cvor.push_back(std::make_pair(xmax, f(xmax)));
    
    return KubnaInterpolacija(cvor);
}

int main ()
{
    std::cout<<"Odaberite opciju (1 - unos cvorova, 2 - aproksimacija): ";
    int opcija{};
    std::cin>>opcija;
    if(opcija==1){
        std::cout<<std::endl<<"Unesite broj cvorova: ";
        int n{};
        std::cin>>n;
        vektorparova nodes(n);
        std::cout<<std::endl<<"Unesite cvorove kao parove x y: ";
        for(int i=0; i<n; i++){
            std::cin>>nodes.at(i).first>>nodes.at(i).second;
        }

        for(;;){
            double y{};
            try{
                auto funkcija = KubnaInterpolacija(nodes);
                std::cout<<std::endl<<"Unesite argument (ili \"kraj\" za kraj): ";
                std::cin>>y;
                if(!std::cin) break;
                funkcija(y);
                std::cout<<std::endl<<"f("<<y<<") = "<<funkcija(y);
            }
            catch(std::range_error e){
                std::cout<<std::endl<<e.what()<<"!";
                std::cin.clear(); 
                std::cin.ignore(99999, '\n');
            }
            catch(std::domain_error e){
                std::cout<<std::endl<<e.what();
                return 0;
            }
        }
    }
    
    else{
        std::cout<<std::endl<<"Unesite krajeve intervala i korak: ";
        double min, max, korak;
        std::cin>>min>>max>>korak;
        for(;;){
            try{
                auto ef = [](double x){return x*x + std::sin(x) + std::log(x+1);};
                auto funkcija=KubnaInterpolacija(ef, min, max, korak);
                std::cout<<std::endl<<"Unesite argument (ili \"kraj\" za kraj): ";
                double arg{};
                std::cin>>arg;
                if(!std::cin) break;
                funkcija(arg);
                std::cout<<std::endl<<"f("<<arg<<") = "<<ef(arg)<<" fapprox("<<arg<<") = "<<funkcija(arg);
            }
            catch(std::range_error e){
                std::cout<<e.what()<<"!"<<std::endl;
                std::cin.clear(); 
                std::cin.ignore(99999, '\n');
            }
            catch(std::domain_error e){
                std::cout<<std::endl<<e.what();
                return 0;
            }
        }
    }
	return 0;
}