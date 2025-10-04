//TP 2023/2024: Zadaća 3, Zadatak 2
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <map>
#include <set>

typedef std::map<std::string, std::vector<std::string>> Knjiga;
typedef std::map<std::string, std::set<std::tuple<std::string, int, int>>> Indeksi;

Indeksi KreirajIndeksPojmova(const Knjiga &k){
    Indeksi ids;
    for(auto it=k.begin(); it!=k.end(); it++){

        auto poglavlje=it->first;
        for(int j=0; j<(it->second).size(); j++){

            auto str=(it->second).at(j);
            for(int k=0; k<str.length(); k++){
                auto p=k+1;
                if((std::isalpha(str.at(k)) || std::isdigit(str.at(k))) && (k==0 || (!std::isalpha(str.at(k-1)) && !std::isdigit(str.at(k-1))))){
                    std::string wrd;
                    while(k<str.length() && (std::isalpha(str.at(k)) || std::isdigit(str.at(k)))) 
                        wrd+=std::toupper(str.at(k++));
                    ids[wrd].insert({poglavlje, j+1, p});
                }
            }
        }
    }
    return ids;
}

bool jelRijec(std::string word){
    for(char c : word){
        if(!std::isalpha(c) && !std::isdigit(c)) return false;
    }
    return true;
}

std::set<std::tuple<std::string, int, int>> PretraziIndeksPojmova(std::string rijec, const Indeksi &indeks){
    if(!jelRijec(rijec)) throw std::domain_error("Neispravna rijec");

    std::string nova_rijec;
    for(auto c : rijec) nova_rijec+=toupper(c);

    std::set<std::tuple<std::string, int, int>> rezultat;
    for(auto i = indeks.begin(); i!=indeks.end(); i++){
        if(i->first == nova_rijec) {
            for(auto z : i->second) rezultat.insert(z);
        }
    }
    return rezultat;
}

void IspisiIndeksPojmova(const Indeksi &index){
    for(auto iter=index.begin(); iter!=index.end(); iter++){
        std::cout<<iter->first<<": ";
        for(auto debilski=iter->second.begin(); debilski !=iter->second.end(); debilski++){
            std::cout<<std::get<0>(*debilski)<<"/"<<std::get<1>(*debilski)<<"/"<<std::get<2>(*debilski);        //glup iterator Selme mi
            if(std::next(debilski) != iter->second.end()) std::cout<<", ";
        }
        std::cout<<std::endl;
    }
}

int main ()
{
    Knjiga hobit;
    std::string chapter, content;

    for(;;){
        std::cout<<"Unesite naziv poglavlja: ";
        std::getline(std::cin, chapter);
        if(chapter.empty()) break;

        for(int i=1; ; i++){
            std::cout<<"Unesite sadrzaj stranice "<<i<<": ";
            std::getline(std::cin, content);
            if(content.empty()) break;
            hobit[chapter].push_back(content);
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    Indeksi ix = KreirajIndeksPojmova(hobit);
    std::cout<<std::endl;

    std::cout<<std::endl<<"Kreirani indeks pojmova: "<<std::endl;
    IspisiIndeksPojmova(ix);

    std::cout<<std::endl;
    std::string s;
    for(;;){
        std::cout<<"Unesite rijec: ";
        std::getline(std::cin, s);
        if(s.empty()) break;
        try{
            auto rez=PretraziIndeksPojmova(s, ix);
            if(!rez.empty()){
                std::cout<<"Rijec nadjena na pozicijama: ";
                for(auto i : rez) std::cout<<std::get<0>(i)<<"/"<<std::get<1>(i)<<"/"<<std::get<2>(i)<<" ";
                std::cout<<std::endl;
            }
            else std::cout<<"Rijec nije nadjena!"<<std::endl;
        }
        catch(std::domain_error &e){
            std::cout<<e.what()<<"!"<<std::endl;
        }
    }
    std::cout<<"Dovidjenja!";

	return 0;
}
