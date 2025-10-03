#include <iostream>
#include <set>
#include <stack>

/*Koristeći gore navedene biliboteke (i nijednu više),
napisati C++ program gdje korisnik prvo unosi cijeli broj
n, koji predstavlja broj brojeva koji će se unijeti.
Te brojeve zatim treba ispisati u obrnutom poretku,
ali da se pritom ne ponavlja ni jedan broj.
Ispis svakog broja treba biti u novom redu*/












int main() {
    int n;
    std::cout << "Koliko zelite rijeci: ";
    std::cin >> n;

    std::set<std::string> skup_rijeci;
    std::cout << "Unesite rijeci:" << std::endl;

    for (int i = 0; i < n; i++) {
        std::string rijec;
        std::cin >> rijec;
        skup_rijeci.insert(rijec);
    }

    std::stack<std::string> stek;
    for (const std::string &rijec : skup_rijeci) {
        stek.push(rijec);
    }

    std::cout << "Rijeci u obrnutom redoslijedu:" << std::endl;
    while (!stek.empty()) {
        std::cout << stek.top() << std::endl;
        stek.pop();
    }

    return 0;
}
