#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

class Person{
    std::string ime;
    int dob;
    static int brojosoba;
public:

    Person &Postavi(const std::string &s, int godina){
        ime=s;
        dob = godina;
        return *this;
    }

    void Ispisi(){
        std::cout<<ime<<" "<<dob;
    }

    ~Person(){

    }

};

int Person::brojosoba = 0;

class Temperature{
    std::vector<double> temps;
public:

    void Registruj(double temp){temps.push_back(temp);}
    void Obrisi(){temps.clear();}
    double DajNajvecu() const {return *std::max_element(temps.begin(), temps.end());}
    double DajNajmanju() const {return *std::min_element(temps.begin(), temps.end());}
    double DajProsjecnu() const {
        double suma{};
        for(auto x:temps) suma+=x;
        return suma/temps.size();
    }
    void Ispisi() const;
}; 

void Temperature::Ispisi() const{
    auto noviNiz = temps;

    std::sort(noviNiz.begin(), noviNiz.end());
    for(auto x : noviNiz) std::cout<<x<<std::endl;

}

int main(){

    Temperature t1;
    t1.Registruj(20.2);
    auto t2(t1);
    t2.Registruj(33.3);
    t2.Registruj(10);
    auto t3 = t2;
    t3.Registruj(8);
    t3.Registruj(23.23);

    std::cout<<t1.DajNajmanju()<<" "<<t2.DajNajvecu()<<" "<<t3.DajProsjecnu()<<std::endl;
    t1.Ispisi();
    t2.Ispisi();
    t3.Ispisi();

    return 0;
}