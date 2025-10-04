// TP 2023/2024: Zadaća 3, Zadatak 4
#include <algorithm>
#include <iostream>
#include <list>
#include <stdexcept>
#include <vector>
#include <iterator>

std::vector<int> Razbrajanje(int n, int m) {
  if (n <= 0 || m <= 0)
    throw std::domain_error("Broj blokova i redni broj bloka su pozitivni cijeli brojevi i redni broj bloka ne moze biti veci od broja blokova");

  std::list<int> lista;
  for (int i = 1; i <= n; i++)
    lista.push_back(i);

  std::vector<int> splachina;

  auto it = lista.begin();
  splachina.push_back(*it); // prvi clan ce uvijek biti 1
  it = lista.erase(it);

  while (!lista.empty()) {
    for (int i = 0; i < m - 1; i++) {
      it++;
      if (it == lista.end())
        it = lista.begin();
    }
    splachina.push_back(*it);
    it = lista.erase(it);
    if (it == lista.end())
      it = lista.begin(); // kada se obrise trenutni element liste, iterator se pomjeri za 1, pa ako dođe do kraja, stavi ga na pocetak
  }

  return splachina;
}

int OdabirKoraka(int n, int k) {
    if(k>n || k<=0) throw std::domain_error("Broj blokova i redni broj bloka su pozitivni cijeli brojevi i redni broj bloka ne moze biti veci od broja blokova");
  if(n==1) return 1;
  int m{};
  std::vector<int> potencijali;
  for (int i = 1; i < n * n; i++)
    if (Razbrajanje(n, i).at(n - 1) == k)
      potencijali.push_back(i);
  if (!potencijali.empty())
    return *std::min_element(potencijali.begin(), potencijali.end());
  return m;
}

int main() {
  int n, k;
  std::cout << "Unesite broj distrikta u gradu: ";
  std::cin >> n;
  std::cout << "Unesite redni broj distrikta u kojem se nalazi restoran: ";
  std::cin >> k;
  try {
    auto rez = OdabirKoraka(n, k);
    if (rez)
      std::cout << "Trazeni korak: " << rez;
    else
      std::cout << "Ovo nazalost nije moguce.";
  } catch (std::domain_error e) {
    std::cout << e.what();
  }

  return 0;
}
