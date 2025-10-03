#include <iostream>

class LinFun{
    double k, l;
public:
    LinFun(double ka=0, double el=0) : k(ka), l(el) {}
    LinFun(double el) : k(0), l(el) {}

    LinFun &Postavi(double ka, double el){
        k=ka;
        l=el;
        return *this;
    }

    double getK() const {return k;}
    double getL() const {return l;}

    LinFun operator -() const {return LinFun(-k, -l);}

    LinFun &operator+=(const LinFun &druga){
        k+=druga.k;
        l=druga.l;
        return *this;
    }

    LinFun &operator-=(const LinFun &druga){return (*this)+=(-druga);}

    friend LinFun operator+(const LinFun &l1, const LinFun &l2) {
        auto novi = l1;
        return novi+=l2; 
    }

    friend LinFun operator-(const LinFun &l1, const LinFun &l2) {return l1+(-l2);}

    LinFun &operator*=(double skalar){
        k*=skalar;
        l*=skalar;
        return *this;
    }

    LinFun &operator/=(double skalar) {return (*this) *= 1/skalar;}

    friend LinFun operator*(const LinFun &l, double skalar) {auto nova = l; return nova*=skalar;}
    friend LinFun operator*(double skalar, const LinFun &l) {auto nova = l; return nova*=skalar;}
    friend LinFun operator/(const LinFun &l, double skalar) {auto nova = l; return nova/=skalar;}

    LinFun &operator++() {
        l+=1;
        return *this;
    }

    LinFun operator++(int){
        auto novi = *this;
        ++(*this);
        return novi;
    }

    double operator()(double x) {return k*x+l;}

    LinFun inverzna() const {
        if(k==0) throw std::logic_error("Dijeljenje sa nulom!");

        return LinFun(1/k, -l/k);
    }
};

int main(){

    LinFun f(3,5);

    std::cout<<f(2);

    return 0;
}