//TP 2023/2024: Zadaća 2, Zadatak 1
#include <algorithm>
#include <cctype>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <string>

struct Tacka{       //rijesnje koje prati x i y koordinate u isto vrijeme
    int x;
    int y;
};

namespace Robot {
    enum class Pravci {Sjever, Sjeveroistok, Istok, Jugoistok, Jug, Jugozapad, Zapad, Sjeverozapad};
    enum class KodoviGresaka {PogresnaKomanda, NedostajeParametar, SuvisanParametar, NeispravanParametar};
    enum class Komande {Idi, Rotiraj, Sakrij, Otkrij, PrikaziTeren, Kraj};

    int lijevi_edge{-10}, desni_edge{10}, donji_edge{-10}, gornji_edge{10};
    int px{}, py{};
    Pravci direction=Pravci::Sjever;
    bool vidljivost=true;
    std::vector<Tacka> pozicije;
    bool crtanje=false;

    void KreirajTeren(int xmin, int xmax, int ymin, int ymax, int &x, int &o, Pravci &orijentacija);
    bool Idi(int &x, int &y, Pravci orijentacija, int korak);
    void Rotiraj(Pravci &orijentacija, int ugao);
    void Sakrij();
    void Otkrij();
    void IspisiPoziciju(int x, int y, Pravci orijentacija);
    void PrikaziTeren();
    void PrijaviGresku(KodoviGresaka kod_greske);
    bool IzvrsiKomandu(Komande komanda, int parametar, int &x, int &y, Pravci &orijentacija);
    bool UnosKomande(Komande &komanda, int &parametar, KodoviGresaka &kod_greske);
}

void Robot::Sakrij(){
    Robot::vidljivost=false;
}

void Robot::Otkrij(){
    Robot::vidljivost=true;
    Robot::pozicije.push_back({Robot::px, Robot::py});      //odmah zabiljezi gdje se nalazi
}

void Robot::KreirajTeren(int xmin, int xmax, int ymin, int ymax, int &x, int &o, Robot::Pravci &orijentacija){
    if(xmin>xmax || ymin>ymax) throw std::range_error("Nelegalan opseg");
    Robot::gornji_edge=ymax;
    Robot::donji_edge=ymin;
    Robot::desni_edge=xmax;
    Robot::lijevi_edge=xmin;

    x=(xmin+xmax)/2;
    o=(ymin+ymax)/2;

    orijentacija=Robot::Pravci::Sjever;
    Robot::vidljivost=true;
    Robot::pozicije.clear();                //ocisti sve i kreni ispocetka
}

void Robot::Rotiraj(Robot::Pravci &orijentacija, int ugao){
    int novi_pravac=(static_cast<int>(orijentacija)-ugao)%8;
    if(novi_pravac<0) novi_pravac+=8;
    orijentacija=static_cast<Robot::Pravci>(novi_pravac);
}

bool Robot::Idi(int &x, int &y, Robot::Pravci orijentacija, int korak){
    if(Robot::vidljivost) Robot::pozicije.push_back({x, y});
    bool rez=true;
    std::vector<int> pomak_x = {0, 1, 1, 1, 0, -1, -1, -1};
    std::vector<int> pomak_y = {1, 1, 0, -1, -1, -1, 0, 1}; //parovi smjerova, od sjevera do sjeverozapada

    int dx=pomak_x[static_cast<int>(orijentacija)];
    int dy=pomak_y[static_cast<int>(orijentacija)];

    if(korak<0) {
        korak*=-1;
        dx*=-1;
        dy*=-1;
    }

    for(int i=0; i<korak; i++){
        x+=dx;
        y+=dy;

        if(Robot::vidljivost) Robot::pozicije.push_back({x, y});

        if(x<Robot::lijevi_edge || x>Robot::desni_edge || y<Robot::donji_edge || y>Robot::gornji_edge) rez=false;

        if(!rez) {
            x-=dx;      //predobro 
            y-=dy;
            break;
        }
        Robot::px=x;
        Robot::py=y;
    }

    return rez;
}

void Robot::PrijaviGresku(Robot::KodoviGresaka kod_greske){
    std::vector<std::string> errorke = {                    //pokusaj elegantnijeg rjesavanja bez ijednog ifa
        "Nerazumljiva komanda!",
        "Komanda trazi parametar koji nije naveden!",
        "Zadan je suvisan parametar nakon komande!",
        "Parametar komande nije ispravan!"
    };
    std::cout<<errorke.at(static_cast<int>(kod_greske))<<std::endl;
}

std::string pravacustring(Robot::Pravci orijentacija){
    std::vector<std::string> pravci = {"sjever", "sjeveroistok", "istok", "jugoistok", "jug", "jugozapad", "zapad", "sjeverozapad"};        //slican fazon
    int index = static_cast<int>(orijentacija);
    return pravci.at(index);
}

void Robot::IspisiPoziciju(int x, int y, Robot::Pravci orijentacija){
    std::cout<<"Robot je ";
    if(Robot::vidljivost) std::cout<<"vidljiv, ";                   //moglo je i bez ifova kao gore al nema veze, samo su 2 slucaja ipak
    else std::cout<<"nevidljiv, ";
    std::cout<<"nalazi se na poziciji ("<<x<<","<<y<<") i gleda na "<<pravacustring(orijentacija)<<"."<<std::endl;;
}

void Robot::PrikaziTeren(){

    for(int i=Robot::gornji_edge+1; i>=Robot::donji_edge-1; i--){           //za 1 vise i manje od ivica, da bi # "okruzivale" teren
        for(int j=Robot::lijevi_edge-1; j<=Robot::desni_edge+1; j++){
            if(i==Robot::gornji_edge+1 || i==Robot::donji_edge-1 || j==Robot::lijevi_edge-1 || j==Robot::desni_edge+1) std::cout<<"#";
            else if(i==Robot::py && j==Robot::px) std::cout<<"O";           //ispis pozicije, trebao bi uvijek biti tacan
            else {
                bool nasao=false;
                for(auto p : Robot::pozicije){                              //ako je sve dosad bilo dobro, ovo bi bez izuzetka trebalo tacno ispisati pozicije
                    if(p.x==j && p.y==i){                                   //struktura ovdje spasava
                        nasao=true;
                        break;
                    }
                }
                if(nasao) std::cout<<"*";
                else std::cout<<" ";
            }
        }
        std::cout<<std::endl;
    }
    Robot::crtanje=true;
}

bool Robot::IzvrsiKomandu(Komande komanda, int parametar, int &x, int &y, Pravci &orijentacija){
    //pokusaj je bio za vektor pokazivaca na funkcije ali neuspjesan
    if(komanda==Robot::Komande::Idi) return Robot::Idi(x, y, orijentacija, parametar);
    else if(komanda==Robot::Komande::Rotiraj) Robot::Rotiraj(orijentacija, parametar);
    else if(komanda==Robot::Komande::Sakrij) Robot::Sakrij();
    else if(komanda==Robot::Komande::Otkrij) Robot::Otkrij();
    else if(komanda==Robot::Komande::PrikaziTeren) Robot::PrikaziTeren();

    return true;
}

bool Robot::UnosKomande(Robot::Komande &komanda, int &parametar, Robot::KodoviGresaka &kod_greske){
    char com;
    std::cin>>com;
    if(com=='I' || com=='R'){
        int parr;
        bool pozz{true};
        while(std::cin.peek()==' ') std::cin.get();                       //pojedi sve sto je praznina
        if(std::cin.peek()=='\n'){
            kod_greske = Robot::KodoviGresaka::NedostajeParametar;        //ako si dosao do kraja kada si pojeo sve praznine onda nije dobro
            std::cin.clear(); 
            std::cin.ignore(99999, '\n');
            return false;
        }
        if(!std::isdigit(std::cin.peek()) && std::cin.peek()!='-'){
            kod_greske = Robot::KodoviGresaka::NeispravanParametar;       //ako nakon jedenja nisi naisao na broj ili barem minus, nije dobro
            std::cin.clear(); 
            std::cin.ignore(99999, '\n');
            return false;
        }
        if(std::cin.peek()=='-') pozz=false, std::cin.get();

        std::cin>>parr;
        if(std::cin.peek()!=' ' && std::cin.peek()!='\n'){
            kod_greske = Robot::KodoviGresaka::NeispravanParametar;        
            std::cin.clear(); 
            std::cin.ignore(99999, '\n');
            return false;
        }
        while(std::cin.peek()==' ') std::cin.get();
        if(std::cin.peek()!='\n'){
            kod_greske = Robot::KodoviGresaka::SuvisanParametar;        //ako nisi dosao do kraja kada si pojedo sve praznine onda nije dobro
            std::cin.clear(); 
            std::cin.ignore(99999, '\n');
            return false;
        }
        if(!pozz) parr*=-1;
        parametar=parr;
        komanda = (com == 'I') ? Robot::Komande::Idi : Robot::Komande::Rotiraj;         //fancy
    }

    else if(com=='T' || com=='K'){
        while (std::cin.peek() == ' ') std::cin.get();
        if (std::cin.peek() != '\n') {
            kod_greske = Robot::KodoviGresaka::SuvisanParametar;        //slican fazon kao i gore samo mnogo jednostavnije
            std::cin.clear();
            std::cin.ignore(99999, '\n');
            return false;
        }
        komanda = (com == 'T') ? Robot::Komande::PrikaziTeren : Robot::Komande::Kraj;   
    }

    else if(com=='S'){
        char znak;
        while(std::cin.peek()==' ') std::cin.get();                       //pojedi sve sto je praznina
        if(std::cin.peek()=='\n'){
            kod_greske = Robot::KodoviGresaka::NedostajeParametar;        //ako si dosao do kraja kada si pojeo sve praznine onda nije dobro
            std::cin.clear(); 
            std::cin.ignore(99999, '\n');
            return false;
        }
        std::cin>>znak;
        if(znak!='+' && znak != '-'){
            kod_greske = Robot::KodoviGresaka::PogresnaKomanda;
            std::cin.clear();
            std::cin.ignore(99999, '\n');
            return false;
        }
        if(std::cin.peek()!='\n'){
            kod_greske = Robot::KodoviGresaka::SuvisanParametar;
            std::cin.clear();
            std::cin.ignore(99999, '\n');
            return false;
        }
        komanda = (znak=='+') ? Robot::Komande::Sakrij : Robot::Komande::Otkrij;
    }

    else {
        kod_greske=Robot::KodoviGresaka::PogresnaKomanda;
        std::cin.clear();
        std::cin.ignore(99999, '\n');
        return false;
    }
    return true;
}

int main ()
{
    std::cout<<"Unesite dimenzije terena (xmin xmax ymin ymax): ";
    int xmax{}, xmin{}, ymax{}, ymin{};
    std::cin>>xmin>>xmax>>ymin>>ymax;
    if(xmax<xmin || ymax<ymin){
        std::cout<<"Nije moguće kreiarati takav teren!";
        return 0;
    }
    Robot::KreirajTeren(xmin, xmax, ymin, ymax, Robot::px, Robot::py, Robot::direction);
    while(1){
        if(!Robot::crtanje) Robot::IspisiPoziciju(Robot::px, Robot::py, Robot::direction);
        Robot::crtanje=false;
        std::cout<<"Unesi komandu: ";
        Robot::Komande comand{};
        int parametar{};
        Robot::KodoviGresaka eror_kod{};

        if(!Robot::UnosKomande(comand, parametar, eror_kod)) {
            Robot::PrijaviGresku(eror_kod); 
            Robot::crtanje=true;
            continue;
        }
        if(comand==Robot::Komande::Kraj) break;
        if(!Robot::IzvrsiKomandu(comand, parametar, Robot::px, Robot::py, Robot::direction))
            std::cout<<"Robot je pokusao napustiti teren!"<<std::endl;
    }

    std::cout<<"Dovidjenja!";
	return 0;
}
