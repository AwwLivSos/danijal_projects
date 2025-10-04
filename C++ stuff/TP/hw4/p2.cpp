// TP 2023/2024: Zadaća 4, Zadatak 2
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

enum Pol { Musko, Zensko };

class GradjaninBiH {
  std::string ime;
  long long int maticni;
  GradjaninBiH *posljednji;
  static GradjaninBiH *zadnji_kreiran;

  bool provjeriJMBG(long long int mat) const;
  bool provjeriDatum(int d, int m, int g) const;
  long long int napraviJMBG(int d, int m, int g, int r, Pol pol);
  int zadnjiJMBG(std::string s) const;
  std::string jmbg_u_string() const { return std::to_string(maticni); }
  bool postojiJMBG(long long int jmbg) const;

public:
  GradjaninBiH(std::string name, long long int JMBG)
      : ime(name), maticni(JMBG) {
    if (name.empty())
      throw std::logic_error("Ime ne moze biti prazno!");
    if (postojiJMBG(JMBG))
      throw std::logic_error("Vec postoji gradjanin sa istim JMBG");
    if (!provjeriJMBG(JMBG))
      throw std::logic_error("JMBG nije validan");
    posljednji = zadnji_kreiran;
    zadnji_kreiran = this;
  }

  GradjaninBiH(std::string name, int dan, int mjesec, int godina, int region, Pol pol) : ime(name) {
    if (name.empty())
      throw std::logic_error("Ime ne moze biti prazno!");
    if (region < 0 || region > 99 || !provjeriDatum(dan, mjesec, godina))
      throw std::logic_error("Neispravni podaci");
    auto mat_br = napraviJMBG(dan, mjesec, godina, region, pol);
    maticni = mat_br;
    posljednji = zadnji_kreiran;
    zadnji_kreiran = this;
  }

  ~GradjaninBiH() {
    if (zadnji_kreiran == this)
      zadnji_kreiran = posljednji;
    else {
      GradjaninBiH *temp = zadnji_kreiran;
      while (temp!=nullptr && temp->posljednji != this)
        temp = temp->posljednji;
      if(temp!=nullptr)
        temp->posljednji = posljednji;
    }
  }

  GradjaninBiH(const GradjaninBiH &g) = delete;
  GradjaninBiH &operator=(const GradjaninBiH &g) = delete;

  std::string DajImeIPrezime() const { return ime; }
  long long int DajJMBG() const { return maticni; }
  int DajDanRodjenja() const;
  int DajMjesecRodjenja() const;
  int DajGodinuRodjenja() const;
  int DajSifruRegije() const;
  Pol DajPol() const;
  void PromijeniImeIPrezime(std::string s) {
    if (s.empty())
      throw std::logic_error("Ime ne moze biti prazno!");
    ime = s;
  }
  GradjaninBiH *DajPrethodnog() { return posljednji; }
  void PostaviPrethodnog(GradjaninBiH *g) { posljednji = g; }
};
GradjaninBiH *GradjaninBiH::zadnji_kreiran = nullptr; // ovo je potrebno uraditi van deklaracije klase TRUST

int GradjaninBiH::DajGodinuRodjenja() const {
  std::string pomocni = jmbg_u_string();
  if (pomocni.length() != 13)
    pomocni = "0" + pomocni;
  int god = std::stoi(pomocni.substr(4, 3));
  if (god < 100)
    god += 2000;
  else
    god += 1000;
  return god;
}

int GradjaninBiH::DajMjesecRodjenja() const {
  std::string pomocni = jmbg_u_string();
  if (pomocni.length() != 13)
    pomocni = "0" + pomocni;
  return std::stoi(pomocni.substr(2, 2));
}

int GradjaninBiH::DajDanRodjenja() const {
  std::string pomocni = jmbg_u_string();
  if (pomocni.length() != 13)
    pomocni = "0" + pomocni;
  return std::stoi(pomocni.substr(0, 2));
}

int GradjaninBiH::DajSifruRegije() const {
  std::string pomocni = jmbg_u_string();
  if (pomocni.length() != 13)
    pomocni = "0" + pomocni;
  return std::stoi(pomocni.substr(7, 2));
}

Pol GradjaninBiH::DajPol() const {
  std::string pomocni = jmbg_u_string();
  if (pomocni.length() != 13)
    pomocni = "0" + pomocni;
  if (std::stoi(pomocni.substr(9, 3)) < 500)
    return Musko;
  else
    return Zensko;
}

int GradjaninBiH::zadnjiJMBG(std::string s) const {
  int sum{};
  for (int i = 0; i < 6; i++) {
    sum += ((7 - i) * (int(s.at(i) - '0') + int(s.at(6 + i) - '0')));
  }
  int n = 11 - sum % 11;
  if (n == 11)
    n = 0;
  return n;
}

bool GradjaninBiH::provjeriJMBG(long long int mat) const {          // pegla
  auto jmbg = std::to_string(mat);
  if (jmbg.length() != 13)
    jmbg = "0" + jmbg;
  if (jmbg.length() != 13)
    return false;
  int dan = std::stoi(jmbg.substr(0, 2));
  int mjesec = std::stoi(jmbg.substr(2, 2));
  int godina = std::stoi(jmbg.substr(4, 3));
  if (godina < 100)
    godina += 2000;
  else
    godina += 1000;
  if (!provjeriDatum(dan, mjesec, godina))
    return false;
  if (int(jmbg.at(12) - '0') != zadnjiJMBG(jmbg))
    return false;
  return true;
}

bool GradjaninBiH::provjeriDatum(int d, int m, int g) const {
  int broj_dana[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (g % 4 == 0 && g % 100 != 0 || g % 400 == 0)
    broj_dana[1]++;
  return !(g < 1 || m < 1 || m > 12 || g < 1921 || g > 2021 || d < 1 ||
           d > broj_dana[m - 1]);
}

long long int GradjaninBiH::napraviJMBG(int d, int m, int g, int r, Pol pol) {      // pegla na kvadrat
  std::string dd = (d < 10) ? "0" + std::to_string(d) : std::to_string(d);
  std::string mm = (m < 10) ? "0" + std::to_string(m) : std::to_string(m);
  std::string ggg = std::to_string(g).substr(1, 3);
  std::string rr = (r < 10) ? "0" + std::to_string(r) : std::to_string(r);
  std::string kkk = (pol == Musko) ? "000" : "500";
  std::string dvanaest = dd + mm + ggg + rr + kkk;

  for (auto temp = zadnji_kreiran; temp != nullptr; temp = temp->posljednji) {
    auto polje = temp->jmbg_u_string();
    polje.pop_back();
    if (polje.length() != 13)
      polje = "0" + polje;
    if (polje == dvanaest) {
      dvanaest = std::to_string(std::stoll(dvanaest) + 1);
      temp=zadnji_kreiran;                                              //resetujes petlju tako da ponovno gleda sviju ispocetka
      if (d < 10)
        dvanaest = "0" + dvanaest;
    }
  }

  int zadnji = zadnjiJMBG(dvanaest);
  while (zadnji == 10){
      dvanaest = std::to_string(std::stoll(dvanaest) + 1);
      if (dvanaest.length() != 13)
      dvanaest = "0" + dvanaest;
      zadnji= zadnjiJMBG(dvanaest);
  }
  return std::stoll(dvanaest + std::to_string(zadnji));
}

bool GradjaninBiH::postojiJMBG(long long int jmbg) const {
  for (auto temp = zadnji_kreiran; temp != nullptr; temp = temp->posljednji)
    if (temp->maticni == jmbg)
      return true;
  return false;
}

int main() {
  int n;
  std::cout << "Unesite broj gradjana: ";
  std::cin >> n;
  GradjaninBiH **citizens = new GradjaninBiH *[n] {};
  for (int i = 0; i < n; i++) {
    std::cout << "Unesite podatke za " << i + 1
              << ". gradjanina: " << std::endl;
    std::string ime;
    std::cout << "Ime i prezime: ";
    std::cin.clear();
    std::cin.ignore(99999, '\n');
    std::getline(std::cin, ime);
    std::cout << "Datum rodjenja: ";
    int dan, mjesec, godina;
    std::cin >> dan >> mjesec >> godina;
    std::cout << "Sifra regije: ";
    int region;
    std::cin >> region;
    std::cout << "Pol ( 1-Musko, 2-Zensko ): ";
    int pol;
    Pol pol_enum;
    std::cin >> pol;
    pol_enum = (pol == 1) ? Musko : Zensko;
    try {
      citizens[i] =
          new GradjaninBiH(ime, dan, mjesec, godina, region, pol_enum);
    } catch (std::logic_error &e) {
      std::cout << e.what() << ". Molimo ponovite unos." << std::endl;
      i--;
    } catch (std::bad_alloc) {
      for (int i = 0; i < n; i++)
        delete citizens[i];
      delete[] citizens;
      return 0;
    }
  }

  std::sort(citizens, citizens + n,
            [](const GradjaninBiH *a, const GradjaninBiH *b) {
              if (a->DajGodinuRodjenja() != b->DajGodinuRodjenja())
                return a->DajGodinuRodjenja() < b->DajGodinuRodjenja();
              if (a->DajMjesecRodjenja() != b->DajMjesecRodjenja())
                return a->DajMjesecRodjenja() < b->DajMjesecRodjenja();
              return a->DajDanRodjenja() < b->DajDanRodjenja();
            });

  std::cout << "\nGradjani sortirani po starosti: " << std::endl;
  for (int i = 0; i < n; i++) {
    auto mat = std::to_string(citizens[i]->DajJMBG());
    if (mat.length() != 13)
      mat = "0" + mat;
    std::cout << citizens[i]->DajImeIPrezime() << " "
              << citizens[i]->DajDanRodjenja() << "."
              << citizens[i]->DajMjesecRodjenja() << "."
              << citizens[i]->DajGodinuRodjenja() << " JMBG: " << mat
              << std::endl;
  }

  for (int i = 0; i < n; i++)
    delete citizens[i];
  delete[] citizens;

  return 0;
}
