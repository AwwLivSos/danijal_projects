#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

void radixSort(vector<int> &a){
    if(a.empty()) return;                                   //logicno

    auto najveci = *max_element(a.begin(), a.end());        //za pronalazak najveceg elementa, tj onog sa najvise cifara, sa ovime ostaje linearna kompleksnost
    int br_cif = log10(najveci) + 1;                        //ez metod za nalazak broja cifara
    
    vector<queue<int>> redovi(10);
    int faktor=1;

    for(int i=0; i<br_cif; i++){                            //prva petlja je za sve cifre
        for(int broj : a){                                  //zatim trpamo brojeve na osnovu cifri
            int cifra = (broj/faktor)%10;
            redovi[cifra].push(broj);
        }

        int idx=0;
        for(int j=0; j<10; j++){
            while(!redovi[j].empty()){                      //ubacuj u og vektor sve elemente svih redova, po redu
                a[idx++]=redovi[j].front();
                redovi[j].pop();
            }
        }
        faktor*=10;
    }
}

int roditelj(int i) {return (i-1)/2;}
int lijevo_dijete(int i) {return 2*i+1;}
int desno_dijete(int i) {return 2*i+2;}
int jel_list(int i, const vector<int> &a) {return a.empty() || (i>=a.size()/2 && i<a.size());}

void popravi_dole(vector<int> &a, int i, int velicina) {
    while (i < velicina / 2) {
        int veci = lijevo_dijete(i);
        int dd = desno_dijete(i);

        if (dd < velicina && a[dd] > a[veci]) veci = dd;
        if (a[i] > a[veci]) break;
        swap(a[i], a[veci]);
        i = veci;
    }
}

void stvoriGomilu(vector<int> &a){
    int velicina = a.size();
    for(int i=velicina/2 - 1; i>=0; i--) popravi_dole(a, i, velicina);
}

void popravi_gore(vector<int> &a, int i){
    while(i!=0 && a[i]> a[roditelj(i)]){
        swap(a[i], a[roditelj(i)]);
        i=roditelj(i);
    }
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina) {
    a.push_back(umetnuti);
    velicina = a.size(); 
    popravi_gore(a, velicina - 1);
}  

int izbaciPrvi(vector<int> &a, int &velicina) {
    if (velicina == 0) throw domain_error("Gomila je prazna");
    velicina--;
    swap(a[0], a[velicina]);
    if (velicina != 0) popravi_dole(a, 0, velicina);
    return a[velicina];
}

void gomilaSort(vector<int> &a) {
    stvoriGomilu(a);
    int velicina = a.size();
    for (int i = velicina - 1; i >= 1; i--) {
        swap(a[0], a[i]);
        velicina--;
        popravi_dole(a, 0, velicina);
    }
}

int main(){
    vector<int> v{1,2,85,2898,98984,52,52,82,782,752,786};
    auto w=v;
    radixSort(v);
    for(auto x : v) cout<<x<<" ";
    cout<<endl;

    stvoriGomilu(w);
    for(auto x : w) cout<<x<<" ";
    cout<<endl;
    int vel = w.size();
    cout<<izbaciPrvi(w, vel)<<endl;
    umetniUGomilu(w, 10000, vel);
    for(auto x : w) cout<<x<<" ";
    cout<<endl;
    gomilaSort(w);
    for(auto x : w) cout<<x<<" ";
    cout<<endl;

    return 0;
}