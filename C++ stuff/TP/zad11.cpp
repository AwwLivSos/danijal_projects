#include <stdexcept>
#include <iostream>

class DoubleDeque {
private:
    double* elementi;      
    int kapacitet, brojElemenata;
    int prvi;
    int zadnji;

    void resize();                                           //preporučeno

public:
    DoubleDeque();                                              //defaultno ga inicijalizira da ima 10 elemenata
    DoubleDeque(std::initializer_list<double> lista);
    DoubleDeque(int velicina);
    DoubleDeque(const DoubleDeque& other);
    DoubleDeque(DoubleDeque&& other);
    ~DoubleDeque();
    DoubleDeque& operator=(const DoubleDeque& other);           //izborno, ali mora biti barem neki operator dodjele implementiran
    DoubleDeque& operator=(DoubleDeque&& other);

    void push_back(double vrijednost);
    void push_front(double vrijednost);
    void pop_back();
    void pop_front();

    double& front();
    double& back();
    double& element(int i);

    int size() const;
    bool empty() const;
};




DoubleDeque::DoubleDeque() : kapacitet(10), prvi(5), zadnji(5), brojElemenata(0) {
    elementi = new double[kapacitet];
}

DoubleDeque::~DoubleDeque() {
    delete[] elementi;
}

DoubleDeque::DoubleDeque(std::initializer_list<double> lista) : kapacitet(10), prvi(5), zadnji(5), brojElemenata(lista.size()) {
    elementi = new double[kapacitet];
    for (auto it = lista.begin(); it != lista.end(); ++it) {
        if (zadnji >= kapacitet) resize();
        elementi[zadnji++] = *it;
    }
}

DoubleDeque::DoubleDeque(int velicina) : kapacitet(velicina), prvi(velicina / 2), zadnji(velicina / 2), brojElemenata(0) {
    if (velicina <= 0) throw std::range_error("Neispravna velicina deka");
    elementi = new double[kapacitet];
}

DoubleDeque::DoubleDeque(const DoubleDeque& other) : kapacitet(other.kapacitet), prvi(other.prvi), zadnji(other.zadnji), brojElemenata(other.brojElemenata) {
    elementi = new double[kapacitet];
    std::memcpy(elementi, other.elementi, sizeof(double) * kapacitet);
}

DoubleDeque& DoubleDeque::operator=(const DoubleDeque& other) {
    if (this == &other) return *this;

    delete[] elementi;

    kapacitet = other.kapacitet;
    prvi = other.prvi;
    zadnji = other.zadnji;
    brojElemenata = other.brojElemenata;

    elementi = new double[kapacitet];
    std::memcpy(elementi, other.elementi, sizeof(double) * kapacitet);

    return *this;
}

DoubleDeque::DoubleDeque(DoubleDeque&& other)
    : elementi(other.elementi), kapacitet(other.kapacitet), prvi(other.prvi), zadnji(other.zadnji), brojElemenata(other.brojElemenata) {
    other.elementi = nullptr;
    other.kapacitet = 0;
    other.brojElemenata = 0;
}

DoubleDeque& DoubleDeque::operator=(DoubleDeque&& other) {
    if (this == &other) return *this;

    delete[] elementi;

    elementi = other.elementi;
    kapacitet = other.kapacitet;
    prvi = other.prvi;
    zadnji = other.zadnji;
    brojElemenata = other.brojElemenata;

    other.elementi = nullptr;
    other.kapacitet = 0;
    other.brojElemenata = 0;

    return *this;
}

void DoubleDeque::resize() {
    int newkapacitet = kapacitet * 2;
    double* novi = new double[newkapacitet];

    int novi_prvi = newkapacitet / 2 - brojElemenata / 2;

    for (int i = 0; i < brojElemenata; ++i)
        novi[novi_prvi + i] = elementi[prvi + i];

    delete[] elementi;
    elementi = novi;

    prvi = novi_prvi;
    zadnji = prvi + brojElemenata;
    kapacitet = newkapacitet;
}

void DoubleDeque::push_back(double vrijednost) {
    if (zadnji >= kapacitet) resize();
    elementi[zadnji++] = vrijednost;
    ++brojElemenata;
}

void DoubleDeque::push_front(double vrijednost) {
    if (prvi <= 0) resize();
    elementi[--prvi] = vrijednost;
    ++brojElemenata;
}

void DoubleDeque::pop_back() {
    if (empty()) throw std::out_of_range("Deque is empty");
    --zadnji;
    --brojElemenata;
}

void DoubleDeque::pop_front() {
    if (empty()) throw std::out_of_range("Deque is empty");
    ++prvi;
    --brojElemenata;
}

double& DoubleDeque::front() {
    if (empty()) throw std::out_of_range("Deque is empty");
    return elementi[prvi];
}

double& DoubleDeque::back() {
    if (empty()) throw std::out_of_range("Deque is empty");
    return elementi[zadnji - 1];
}

double& DoubleDeque::element(int i) {
    if (i < 0 || i >= brojElemenata) throw std::out_of_range("Index out of range");
    return elementi[prvi + i];
}

int DoubleDeque::size() const {
    return brojElemenata;
}

bool DoubleDeque::empty() const {
    return brojElemenata == 0;
}
