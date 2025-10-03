//Pokušaj sopstvene implementacije binarnog Huffmanovog kodiranja
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>

//Za početak pravimo čvor koji sadrži simbol i njegovu frekvenciju, kao i pokazivače na "lijevi i desni" čvor
//Napravljeno je u fazonu specijalnog binarnog stabla, specijalno za Huffmana, te organizirano hijerarhijski
//Pokušaj vizuelnog dočaravanja ovakvog stabla:
/*
        (100)
       /     \
    (45)    (55)
           /    \
        (26)     (29)
       /   \     /   \
    (12)  (14) (13) (16)
         /   \
       (5)   (9)
*/
//Kad u algoritmu spajamo frekvencije ili vjerovatnoće, one idu za granu gore, dok ne dođu do ukupne frekvencije
//Na dnu su frekvencije pojavljivanja pojedinih simbola, pa kad ih spajamo one idu spojeno nivo iznad
//Lijeva grana (lijevi čvor) će se označavati sa 1, a desna grana (desni čvor) sa 0

struct Cvor {
    char simbol;
    int frekvencija;    //ili vjerovatnoća  
    Cvor* lijevi;
    Cvor* desni;
};

//Ovdje je custom "objekat" za usporedbu, koji nam treba za red sa prioritetom
struct Poredi{
    bool operator()(Cvor* c1, Cvor* c2){
        return c1->frekvencija > c2->frekvencija;       //Zapravo je ovo min-heap, navodno
    }
};//Ovaj dio mi je najmanje poznat bio

//Prva ozbiljnija funkcija za ovaj algoritam, korisitmo naše frekvencije koje su pojednostavljene u jedan pokazivač na čvor
//Pored toga imamo i string kodova, koji će se poslije puniti odgovarajućim bitovima
//Za kraj imamo ovu mapu koju punimo stringovima za odgovarajuće simbole 
void GenerisiKod(Cvor* korijen, std::string kod, std::map<char, std::string> &kodovi){
    if(!korijen) return;                                                    //Prekid funkcije kada nema više dalje da ide
    if(korijen->simbol != '\0') kodovi[korijen->simbol] = kod;              //Dodijeli kod za taj simbol
    GenerisiKod(korijen->lijevi, kod+"1", kodovi);                          //Kao što je gore rečeno, lijevo 1, desno 0
    GenerisiKod(korijen->desni, kod+"0", kodovi);
}
//Nakon ovoga ćemo imati kod za svaki skup simbola u svakoj iteraciji u Huffmanovom kodiranju, ne samo za pojedinačne simbole  

//Ovdje se sada ustvari vrši sva magija, pošaljemo mapu simbola i njihovih frekvencija
//i još šaljemo mapu u koju smještamo stablo simbola (i svih njihovih varijacija) i njihovih kodova
Cvor* HuffmanovKod(std::map<char, int> frekvencije, std::map<char, std::string> &kodovi){
    std::priority_queue<Cvor*, std::vector<Cvor*>, Poredi> red;                 //Najbitniji član je ovaj red sa prioritetom

    for(const auto &par: frekvencije) red.push(new Cvor{par.first, par.second, nullptr, nullptr});
    //Ovdje inicijaliziramo mapu (stablo) sa nepovezanim ćvorovima, tj "listovima"

    //Sada ovaj dio služi da bi spojili stvorene čvorove
    while(red.size() > 1){
        Cvor* left=red.top();
        red.pop();
        Cvor* rajt=red.top();
        red.pop();                  //U ovom dosad dijelu uzimam neka dva čvora, ali, jer znamo da su poredani, to su dva čvora sa najmanjom frekvencijom

        Cvor* spojeni = new Cvor {'\0', rajt->frekvencija+left->frekvencija, left, rajt};
        //Proces spajanja se debelo oslanja na red s prioritetom, jer nakon što uzmemo neka dva člana i strpamo im frekvencije u neki novi,
        //njegovo vraćanje nazad će ga opet staviti na pravo mjesto
        //Odluka za null terminator kao simbol je zato što nas zapravo ne zanima koji je simbol u čvorovima koji nisu "listovi"
        red.push(spojeni);
    }//Koristan uslov za petlju je da je veličina reda veća od 1, gdje će na kraju biti samo jedan čvor sa ukupnom fekvencijom

    Cvor* root = red.top();
    GenerisiKod(root, "", kodovi);  //Konačno, sve se na kraju lijepo zaokruži sa generisanjem kodova za stvoreno stablo
    return root;                    //Vracamo pokazivač koji je korijen cijelog stabla
}

//Dodaćemo još funkcije za kodiranje i dekodiranje "riječi" na osnovu Huffmanovog koda
std::string Encode(std::string poruka, const std::map<char, std::string> &kodovi){
    std::string kodirana;
    for(char c : poruka){
        auto it = kodovi.find(c);
        if(it!=kodovi.end()) kodirana += it->second;
        else throw std::domain_error("Greška, simbol ne postoji");      //Dodano za svaki slučaj
    }
    return kodirana;
}

std::string Decode(std::string kodirana, Cvor* korijen){        //Zbog ove funkcije u "HuffmanovKod" vrćamo pokazivač na čvor
    std::string dekodirana;
    Cvor* trenutni = korijen;           
    for(char bit : kodirana){
        if(bit == '1') trenutni = trenutni->lijevi;
        else trenutni = trenutni->desni;

        if(!trenutni->lijevi && !trenutni->desni){      //Došao do "lista"
            dekodirana+=trenutni->simbol;
            trenutni=korijen;               //Kad se riješi jedan znak, onda možemo ispočetka
        }
    } 
    return dekodirana;
}

//U mainu ćemo demonstrirati rad funkcije pomoću već navedenih frekvencija
int main(){
    std::map<char, int> frekvencije = {{'a', 5}, {'b', 9}, {'c', 12}, {'d', 13}, {'e', 16}, {'f', 45}};

    std::map<char, std::string> kodovi;
    Cvor* korijen = HuffmanovKod(frekvencije, kodovi);      //Pošto šaljemo mapu "kodovi" kao referencu, u nju će se smjestiti svi kodovi simbola

    std::cout<<"Huffanovo kodiranje glasi: \n";
    for(const auto &par : kodovi) std::cout<<par.first<<": "<< par.second<<std::endl;

    try{
        std::string poruka = "abcdef";
        std::cout<<"\nOriginalna poruka: " << poruka << std::endl;
        auto kodirana = Encode(poruka, kodovi);
        std::cout<<"Kodirana poruka: " << kodirana << std::endl;
        auto dekodirana = Decode(kodirana, korijen);
        std::cout<<"Dekodirana poruka: "<< dekodirana << std::endl;
    }catch(...){
        std::cout<<"Neki simbol nije definisan!\n";
    }
}           //Dobijeni rezultati se poklapaju sa ručnom provjerom, tako da je implementacija uspješna!