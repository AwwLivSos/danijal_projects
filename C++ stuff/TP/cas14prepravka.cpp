#include <iostream>
#include <stdexcept>
#include <cstring>

struct Grad {
    char *ime = nullptr;
    int broj_stanovnika = 0;
};

class Stanovnistvo {
    Grad* gradovi;
    int kapacitet;
    int brojGradova;

public:
    explicit Stanovnistvo(int max) : kapacitet(max), brojGradova(0) {
        gradovi = new Grad[kapacitet];
        for (int i = 0; i < kapacitet; i++)
            gradovi[i].ime = nullptr;
    }

    ~Stanovnistvo() {
        for (int i = 0; i < kapacitet; i++)
            delete[] gradovi[i].ime;
        delete[] gradovi;
    }

    // Copy constructor
    Stanovnistvo(const Stanovnistvo& other) 
        : kapacitet(other.kapacitet), brojGradova(other.brojGradova), gradovi(new Grad[other.kapacitet]) {
        for (int i = 0; i < brojGradova; i++) {
            if (!other.gradovi[i].ime)
                throw std::logic_error("Invalid copy: nullptr city name");
            size_t len = std::strlen(other.gradovi[i].ime);
            gradovi[i].ime = new char[len + 1];
            std::strcpy(gradovi[i].ime, other.gradovi[i].ime);
            gradovi[i].broj_stanovnika = other.gradovi[i].broj_stanovnika;
        }
        for (int i = brojGradova; i < kapacitet; i++)
            gradovi[i].ime = nullptr;
    }

    // Move constructor
    Stanovnistvo(Stanovnistvo&& other) noexcept 
        : gradovi(other.gradovi), kapacitet(other.kapacitet), brojGradova(other.brojGradova) {
        other.gradovi = nullptr;
        other.kapacitet = 0;
        other.brojGradova = 0;
    }

    // Copy-and-swap idiom for assignment
    Stanovnistvo& operator=(Stanovnistvo other) {
        std::swap(gradovi, other.gradovi);
        std::swap(kapacitet, other.kapacitet);
        std::swap(brojGradova, other.brojGradova);
        return *this;
    }

    void DodajGrad(const char *ime, int broj_stanovnika) {
        if (brojGradova >= kapacitet)
            throw std::range_error("Kolekcija je puna!");
        if (broj_stanovnika <= 0)
            throw std::domain_error("Neispravan broj stanovnika!");

        for (int i = 0; i < brojGradova; i++)
            if (std::strcmp(ime, gradovi[i].ime) == 0)
                throw std::domain_error("Grad već postoji!");

        size_t len = std::strlen(ime);
        gradovi[brojGradova].ime = new char[len + 1];
        std::strcpy(gradovi[brojGradova].ime, ime);
        gradovi[brojGradova].broj_stanovnika = broj_stanovnika;
        brojGradova++;
    }

    Grad operator[](const char *c) {
        for (int i = 0; i < brojGradova; i++)
            if (std::strcmp(c, gradovi[i].ime) == 0)
                return gradovi[i];
        throw std::domain_error("Nepoznat grad!");
    }

    const Grad operator[](const char *c) const {
        for (int i = 0; i < brojGradova; i++)
            if (std::strcmp(c, gradovi[i].ime) == 0)
                return gradovi[i];
        throw std::domain_error("Nepoznat grad!");
    }

    void ObrisiGrad(const char *c) {
        for (int i = 0; i < brojGradova; i++) {
            if (std::strcmp(c, gradovi[i].ime) == 0) {
                delete[] gradovi[i].ime;
                for (int j = i; j < brojGradova - 1; j++)
                    gradovi[j] = gradovi[j + 1];
                gradovi[--brojGradova].ime = nullptr;
                return;
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Stanovnistvo& s) {
        for (int i = 0; i < s.brojGradova; i++)
            os << s.gradovi[i].ime << ": " << s.gradovi[i].broj_stanovnika << '\n';
        return os;
    }
};

int main() {
    Stanovnistvo s(5);
    s.DodajGrad("Travnik", 25000);
    s.DodajGrad("Sarajevo", 500000);
    std::cout << s;

    Stanovnistvo kopija(s);
    std::cout << kopija<<std::endl<<s["Travnik"].ime<<" "<<kopija["Sarajevo"].broj_stanovnika;

    return 0;
}
