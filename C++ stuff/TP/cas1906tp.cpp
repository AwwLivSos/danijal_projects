/*Data je tekstualna datoteka “TEMPERATURE.TXT” (kreirana pomoću nekog tekstualnog editora, recimo
NotePad-a), koja sadrži rezultate mjerenja temperature u nekoj meteorološkoj stanici. Datoteka je
organizirana na sljedeći način. U prvom redu nalazi se datum kada je obavljeno mjerenje, u formatu
dan/mjesec/godina (npr. 5/12/2014). U drugom redu nalazi se komentar, koji može biti proizvoljan
tekst (to može biti npr. naziv mjernog mjesta, ili nešto drugo). U trećem redu nalaze se rezultati
mjerenja temperature za taj dan (mjerenja se vrše nekoliko puta dnevno), koji su međusobno
razdvojeni zarezima (iza posljednjeg rezultata nema zareza). Dalje se podaci ponavljaju za svaki od
dana za koji su registrirani rezultati mjerenja. Slijedi primjer mogućeg izgleda ove datoteke:

23/5/2016
Meteoroloska stanica Bjelave
7,12,16,20,18,13,6

9/11/2014
Mobilno mjerno mjesto Aerodrom, stanica II
−1,3,6,10,8

9/11/2014
Meteoroloska opservatorija Bjelasnica
−5,−2,0,3,1,2,−1,−4

Napisati program koji iščitava sadržaj ove tekstualne datoteke i kreira drugu tekstualnu datoteku
“IZVJESTAJ.TXT” koja sadrži izvještaj o mjerenjima koji izgleda poput sljedećeg, pri čemu se prosječna
temperatura ispisuje fiksno na dvije decimale:
Meteoroloska opservatorija Bjelasnica
-------------------------------------
Datum mjerenja: 9/11/2014
Minimalna temperatura: −5
Maksimalna temperatura: 3
Prosjecna temperatura: -0.75

Mobilno mjerno mjesto Aerodrom, stanica II
------------------------------------------
Datum mjerenja: 9/11/2014
Minimalna temperatura: -1
Maksimalna temperatura: 10
Prosjecna temperatura: 5.20

Meteoroloska stanica Bjelave
----------------------------
Datum mjerenja: 23/5/2016
Minimalna temperatura: 6
Maksimalna temperatura: 20
Prosjecna temperatura: 13.14

Spisak treba biti sortiran po datumu mjerenja, kao što je gore prikazano. U slučaju da su za dvije
grupe mjerenja datumi isti, podaci za grupu sa manjom prosječnom temperaturom trebaju doći
prije podataka za grupu sa većom prosječnom temperaturom. Pretpostaviti da ulazna datoteka
sadrži samo ispravne podatke, u ispravnom formatu (uključujući i ispravan format datuma). U
slučaju da čitanje datoteke protekne korektno, program ne ispisuje ništa na ekran. S druge strane,
ukoliko datoteka ne postoji, na ekran treba ispisati tekst “Datoteka TEMPERATURE.TXT ne postoji!”
(sa prelaskom u novi red nakon ispisa), a ukoliko prilikom njenog čitanja dođe do bilo kakvih drugih
problema osim čitanja iza kraja datoteke (poput fizičkog oštećenja, nailaska na nenumeričke
podatke prilikom čitanja numeričkih podataka, itd.) na ekran treba ispisati tekst “Problemi pri
citanju datoteke TEMPERATURE.TXT” (također uz prelazak u novi red).*/

#include <iostream>
#include <ios>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

/*Data je neka binarna datoteka koja sadrži snimljen sadržaj nekog niza realnih brojeva (tipa “double”).
Napisati funkciju “IzvrniDatoteku” koja kao parametar prima ime datoteke (tipa “string”), a koja
izvrće sadržaj datoteke kao “u ogledalu” tako da njen prvi element postane posljednji, drugi
pretposljednji, itd. Pri tome, sve se mora izvoditi direktno nad sadržajem datoteke, odnosno nije
dozvoljeno prethodno učitavanje sadržaja datoteke u memoriju. U slučaju da datoteka ne postoji,
treba baciti izuzetak tipa “logic_error” uz prateći tekst “Datoteka ne postoji”, a u slučaju bilo kakvih
problema prilikom čitanja ili upisa, treba baciti isti izuzetak, uz prateći tekst “Problemi prilikom
pristupa datoteci”. Obavezno napisati i testni program u kojem se demonstrira napisana funkcija.*/

void IzvrniDatoteku(const std::string &ime){
    std::fstream ulaz(ime, std::ios::in | std::ios::out | std::ios::binary);
    if(!ulaz) throw std::logic_error("Datoteka ne postoji!");

    ulaz.seekg(0, std::ios::end);
    int broj_elemenata = ulaz.tellg() / sizeof(double);
    for(int i=0, j=broj_elemenata; i<j; i++, j--){
        double pocetak, kraj;
        ulaz.seekg(i*sizeof(double)).read(reinterpret_cast<char*>(&pocetak), sizeof(double));
        ulaz.seekg(j*sizeof(double)).read(reinterpret_cast<char*>(&kraj), sizeof(double));
        ulaz.seekp(i*sizeof(double)).write(reinterpret_cast<char*>(&kraj), sizeof(double));
        ulaz.seekp(j*sizeof(double)).write(reinterpret_cast<char*>(&pocetak), sizeof(double));
        if(!ulaz) throw std::domain_error("Problemi prilikom pristupa datoteci!");
    }
}

struct Mjerenja{
    int dan, mjesec, godina;
    std::string komentar;
    int maxtemp, mintemp;
    double prosjecna;
};

int main(){
    std::ifstream ulaz("TEMPERATURE.TXT");
    if(!ulaz){
        std::cout<<"Datoteka TEMPERATURE.TXT ne postoji!\n";
        return 0;
    }

    char znak;
    Mjerenja m;
    std::vector<Mjerenja> v;

    while(ulaz>>m.dan>>znak>>m.mjesec>>znak>>m.godina){
        ulaz.ignore(99999, '\n');
        std::getline(ulaz, m.komentar);

        m.maxtemp=-99999;
        m.mintemp=999999;
        m.prosjecna=0;
        int trenutna, brojtemp{};

        for(;;){
            ulaz>>trenutna>>std::ws;
            m.prosjecna+=trenutna;
            brojtemp++;
            if(trenutna > m.maxtemp) m.maxtemp = trenutna;
            if(trenutna < m.mintemp) m.mintemp = trenutna;

            if(ulaz.peek() != ',') break;
            ulaz>>znak;
        }

        m.prosjecna/=brojtemp;
        v.push_back(m);
    }

    if(!ulaz.eof()){
        std::cout<<"Problemi pri citanju datoteke TEMPERATURE.TXT";
        return 0;
    }

    std::sort(v.begin(), v.end(), [](const Mjerenja &m1, const Mjerenja &m2){
        if(m1.godina != m2.godina) return m1.godina < m2.godina;
        if(m1.mjesec != m2.mjesec) return m1.mjesec < m2.mjesec;
        if(m1.dan != m2.dan) return m1.dan < m2.dan;
        return m1.prosjecna < m2.prosjecna;
    });

    std::ofstream izlaz("IZVJESTAJ.TXT");
    for(const Mjerenja &m : v){
        izlaz<<m.komentar<<std::endl
        <<std::string(m.komentar.length(), '-')
        <<"Datum mjerenja: "<< m.dan<<"/"<<m.mjesec<<"/"<<m.godina<<std::endl
        <<"Minimalna temperatura: "<<m.mintemp<<std::endl
        <<"Maksimalna temperatura: "<<m.maxtemp<<std::endl
        <<"Prosjecna temperatura: "<<std::setprecision(2)<<std::fixed<<m.prosjecna<<std::endl;
    }

    return 0;
}