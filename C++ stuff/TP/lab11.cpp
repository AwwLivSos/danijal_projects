#include <iostream>
#include <iomanip>
#include <cstring>
#include <stdexcept>
#include <functional>
#include <utility>
#include <algorithm>

template <typename TipIzuzetka>
    void TestirajIzuzetak(std::function<void()> akcija) {
        try { akcija(); }
        catch(TipIzuzetka &e) { std::cout << e.what() << std::endl; }
    }

class Tim {
    char ime_tima[21];
    int broj_odigranih, broj_pobjeda, broj_nerijesenih, broj_poraza, broj_datih,
    broj_primljenih, broj_poena;
public:
    Tim(const char ime[]);
    void ObradiUtakmicu(int broj_datih, int broj_primljenih);
    const char *DajImeTima() const { return ime_tima; }
    int DajBrojPoena() const { return broj_poena; }
    int DajGolRazliku() const { return broj_datih - broj_primljenih; }
    void IspisiPodatke() const;
};

Tim::Tim(const char ime[]) : broj_odigranih(0), broj_pobjeda(0), broj_nerijesenih(0), broj_poraza(0), broj_datih(0), broj_primljenih(0), broj_poena(0) {
    if(std::strlen(ime) > 20) throw std::range_error("Predugacko ime tima");
    std::strcpy(ime_tima, ime);
}

void Tim::ObradiUtakmicu(int broj_datih, int broj_primljenih) {
    if(broj_datih < 0 || broj_primljenih < 0) throw std::range_error("Neispravni rezultati utakmice");
    this->broj_odigranih++;
    this->broj_datih += broj_datih;
    this->broj_primljenih += broj_primljenih;
    
    if(broj_datih > broj_primljenih) {
        this->broj_pobjeda++;
        this->broj_poena += 3;
    } else if(broj_datih < broj_primljenih) {
        this->broj_poraza++;
    } else {
        this->broj_nerijesenih++;
        this->broj_poena++;
    }
}

void Tim::IspisiPodatke() const {
    std::cout << std::left << std::setw(20) << ime_tima << std::right
    << std::setw(4) << broj_odigranih << std::setw(4) << broj_pobjeda
    << std::setw(4) << broj_nerijesenih << std::setw(4) << broj_poraza
    << std::setw(4) << broj_datih << std::setw(4) << broj_primljenih
    << std::setw(4) << broj_poena << std::endl;
}

class Liga{
    int broj_timova;
    const int max_br_timova;
    Tim **timovi;

    void Dealociraj();
    int GdjejeTim(const char tim[]) const;

public:
    explicit Liga(int velicina_lige) : broj_timova(0), max_br_timova(velicina_lige), timovi(new Tim*[velicina_lige]{}) {}

    explicit Liga(std::initializer_list<Tim> lista_timova);

    ~Liga() { Dealociraj(); }

    Liga(const Liga &l);
    Liga(Liga &&l);

    Liga &operator =(Liga l);

    void DodajNoviTim(const char ime_tima[]);
    void RegistrirajUtakmicu(const char tim1[], const char tim2[],
    int rezultat_1, int rezultat_2);
    void IspisiTimove() const;
    void IspisiTabelu() const;
};

Liga::Liga(std::initializer_list<Tim> lista_timova) : broj_timova(0), max_br_timova(lista_timova.size()), timovi(new Tim*[lista_timova.size()]{}){
    try{
        int i=0;
        for(auto tim : lista_timova) timovi[i++] = new Tim(tim);
    }
    catch(...){
        Dealociraj();
        throw;
    }
}

Liga::Liga(const Liga &l) : broj_timova(l.broj_timova), max_br_timova(l.max_br_timova), timovi(new Tim*[l.max_br_timova]{}){
    try{
        for(int i=0; i<broj_timova; i++) timovi[i] = new Tim(*l.timovi[i]);
    }
    catch(...){
        Dealociraj(); throw;
    }
}

Liga::Liga(Liga &&l) : broj_timova(l.broj_timova), max_br_timova(l.max_br_timova), timovi(l.timovi){
    l.timovi=nullptr; 
    l.broj_timova = 0;
}

void Liga::Dealociraj(){
    for(int i=0; i<broj_timova; i++) delete timovi[i];
    delete[] timovi;
}

Liga &Liga::operator=(Liga l){
    if(max_br_timova != l.max_br_timova) throw std::logic_error("Nesaglasni kapaciteti liga");

    std::swap(l.broj_timova, broj_timova);
    std::swap(timovi, l.timovi);

    return *this;
}

void Liga::DodajNoviTim(const char ime_tima[]) {
 if(broj_timova >= max_br_timova) throw std::range_error("Liga popunjena");
 timovi[broj_timova++] = new Tim(ime_tima);
}

void Liga::IspisiTabelu() const {
 Tim **timovi_kopija = new Tim*[broj_timova];
 std::copy(timovi, timovi + broj_timova, timovi_kopija);

 std::sort(timovi_kopija, timovi_kopija + broj_timova, [](Tim *t1, Tim *t2) {
 if(t1->DajBrojPoena() != t2->DajBrojPoena())
 return t1->DajBrojPoena() > t2->DajBrojPoena();
 if(t1->DajGolRazliku() != t2->DajGolRazliku())
 return t1->DajGolRazliku() > t2->DajGolRazliku();
 return std::strcmp(t1->DajImeTima(), t2->DajImeTima()) < 0;
 });

 for(int i = 0; i < broj_timova; i++) timovi_kopija[i]->IspisiPodatke();
 delete[] timovi_kopija;
}

int main() {
    
    return 0;
}
