#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <deque>

/*Napišite funkciju koja prima std::vector<int> i iz njega uklanja sve brojeve 
koji imaju bar jednu cifru jednaku parametru. Demonstrirajte rad funkcije unosom vektora 
i ispisom nakon filtracije.*/

//{1,2,3,4,5,6,7,595,84,58,16,51,1,5}

bool DaLiSadrzi(int broj, int cifra){

    broj = std::abs(broj);
    if(broj==0 && cifra==0) return true;

    while(broj!=0){
        if(broj%10 == cifra) return true;
        broj/=10;
    }
    return false;
}

void UkloniSaCifrom(std::vector<int> &v, int cifra){
    for(int i=0; i<v.size(); i++){
        if(DaLiSadrzi(v[i], cifra)){
            v.erase(v.begin() + i);
            i--;
        }
    }
}

std::vector<int> NapraviNovi(const std::vector<int> &v, int cifra){
    std::vector<int> novi;
    for(int i=0; i<v.size(); i++){
        if(!DaLiSadrzi(v[i], cifra)){
            novi.push_back(v[i]);
        }
    }
    return novi;
}

/*Napiši funkciju koja prima vektor stringova i vraća najdužu zajedničku 
prefiksnu podstringu (ako postoji). Ako nijedna ne postoji, vraća prazan string.

Primjer:
{"programiranje", "progresija", "proces"} → "pro"
{"dan", "noc", "sunce"} → ""*/

std::string NadjiPrefiks(const std::vector<std::string> &v){

    if(v.empty()) return "";

    std::string prefiks = v[0];
    for(int i=0; i<v.size(); i++){
        int j=0;
        while(j < v[i].length() && j<prefiks.length() && v[i][j] == prefiks[j])
            j++;
        prefiks = prefiks.substr(0, j);

    }
    return prefiks;
}

/*Napiši funkciju koja prima std::string, a vraća std::vector<int> 
s dužinama riječi (slova odvojena razmacima). Sve ne-slovo se ignoriše.*/

std::vector<int> DuzineRijeci(const std::string &s){
    std::vector<int> rez;

    int brojac=0;
    for(char c : s){
        if(std::isalpha(c)) brojac++;
        else if(!std::isalpha(c) && brojac>0){
            rez.push_back(brojac);
            brojac=0;
        }
    }

    if(brojac>0) rez.push_back(brojac);
    return rez;
}

/*aaa        bb      c  hh c*/

void OcistiRazmake(std::string &s){
    int j=0;
    bool oznaka = false;
    for(int i=0; i<s.length(); i++){
        if(s[i] == ' '){
            if(!oznaka){
                s[j++] = s[i];
                oznaka=true;
            }
        }
        else{
            s[j++] = s[i];
            oznaka=false;
        }
    }
    s.resize(j);
}

/*Generička funkcija koja prima dva iteratora i vraća maksimalni 
element. Ako je niz prazan, baci domain_error.*/

template<typename NekiTip>
auto NadjiMaks(NekiTip pocetak, NekiTip kraj){

    if(pocetak == kraj) throw std::domain_error("Prazna Kolekcija!");

    auto maksimalni = *pocetak;

    auto it = pocetak;
    while(it != kraj){
        if(*it > maksimalni) maksimalni = *it;
        it++;
    }

    return maksimalni;
}

/*Napišite generičku funkciju “PopuniPoKriteriju” koja prihvata pet parametara
p1, p2, p3, p4 i 𝑓. Parametri p1 i p2 omeđuju jedan blok podataka i mogu biti 
pokazivači ili iteratori, ali oba su istog tipa, dok parametar p3 (također pokazivač ili iterator) 
pokazuje na početak nekog odredišnog bloka.
Tip p3 ne mora biti isti kao tip p1 odnosno p2. Funkcija “PopuniPoKriteriju” treba da na svaki
element prvog bloka primijeni transformacionu funkciju 𝑓 (koja prima jedan argument) i da rezultate
transformacije upiše u drugi blok (pri čemu se pretpostavlja da u njemu ima dovoljno mjesta da
primi sve rezultate). U parametar p4 treba smjestiti pokazivač odnosno iterator koji pokazuje tačno
iza posljednjeg elementa odredišnog bloka. Sama funkcija treba da kao rezultat vrati sumu svih
elemenata u izvornom bloku. Za iteratore se podrazumijeva da podržavaju samo osnovni skup operacija
koje moraju podržavati svi iteratori. Napisanu funkciju demonstrirajte u kratkom isječku programa
gdje se sa tastature unosi neki vektor cijelih brojeva, a zatim se pozivom napisane funkcije popunjava
neki dek kvadratima elemenata tog vektora, ali u obrnutom poretku elemenata. Na primjer, za
elemente vektora 1, 2 i 3, kreirani dek treba redom da sadrži 9, 4 i 1 (kao rezultat funkcije vraća se 6).
Korištenje umetača je dozvoljeno i poželjno.*/

template<typename Tip1, typename Tip2, typename Funkcija>
auto PopuniPoKriteriju(Tip1 p1, Tip1 p2, Tip2 p3, Tip2 p4, Funkcija f){

    auto suma = *p1;
    auto it = p1;

    while(it != p2){
        *p3 = f(*it);

        it++;
        p3++;

        if(it!=p1) suma += *it;
    }
    p4 = p3;

    return suma;
}

int main(){

    std::vector<int> v{1,2,3};
    std::deque<int> d(3);

    auto it = d.rend();

    int suma = PopuniPoKriteriju(v.begin(), v.end(), d.rbegin(), it, [](int x){return x*x;});

    for(auto x : d) std::cout<<x<<" ";

    return 0;
}