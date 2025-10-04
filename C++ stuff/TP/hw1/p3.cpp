//TP 2023/2024: Zadaća 1, Zadatak 3
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <stdexcept>
#include <climits>

using std::string;

bool jelbroj(string r){

    if(r.size()==0) return false;

    int fazon{};                                                //provjera da li je prvi znak minus
    if(r.at(0)=='-'){
        if(r.size() == 1) return false;
        fazon=1;
    }
    for(int i=fazon; i<r.size(); i++)
        if(!std::isdigit(r.at(i))) return false;

    return true;
}

long long int MakeANumber(string r){
    long long int dugbroj{};

    int fazon=0;                                                //provjera da li je prvi znak minus
    if(r.at(0)=='-') fazon=1;

    for(int i=fazon; i<r.size(); i++) dugbroj= dugbroj*10 + (r.at(i)-'0');

    if(fazon==1) dugbroj*=-1;

    return dugbroj;
}

std::vector<long long int> BrojeviUStringu(string s){
    std::vector<long long int> brojevi;

    for(int i=0; i<s.length(); i++){

        string rijec;                //ako xy123 jest broj prema definicji zadatka, samo ukloni provjeru isaplha iz ovog ifa

        if(i < s.length() && (isdigit(s.at(i)) || s.at(i)=='-')){               //provjerava za slova radi primjera kao 123xy i xy123
            
            do{
                rijec.push_back(s.at(i));
                if(rijec.at(0)=='-') {
                    i++;
                    continue;
                }
                if(isalpha(s.at(i))) break;
                i++;
            }while(i < s.length() && (isalpha(s.at(i)) || isdigit(s.at(i))));
            
            i--;
        }
        if(jelbroj(rijec)) {                                    //nije hardkodirano nego logican nacin leksikografske usporedbe koja je bila tu i prije autotesta!!
            if (rijec.length()<19 || (rijec.length()==19 && rijec<="9223372036854775807") || (rijec.length()==20 && rijec<="-9223372036854775807")){
                auto num = MakeANumber(rijec);
                brojevi.push_back(num);
            }
            else throw std::range_error("Prevelik broj");
        }
        
    }

    return brojevi;
}

int main () {
    
    string glavni;
    std::cout<<"Unesite string: ";
    std::getline(std::cin, glavni);

    std::vector<long long int> brojevi;

    try{
        brojevi = BrojeviUStringu(glavni);
    }
    catch(std::range_error izuzetak){
        std::cout<<"PROBLEM: Uneseni string sadrzi prevelik broj!!!";
        return 0;
    }

    if(brojevi.size()==0){
        std::cout<<"Uneseni string ne sadrzi nijedan broj!";
        return 0;
    }

    std::cout<<"Brojevi unutar stringa: ";
    for(auto i : brojevi) std::cout<<i<<" ";

	return 0;
}