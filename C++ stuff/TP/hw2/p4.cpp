//TP 2023/2024: Zadaća 2, Zadatak 4
#include <algorithm>
#include <iostream>
#include <vector>

using matrix=std::vector<std::vector<int>>;

template <typename tip>
bool Kriterij(const std::vector<tip> &a, const std::vector<tip> &b){
    auto max1=std::max_element(a.begin(), a.end());
    auto max2=std::max_element(b.begin(), b.end());
    if(*max1==*max2) return a>b; 
    return *max1>*max2;
}
template <typename tip1>
void SortirajPoDobrotiRedova(std::vector<std::vector<tip1>> &m){
    std::sort(m.begin(), m.end(), Kriterij<tip1>);
}


int main ()
{
    std::cout<<"Unesite elemente (* za kraj reda, * na pocetku reda za kraj unosa): "<<std::endl;
    matrix m;
    while(1){

        std::vector<int> red;
        
        while(1){
            int n;
            if (!(std::cin >> n)) {
                std::cin.clear();

                std::cin.ignore(99999, '\n');

                break;
            }
            red.push_back(n);
        }
        if(red.size()!=0) m.push_back(red);
        else break;

    }

    SortirajPoDobrotiRedova(m);

    std::cout<<"Matrica nakon sortiranja: "<<std::endl;

    for(int i=0; i<m.size(); i++){
        for(int j=0; j<m.at(i).size(); j++) std::cout<<m.at(i).at(j)<<" ";
        std::cout<<std::endl;
    }

    std::vector<int> sekvenca;
    std::cout<<"Unesite elemente sekvence koja se trazi (* za kraj reda): ";
    while(1){
        int a;
        std::cin>>a;
        if(!std::cin) break;
        sekvenca.push_back(a);
    }

    auto p=std::lower_bound(m.begin(), m.end(), sekvenca, Kriterij<int>);

    if(p!=m.end() && *p==sekvenca) std::cout<<"Trazena sekvenca se nalazi u "<<(p-m.begin()+1)<<". redu (nakon sortiranja)";
    else std::cout<<"Trazena sekvenca se ne nalazi u matrici";
	return 0;
}