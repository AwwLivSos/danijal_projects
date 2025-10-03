#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cmath>

class Polinom{
    char var;
    std::vector<double> koeficijenti;

public:
    Polinom(char c, int vel) : var(c), koeficijenti(vel, 0){
        if(vel<=0) throw std::domain_error("Neispravna velicina polinoma!");
    }

    Polinom(char c, std::initializer_list<double> lista) : var(c){
        for(const auto &x : lista) koeficijenti.push_back(x);
    }

    double &operator[](int idx){
        if(idx<0 || idx>=koeficijenti.size()) throw std::range_error("Neispravan indeks!");
        return koeficijenti[idx];
    }

    const double &operator[](int idx) const {
        if(idx<0 || idx>=koeficijenti.size()) throw std::range_error("Neispravan indeks!");
        return koeficijenti[idx];
    }

    bool operator!() const {
        /* int broj_nula{};
        for(const auto &x : koeficijenti) if(x==0) broj_nula++;
        return broj_nula==koeficijenti.size(); */

        return std::all_of(koeficijenti.begin(), koeficijenti.end(), [](const double &x) {return x==0;}) == koeficijenti.size();
    }

    //5 2 8 9 1 6 0

    int operator*() const{
        int i;
        for(i=koeficijenti.size()-1; i>=0; i--) {
            if(koeficijenti[i]!=0) break;
            if(i==0) throw std::logic_error("Nul-polinom!");
        }
        return i;
    }

    double operator()(double x) const {
        double rez{};
        for(int i=koeficijenti.size()-1; i>=0; i--){
            rez += std::pow(x, i)*koeficijenti[i]; 
        }
        return rez;
    }

    Polinom operator-() const {
        auto novi = *this;
        for(auto &x : novi.koeficijenti) x=-x;
        return novi;
    }

    Polinom &operator+=(const Polinom &drugi){
        if(drugi.var != var) throw std::logic_error("Nesaglasne promjenjive!");
        koeficijenti.resize(std::max(drugi.koeficijenti.size(), koeficijenti.size()));

        for(int i=0; i<koeficijenti.size(); i++){
            if(i < drugi.koeficijenti.size()) koeficijenti[i] += drugi.koeficijenti[i];
        }

        return *this;
    }

    inline Polinom &operator-=(const Polinom &drugi){return (*this) += (-drugi);}

    Polinom &operator*=(const Polinom &drugi){
        if(drugi.var != var) throw std::logic_error("Nesaglasne promjenjive!");

        std::vector<double> novi(koeficijenti.size() + drugi.koeficijenti.size() - 1, 0);
        for(int i=0; i<koeficijenti.size(); i++){
            for(int j=0; j<drugi.koeficijenti.size(); j++){
                novi[i+j] += koeficijenti[i] * drugi.koeficijenti[j];
            }
        }
        koeficijenti = novi;
        return *this;
    }

    Polinom &operator+=(double skalar){
        koeficijenti[0] += skalar;
        return *this;
    }
    Polinom &operator-=(double skalar) {return (*this) += (-skalar);}

    Polinom &operator*=(double skalar){
        for(auto &x : koeficijenti) x*=skalar;
        return *this;
    }

    friend Polinom operator+(const Polinom &p1, const Polinom &p2) {auto p3=p1; return p3+=p2;}
    friend Polinom operator-(const Polinom &p1, const Polinom &p2) {auto p3=p1; return p3-=p2;}
    friend Polinom operator*(const Polinom &p1, const Polinom &p2) {auto p3=p1; return p3*=p2;}
    friend Polinom operator+(const Polinom &p1, double skalar) {auto p2=p1; return p2+=skalar;}
    friend Polinom operator+(double skalar, const Polinom &p1) {return p1+skalar;}
    friend Polinom operator-(const Polinom &p1, double skalar) {auto p2=p1; return p2-=skalar;}
    friend Polinom operator-(double skalar, const Polinom &p1) {return p1-skalar;}
    friend Polinom operator*(const Polinom &p1, double skalar) {auto p2=p1; return p2*=skalar;}
    friend Polinom operator*(double skalar, const Polinom &p1) {return p1*skalar;}

    friend bool operator ==(const Polinom &p1, const Polinom &p2){
        if(p1.var != p2.var) return false;
        if(p1.koeficijenti.size() != p2.koeficijenti.size()) return false;

        for(int i=0; i<p1.koeficijenti.size(); i++) if(p1[i] != p2[i]) return false;
        return true;
    }

    friend bool operator !=(const Polinom &p1, const Polinom &p2) {return !(p1==p2);}


    //2 3 -1 0 6
    friend std::ostream &operator <<(std::ostream &tok, const Polinom &p1){
        if(p1.koeficijenti.size() == 1 && p1[0] == 0) return tok<<"0";
        for(int i=0; i<p1.koeficijenti.size(); i++){
            double clan = p1.koeficijenti[i];

            if(clan==0) continue;

            if(clan>0 && i!=0) tok<<"+";
            else if(clan<0 && i!=0) tok<<"-";
            if(clan!=1 && clan!=-1) tok<<std::fabs(clan);

            if(i>0) tok<<p1.var;
            if(i>1) tok<<"^"<<i;
        }
        return tok;
    }

};

int main(){

    Polinom p1('x', {2, 3, -1, 0, 6});
    auto p2=p1*10;
    std::cout<<p2*p1;


    return 0;
}