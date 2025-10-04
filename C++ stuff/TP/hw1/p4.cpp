//TP 2023/2024: Zadaća 1, Zadatak 4
#include <iostream>
#include <cmath>
#include <string>
#include <stdexcept>

using std::string;

bool testirajkljuc(string s){
    if (s.length()==0) return false;
    for(int i=0; i<s.length(); i++)
        if(s.at(i)<'A' || s.at(i)>'Z') return false;
    return true;
}

string napravikljuc(int max, string s){             //rastegne kljuc da je iste duzine kao i poruka
    string velikikljuc;
    for(int i=0; ; i++){
        if(i==s.length()) i=0;
        if(velikikljuc.length()==max) break;
        velikikljuc.push_back(s.at(i));
    }
    return velikikljuc;
}

string ViginereSifriranje(string poruka, string kljuc){
    if(!testirajkljuc(kljuc)) throw std::domain_error("Neispravan kljuc");
    string sifra;
    string keyword=napravikljuc(poruka.length(), kljuc);
    int j{};                                                            //dodano radi brojanja slova u kljucu bez razmaka i drugih karaktera
    
    for(int i=0; i<poruka.length(); i++){
        if(isalpha(poruka.at(i))){

            char x = (poruka.at(i) + (keyword.at(j)))%26;
            
            if(poruka.at(i) <='Z' && poruka.at(i)>='A') x+='A';
            else if (poruka.at(i) <='z' && poruka.at(i)>='a') {
                x+='a'-6;
                if(x<'a') x+=26;
            }

            sifra.push_back(x);
            j++;

        }
        else sifra.push_back(poruka.at(i));
    }

    return sifra;
}

string ViginereDesifriranje(string poruka, string kljuc){
    if(!testirajkljuc(kljuc)) throw std::domain_error("Neispravan kljuc");
    string original;
    string keyword = napravikljuc(poruka.length(), kljuc);
    int j=0;     

    for(int i=0; i<poruka.length(); i++){
        if(isalpha(poruka.at(i))){

            char x = (poruka.at(i) - keyword.at(j)+26)%26;
            
            if(poruka.at(i) <='Z' && poruka.at(i)>='A') x+='A';
            else if (poruka.at(i) <='z' && poruka.at(i)>='a') {
                x+='a'-6;                                               //6 karaktera između kraja velikih i pocetka malih slova!!
                if(x<'a') x+=26;
            }

            original.push_back(x);
            j++;

        }
        else original.push_back(poruka.at(i));
    }

    return original;
}

int main ()
{
    string key;
    while(1){
        std::cout<<"Unesite kljuc: ";
        std::getline(std::cin, key);
        if(!testirajkljuc(key))
            std::cout<<"Neispravan kljuc, ponovite unos: "<<std::endl;
        else break;
    }
    std::cout<<"Kljuc prihvacen. Unesite poruku: ";
    string poruka;
    std::getline(std::cin, poruka);
    string cipher=ViginereSifriranje(poruka, key);
    std::cout<<"Sifrirana poruka: "<<cipher<<std::endl;

    string novikey;
    std::cout<<std::endl;

    while(1){
        std::cout<<"Pokusajte pogoditi kljuc: ";
        std::getline(std::cin, novikey);
        if(!testirajkljuc(novikey)){
            std::cout<<"Niste pogodili kljuc, probajte ponovo!"<<std::endl;
            continue;
        }
        string decipher=ViginereDesifriranje(cipher, novikey);
        std::cout<<"Pokusaj desifriranja: "<<decipher<<std::endl;
        if(decipher!=poruka) std::cout<<"Niste pogodili kljuc, probajte ponovo!"<<std::endl;
        else break;
    }

    std::cout<<"Bravo, pogodili ste kljuc!";

	return 0;
}
