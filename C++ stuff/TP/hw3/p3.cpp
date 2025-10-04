//TP 2023/2024: Zadaća 3, Zadatak 3
#include <functional>
#include <iostream>
#include <cmath>
#include <map>
#include <stdexcept>
#include <new>
#include <iomanip>

enum class Smjer{Desno, Dolje, Dijagonalno};

template <typename TipElemenata>
    struct Matrica {
        char ime_matrice; // Koristi se samo u funkciji "UnesiMatricu"
        int br_redova, br_kolona;
        TipElemenata **elementi = nullptr; // VEOMA BITNA INICIJALIZACIJA!!!
    };

template <typename TipElemenata>
    void UnistiMatricu(Matrica<TipElemenata> &mat) {
        if(!mat.elementi) return;
        for(int i = 0; i < mat.br_redova; i++) delete[] mat.elementi[i];
        delete[] mat.elementi;
        mat.elementi = nullptr;
    }

template <typename TipElemenata>
    Matrica<TipElemenata> StvoriMatricu(int br_redova, int br_kolona, char ime = 0) {
        Matrica<TipElemenata> mat;
        mat.br_redova = br_redova; mat.br_kolona = br_kolona; mat.ime_matrice = ime;
        mat.elementi = new TipElemenata*[br_redova]{};
        try {
            for(int i = 0; i < br_redova; i++){
                mat.elementi[i] = new TipElemenata[br_kolona]{};
                for(int j=0; j<br_kolona; j++) mat.elementi[i][j]=0;
            }
        }
        catch(...) {
            UnistiMatricu(mat);
            throw;
        }
        return mat;
    }

template <typename TipElemenata>
    void UnesiMatricu(Matrica<TipElemenata> &mat) {
        for(int i = 0; i < mat.br_redova; i++)
            for(int j = 0; j < mat.br_kolona; j++) {
                std::cout << mat.ime_matrice << "(" << i + 1 << "," << j + 1 << ") = ";
                std::cin >> mat.elementi[i][j];
            }
    }

template <typename TipElemenata>
    void IspisiMatricu(Matrica<TipElemenata> mat, int sirina_ispisa, int sirina = 4, bool treba_brisati = false) {
        for(int i = 0; i < mat.br_redova; i++) {
            for(int j = 0; j < mat.br_kolona; j++)
                std::cout << std::fixed << std::setprecision(sirina-1) << std::setw(sirina_ispisa) << mat.elementi[i][j];
            std::cout << std::endl;
        }
        if(treba_brisati) UnistiMatricu(mat);
    }

template <typename TipElemenata>
    Matrica<TipElemenata> ZbirMatrica(const Matrica<TipElemenata> &m1, const Matrica<TipElemenata> &m2) {
        if(m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
            throw std::domain_error("Matrice nemaju jednake dimenzije!");
        auto m3 = StvoriMatricu<TipElemenata>(m1.br_redova, m1.br_kolona);
        for(int i = 0; i < m1.br_redova; i++)
            for(int j = 0; j < m1.br_kolona; j++)
                m3.elementi[i][j] = m1.elementi[i][j] + m2.elementi[i][j];
        return m3;
    }

Matrica<int> ProsiriPremaFunkcijama(const Matrica<int> &m, const std::map<Smjer, std::function<int(int)>> &mapa, int n){
    if(n<1) throw std::domain_error("Besmislen parametar");

    Matrica<int> finalna = StvoriMatricu<int>(m.br_redova, m.br_kolona);
    for(int i=0; i<m.br_redova; i++){
        for(int j=0; j<m.br_kolona; j++) finalna.elementi[i][j] = m.elementi[i][j];
    }
    if(n==1) return finalna;                                                            //prvo je napravis pa je onda vratis jer kad se brise, onda se moze izbrisat dvaput
    for(int inkrement=0; inkrement<n-1; inkrement++){
        int novi_redovi = finalna.br_redova, nove_kolone=finalna.br_kolona;
        bool desno_siri = mapa.find(Smjer::Desno) != mapa.end();
        bool dole_siri = mapa.find(Smjer::Dolje) != mapa.end();
        bool dijagonal_siri = mapa.find(Smjer::Dijagonalno) != mapa.end();              //provjere smjerova

        if(dijagonal_siri) nove_kolone*=2, novi_redovi*=2;
        else {
            if(desno_siri) nove_kolone*=2;
            if(dole_siri) novi_redovi*=2;
        }

        Matrica<int> pomocna=StvoriMatricu<int>(novi_redovi, nove_kolone);              //nova matrica sa novim dimenzijama

        for(int i=0; i<finalna.br_redova; i++){
            for(int j=0; j<finalna.br_kolona; j++){
                pomocna.elementi[i][j]=finalna.elementi[i][j];

                if(desno_siri) pomocna.elementi[i][j+finalna.br_kolona] = mapa.at(Smjer::Desno)(finalna.elementi[i][j]);
                if(dole_siri) pomocna.elementi[i+finalna.br_redova][j] = mapa.at(Smjer::Dolje)(finalna.elementi[i][j]);
                if(desno_siri && dole_siri){
                    if(dijagonal_siri) pomocna.elementi[i+finalna.br_redova][j+finalna.br_kolona] = mapa.at(Smjer::Dijagonalno)(finalna.elementi[i][j]);
                    else pomocna.elementi[i+finalna.br_redova][j+finalna.br_kolona] = finalna.elementi[i][j];
                }
            }
        }

        //ifologicno popunjavanje ¯\_(ツ)_/¯ 

        if(dijagonal_siri){
            if(!desno_siri && !dole_siri){
                for(int i=0; i<finalna.br_redova; i++){
                    for(int j=0; j<finalna.br_kolona; j++){
                        pomocna.elementi[i][j]=finalna.elementi[i][j];
                        pomocna.elementi[i+finalna.br_redova][j+finalna.br_kolona] = mapa.at(Smjer::Dijagonalno)(finalna.elementi[i][j]);
                        pomocna.elementi[i][j+finalna.br_kolona] = finalna.elementi[i][j];
                        pomocna.elementi[i+finalna.br_redova][j] = finalna.elementi[i][j];
                    }
                }
            } else if (!dole_siri){
                for(int i=0; i<finalna.br_redova; i++){
                    for(int j=0; j<finalna.br_kolona; j++){
                        pomocna.elementi[i][j]=finalna.elementi[i][j];
                        pomocna.elementi[i+finalna.br_redova][j+finalna.br_kolona] = mapa.at(Smjer::Dijagonalno)(finalna.elementi[i][j]);
                        pomocna.elementi[i][j+finalna.br_kolona] = mapa.at(Smjer::Desno)(finalna.elementi[i][j]);
                        pomocna.elementi[i+finalna.br_redova][j] = finalna.elementi[i][j];
                    }
                }
            } else if (!desno_siri){
                for(int i=0; i<finalna.br_redova; i++){
                    for(int j=0; j<finalna.br_kolona; j++){
                        pomocna.elementi[i][j]=finalna.elementi[i][j];
                        pomocna.elementi[i+finalna.br_redova][j+finalna.br_kolona] = mapa.at(Smjer::Dijagonalno)(finalna.elementi[i][j]);
                        pomocna.elementi[i][j+finalna.br_kolona] = finalna.elementi[i][j];
                        pomocna.elementi[i+finalna.br_redova][j] = mapa.at(Smjer::Dolje)(finalna.elementi[i][j]);
                    }
                }
            }
        }  
        UnistiMatricu(finalna);                 //prvo dealociraj tekucu pa se vrati na radnu, koju ces i vratiti
        finalna = pomocna;
    }
    return finalna;
}

int main ()
{
    Matrica<int> a; // AUTOMATSKA INICIJALIZACIJA!!!
    int m, n;
    std::cout << "Unesi broj redova i kolona matrice:\n";
    std::cin >> m >> n;
    try{
        a=StvoriMatricu<int>(m, n, 'M');
        UnesiMatricu(a);
        std::cout<<"Unesite n: ";
        int br;
        std::cin>>br;
        std::map<Smjer, std::function<int(int)>> funkcije{
            {Smjer::Desno, [] (int x) { return x + 1; }},
            {Smjer::Dolje, [] (int x) { return x + 2; }},
            {Smjer::Dijagonalno, [] (int x) { return x + 3; }}
        }; 
        auto b=ProsiriPremaFunkcijama(a, funkcije, br);
        IspisiMatricu(b, 4);
        UnistiMatricu(b);
    }
    catch(std::exception &e){
        std::cout<<e.what();
    }
    UnistiMatricu(a);
	return 0;
}
