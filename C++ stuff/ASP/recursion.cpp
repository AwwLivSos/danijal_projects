#include <iostream>

long long int fib2_0(int n, int zadnji = 1, int predzadnji = 0){
    if(n==0) return predzadnji;
    return fib2_0(n-1, zadnji+predzadnji, zadnji);                  
    //ovo ovdje je vrlo mocna tail-rekurzivna funkcija koja je jako kompaktno napisana, mada malo zbunjujuca
}

int nzd(int x, int y){              //vrlo jednostavna 
    if(y==0) return x;
    else return nzd(y, x%y);
}

using namespace std;

int main(){
    std::cout<<fib2_0(10)<<std::endl;   
    std::cout<<fib2_0(40)<<std::endl;
    std::cout<<nzd(14,7)<<std::endl;
    std::cout<<nzd(14,101)<<std::endl;
    std::cout<<nzd(650,101)<<std::endl;
    return 0;
}