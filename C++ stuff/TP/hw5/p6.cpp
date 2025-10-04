//TP 2023/2024: Zadaća 5, Zadatak 6
#include <exception>
#include <iostream>
#include <cmath>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm> 

template <typename TipElemenata>
    void SortirajBinarnuDatoteku(const char ime_datoteke[], std::function<bool(TipElemenata, TipElemenata)> kriterij = std::less<TipElemenata>()){
        std::fstream file(ime_datoteke, std::ios::in | std::ios::out | std::ios::binary);
        if(!file) throw std::logic_error("Datoteka ne postoji");
        auto sajz = sizeof(TipElemenata);

        file.seekg(0, std::ios::end);
        long long int vel = file.tellg();
        int brojelementi = vel/sajz;
        
        //bablsort
        for(int i=0; i<brojelementi; i++){
            for(int j=0; j<brojelementi-1-i; j++){
                TipElemenata a, b;
                
                file.seekg(j*sajz);
                file.read(reinterpret_cast<char*>(&a), sajz);
                file.read(reinterpret_cast<char*>(&b), sajz);

                if(!kriterij(a, b)){
                    file.seekp(j*sajz);
                    file.write(reinterpret_cast<char*>(&b), sajz);
                    file.write(reinterpret_cast<char*>(&a), sajz);
                }
            }
        }
        file.close();
    }

void napisifile(const char imefilea[], std::vector<int> data){
    std::ofstream outputFile(imefilea, std::ios::binary);
    if(!outputFile) throw std::logic_error("Problemi sa otvaranjem datoteke!");
    outputFile.write(reinterpret_cast<char*>(&data[0]), data.size() * sizeof(int));
    outputFile.close();
}

void Ipisi(const char imefilea[], int vel){
    std::ifstream inputFile(imefilea, std::ios::binary);
    if (!inputFile) {
        throw std::runtime_error("Problemi sa otvaranjem datoteke!");
    }
    std::vector<int> pod (vel);
    while(inputFile){
        inputFile.read(reinterpret_cast<char*>(&pod[0]), pod.size()*vel);
        int broj = inputFile.gcount()/vel;
        for(int i=0; i<broj; i++){
            std::cout<<pod[i]<<" ";
        }
    }
    inputFile.close();
    std::cout << std::endl;
}

int main ()
{
    char ime[] = "podaci.bin";
    std::vector<int> data = {5, 2, 8, 1, 7, 36, 9, 4, 6, 90, 78, 78, 56, 54, 90, 23, 6798, 6789, 234, 7654, 908, 878, 676, 454, 343, 232, 212, 23, 56, 89, 21, 54, 87, 23, 20};
    napisifile(ime, data);
    try {
        SortirajBinarnuDatoteku<int>(ime);
    } catch (const std::exception& e) {
        std::cout << "Eror: " << e.what() << std::endl;
        return 0;
    }
    std::cout<<"Sortirano: "<<std::endl;
    Ipisi(ime, sizeof(int));

	return 0;
}