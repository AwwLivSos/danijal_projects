//TP 2023/2024: Zadaća 4, Zadatak 1
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <algorithm>

typedef std::pair<double, double> Tacka;
enum Pozicija {GoreLijevo, GoreDesno, DoljeLijevo, DoljeDesno};
enum Smjer {Nalijevo, Nadesno};

class Pravougaonik{
    Tacka gore_desno, dole_lijevo;
public:
    Pravougaonik(const Tacka &t1, const Tacka &t2);
    void Postavi(const Tacka &t1, const Tacka &t2);
    void Postavi(Pozicija p, const Tacka &t);
    void Centriraj(const Tacka &t);
    Tacka DajTjeme(Pozicija p) const;
    Tacka DajCentar() const;
    double DajHorizontalnu() const;
    double DajVertikalnu() const;
    double DajObim() const;
    double DajPovrsinu() const;
    static Pravougaonik Presjek(const Pravougaonik &p1, const Pravougaonik &p2);
    void Transliraj(double delta_x, double delta_y);
    void Rotiraj(const Tacka &t, Smjer s);
    void Ispisi() const;
    friend bool DaLiSePoklapaju(const Pravougaonik &p1, const Pravougaonik &p2);
    friend bool DaLiSuPodudarni(const Pravougaonik &p1, const Pravougaonik &p2);
    friend bool DaLiSuSlicni(const Pravougaonik &p1, const Pravougaonik &p2);
};

Pravougaonik::Pravougaonik(const Tacka &t1, const Tacka &t2){
    Postavi(t1, t2);
}

void Pravougaonik::Postavi(const Tacka &t1, const Tacka &t2){
    gore_desno={std::max(t1.first, t2.first), std::max(t1.second, t2.second)};              //UZIMAS IM POSEBNO KOORDINATE DA NAPRAVIS SVOJ ĆOŠAK, NAJLAKSE GORE-DESNO I DOLE-LIJEVO JER SU OBA MAX I MIN
    dole_lijevo={std::min(t1.first, t2.first), std::min(t1.second, t2.second)};
}

void Pravougaonik::Postavi(Pozicija p, const Tacka &t){
    Tacka suprotna;
    switch(p){
        case GoreDesno:
            suprotna=dole_lijevo;
            break;
        case GoreLijevo:
            suprotna={std::max(gore_desno.first, dole_lijevo.first), std::min(gore_desno.second, dole_lijevo.second)};
            break;
        case DoljeDesno:
            suprotna={std::min(gore_desno.first, dole_lijevo.first), std::max(gore_desno.second, dole_lijevo.second)};
            break;
        case DoljeLijevo:
            suprotna=gore_desno;
            break;
    }
    Postavi(t, suprotna);
}

void Pravougaonik::Centriraj(const Tacka &t){
    Transliraj((t.first-DajCentar().first), (t.second-DajCentar().second));
}

Tacka Pravougaonik::DajTjeme(Pozicija p) const{
    switch(p){
        case GoreDesno:
            return gore_desno;
        case GoreLijevo:
            return {dole_lijevo.first, gore_desno.second};
        case DoljeDesno:
            return {gore_desno.first, dole_lijevo.second};
        case DoljeLijevo:
            return dole_lijevo;
    }
    return {0, 0};              //stavljeno da usutim kompajler, al ne bi se trebalo nikad desiti
}

Tacka Pravougaonik::DajCentar() const{
    return {(gore_desno.first + dole_lijevo.first)/2, (gore_desno.second + dole_lijevo.second)/2};
}

double Pravougaonik::DajHorizontalnu() const{
    return std::fabs(gore_desno.first - dole_lijevo.first);
}

double Pravougaonik::DajVertikalnu() const{
    return std::fabs(gore_desno.second - dole_lijevo.second);
}

double Pravougaonik::DajObim() const{
    return 2*DajHorizontalnu() + 2*DajVertikalnu();
}

double Pravougaonik::DajPovrsinu() const{
    return DajHorizontalnu()*DajVertikalnu();
}

Pravougaonik Pravougaonik::Presjek(const Pravougaonik &p1, const Pravougaonik &p2){         //OVO NIJE NI BLIZU NAJTEZE
    double x_min = std::max(p1.dole_lijevo.first, p2.dole_lijevo.first);
    double y_min = std::max(p1.dole_lijevo.second, p2.dole_lijevo.second);
    double x_max = std::min(p1.gore_desno.first, p2.gore_desno.first);
    double y_max = std::min(p1.gore_desno.second, p2.gore_desno.second);

    if(x_min<=x_max && y_min<=y_max) return {{x_min, y_min}, {x_max, y_max}};
    else throw std::domain_error("Pravougaonici se ne presjecaju");
}

void Pravougaonik::Transliraj(double delta_x, double delta_y){
    dole_lijevo.second+=delta_y;
    dole_lijevo.first+=delta_x;
    gore_desno.second+=delta_y;
    gore_desno.first+=delta_x;
}

void Pravougaonik::Rotiraj(const Tacka &t, Smjer s){            //OVA METODA JE DEBIL
    auto rotiraj_debilsku_tacku=[&](Tacka &p){
        double debilski_x, debilski_y;
        if(s==Nadesno){
            debilski_x = t.first + p.second - t.second;
            debilski_y = t.second - p.first + t.first;
        }else{
            debilski_x = t.first - p.second + t.second;
            debilski_y = t.second + p.first - t.first;
        }
        p={debilski_x, debilski_y};
    };
    rotiraj_debilsku_tacku(gore_desno);
    rotiraj_debilsku_tacku(dole_lijevo);

    Tacka jos_jedno_gd = {std::max(dole_lijevo.first, gore_desno.first), std::max(dole_lijevo.second, gore_desno.second)};
    Tacka jos_jedno_dl = {std::min(dole_lijevo.first, gore_desno.first), std::min(dole_lijevo.second, gore_desno.second)};
    
    gore_desno = jos_jedno_gd;
    dole_lijevo = jos_jedno_dl;
}

void Pravougaonik::Ispisi() const{
    Tacka gl = DajTjeme(GoreLijevo);
    Tacka dd = DajTjeme(DoljeDesno);                //zasto :(
    std::cout<<"{{"<<gl.first<<","<<gl.second<<"},{"<<dd.first<<","<<dd.second<<"}}";
}

bool DaLiSePoklapaju(const Pravougaonik &p1, const Pravougaonik &p2){
    return std::fabs(p1.gore_desno.first - p2.gore_desno.first) < 1e-10 && 
            std::fabs(p1.gore_desno.second - p2.gore_desno.second) < 1e-10 &&           //ko izmisli realne brojeve
            std::fabs(p1.dole_lijevo.first - p2.dole_lijevo.first) < 1e-10 && 
            std::fabs(p1.dole_lijevo.second - p2.dole_lijevo.second) < 1e-10;
}

bool DaLiSuPodudarni(const Pravougaonik &p1, const Pravougaonik &p2){
    if(DaLiSePoklapaju(p1, p2)) return true;
    double duza1 = std::max(p1.DajVertikalnu(), p1.DajHorizontalnu());
    double kraca1 = std::min(p1.DajVertikalnu(), p1.DajHorizontalnu());
    double duza2 = std::max(p2.DajVertikalnu(), p2.DajHorizontalnu());
    double kraca2 = std::min(p2.DajVertikalnu(), p2.DajHorizontalnu());
    return std::fabs(duza1-duza2)<1e-10 && std::fabs(kraca1-kraca2)<1e-10;
}

bool DaLiSuSlicni(const Pravougaonik &p1, const Pravougaonik &p2){
    if(DaLiSuPodudarni(p1, p2)) return true;
    double duza1 = std::max(p1.DajVertikalnu(), p1.DajHorizontalnu());
    double kraca1 = std::min(p1.DajVertikalnu(), p1.DajHorizontalnu());
    double duza2 = std::max(p2.DajVertikalnu(), p2.DajHorizontalnu());
    double kraca2 = std::min(p2.DajVertikalnu(), p2.DajHorizontalnu());
    if(p1.gore_desno == p1.dole_lijevo || p2.gore_desno == p2.dole_lijevo) return true; //ako je tacka
    if(((duza1 == 0 || kraca1 == 0) && (duza1 != 0 || kraca1 != 0)) && ((duza2 == 0 || kraca2 == 0) && (duza2 != 0 || kraca2 != 0))) return true; //ako su duzi
    double proporcija1 = duza1/kraca1;
    double proporcija2 = duza2/kraca2;
    return std::fabs(proporcija1-proporcija2) < 0.0000000001;
}

int main ()
{
    int n;
    std::cout<<"Unesite n: ";
    std::cin>>n;
    Pravougaonik **pnici = new Pravougaonik*[n]{};
    try{
        for(int i=0; i<n; i++){
            Tacka t1, t2;
            std::cout<<"Unesite 1. tjeme pravougaonika " << i+1 << ": ";
            std::cin>>t1.first>>t1.second;
            std::cout << "Unesite 2. tjeme pravougaonika " << i+1 << ": ";
            std::cin >> t2.first >> t2.second;
            pnici[i]=new Pravougaonik(t1, t2);
        }   
    }catch(std::exception &e){
        std::cout<<e.what();
        for(int i = 0; i < n; i++) delete pnici[i];
        delete[] pnici;
        return 0;
    }

    double dx, dy;
    std::cout << "Unesite podatke za transliranje (dx dy): ";
    std::cin >> dx >> dy;

    std::transform(pnici, pnici+n, pnici, [dx, dy](Pravougaonik *p){
        p->Transliraj(dx, dy);
        p->Rotiraj(p->DajCentar(), Nadesno);
        return p;
    });

    std::cout << "Pravougaonici, nakon transformacija:" << std::endl;

    std::for_each(pnici, pnici+n, [](Pravougaonik *p){
        p->Ispisi();
        std::cout<<std::endl;
    });

    auto max_p = *std::max_element(pnici, pnici+n, [](Pravougaonik *p1, Pravougaonik *p2){
        return p1->DajPovrsinu() < p2->DajPovrsinu();
    });

    std::cout<<"Pravougaonik s najvecom povrsinom: ";
    max_p->Ispisi();
    std::cout<<std::endl;

    for(int i=0; i<n; i++) delete pnici[i];
    delete[] pnici;

	return 0;
}
