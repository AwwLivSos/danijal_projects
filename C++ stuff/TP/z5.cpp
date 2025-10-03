    #include<iostream>
    #include<string>
    #include<fstream>
    #include<stdexcept>
    #include<iomanip>
    #include<vector>
    #include<sstream>

    template <typename TipEl>
      class Matrica {
        int br_redova, br_kolona;
        TipEl **elementi;
        char ime_matrice;
        static TipEl **AlocirajMemoriju(int br_redova, int br_kolona);
        static void DealocirajMemoriju(TipEl **elementi, int br_redova);
        void KopirajElemente(TipEl **elementi);
      public:
        Matrica(int br_redova, int br_kolona, char ime = 0);
        Matrica(const Matrica &m);
        Matrica(Matrica &&m);
        ~Matrica() { DealocirajMemoriju(elementi, br_redova); }
        Matrica &operator =(const Matrica &m);
        Matrica &operator =(Matrica &&m);
        template <typename Tip2>
          friend std::istream &operator >>(std::istream &tok, Matrica<Tip2> &m);
        template <typename Tip2>
          friend std::ostream &operator <<(std::ostream &tok, const Matrica<Tip2> &m);
        template <typename Tip2>
          friend Matrica<Tip2> operator +(const Matrica<Tip2> &m1, const Matrica<Tip2> &m2);
        template <typename Tip2>
          friend Matrica<Tip2> operator -(const Matrica<Tip2> &m1, const Matrica<Tip2> &m2);
        template <typename Tip2>
          friend Matrica<Tip2> operator *(const Matrica<Tip2> &m1, const Matrica<Tip2> &m2);
        template <typename Tip2>
          friend Matrica<Tip2> operator *(const Matrica<Tip2> &m, Tip2 d);
        template <typename Tip2>
          friend Matrica<Tip2> operator *(Tip2 d, const Matrica<Tip2> &m);
        Matrica &operator +=(const Matrica &m);
        Matrica &operator -=(const Matrica &m);
        Matrica &operator *=(const Matrica &m);
        Matrica &operator *=(TipEl d);
        TipEl *operator [](int i) { return elementi[i]; }
        const TipEl *operator [](int i) const { return elementi[i]; }
        TipEl &operator ()(int i, int j);
        const TipEl operator ()(int i, int j) const;
        operator std::string() const;
        void SacuvajUTekstualnuDatoteku(const std::string &ime) const;
        void SacuvajUBinarnuDatoteku(const std::string &ime) const;
        void ObnoviIzTekstualneDatoteke(const std::string &ime);
        void ObnoviIzBinarneDatoteke(const std::string &ime);
        Matrica(const std::string &ime, bool vrsta); //tekstualna false, binarna true
      };

      template <typename TipEl>
      TipEl ** Matrica<TipEl>::AlocirajMemoriju(int br_redova, int br_kolona) {
        TipEl **elementi = new TipEl*[br_redova]{};
        try {
          for(int i = 0; i < br_redova; i++) elementi[i] = new TipEl[br_kolona];
        }
        catch(...) {
          DealocirajMemoriju(elementi, br_redova);
          throw;
        }
        return elementi;
      }

    template <typename TipEl>
      void Matrica<TipEl>::DealocirajMemoriju(TipEl **elementi, int br_redova) {
        for(int i = 0; i < br_redova; i++) delete[] elementi[i];
        delete[] elementi;
      }


      template <typename TipEl>
      Matrica<TipEl>::Matrica(int br_redova, int br_kolona, char ime) :
        br_redova(br_redova), br_kolona(br_kolona), ime_matrice(ime),
        elementi(AlocirajMemoriju(br_redova, br_kolona)) {}

    template <typename TipEl>
      void Matrica<TipEl>::KopirajElemente(TipEl **elementi) {
        for(int i = 0; i < br_redova; i++)
          for(int j = 0; j < br_kolona; j++) Matrica::elementi[i][j] = elementi[i][j];
      }

    template <typename TipEl>
      Matrica<TipEl>::Matrica(const Matrica &m) : br_redova(m.br_redova),
        br_kolona(m.br_kolona), ime_matrice(m.ime_matrice),
        elementi(AlocirajMemoriju(m.br_redova, m.br_kolona)) {
          KopirajElemente(m.elementi);
      }

    template <typename TipEl>
      Matrica<TipEl>::Matrica(Matrica &&m) : br_redova(m.br_redova), br_kolona(m.br_kolona),
        elementi(m.elementi), ime_matrice(m.ime_matrice) {
          m.br_redova = 0; m.elementi = nullptr;
      }

    template <typename TipEl>
      Matrica<TipEl> &Matrica<TipEl>::operator =(const Matrica &m) {
        if(br_redova < m.br_redova || br_kolona < m.br_kolona) {
          TipEl **novi_prostor = AlocirajMemoriju(m.br_redova, m.br_kolona);
          DealocirajMemoriju(elementi, br_redova);
          elementi = novi_prostor;
        }
        else if(br_redova > m.br_redova)
          for(int i = m.br_redova; i < br_redova; i++) delete elementi[i];
        br_redova = m.br_redova; br_kolona = m.br_kolona;
        ime_matrice = m.ime_matrice;
        KopirajElemente(m.elementi);
        return *this;
      }

    template <typename TipEl>
      Matrica<TipEl> &Matrica<TipEl>::operator =(Matrica &&m) {
        std::swap(br_redova, m.br_redova); std::swap(br_kolona, m.br_kolona);
        std::swap(ime_matrice, m.ime_matrice); std::swap(elementi, m.elementi);
        return *this;
      }

    template <typename TipEl>
      std::istream &operator >>(std::istream &tok, Matrica<TipEl> &m) {
        for(int i = 0; i < m.br_redova; i++)
          for(int j = 0; j < m.br_kolona; j++) {
            if(&tok == &std::cin)
              std::cout << m.ime_matrice << "(" << i + 1 << "," << j + 1 << ") = ";
            tok >> m.elementi[i][j];
          }
        return tok;
      }

    template <typename TipEl>
    std::ostream &operator <<(std::ostream &tok, const Matrica<TipEl> &m) {
        int sirina_ispisa = 10;
        for(int i = 0; i < m.br_redova; i++) {
            for(int j = 0; j < m.br_kolona; j++)
                tok << std::setw(sirina_ispisa) << m.elementi[i][j];
            tok << std::endl;
        }
        return tok;
    }


      template <typename TipEl>
      Matrica<TipEl> operator +(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
        if(m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
          throw std::domain_error("Matrice nemaju jednake dimenzije!");
        Matrica<TipEl> m3(m1.br_redova, m1.br_kolona);
        for(int i = 0; i < m1.br_redova; i++)
          for(int j = 0; j < m1.br_kolona; j++)
            m3.elementi[i][j] = m1.elementi[i][j] + m2.elementi[i][j];
        return m3;
      }


    template <typename TipEl>
      Matrica<TipEl> operator -(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
        if(m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
          throw std::domain_error("Matrice nemaju jednake dimenzije!");
        Matrica<TipEl> m3(m1.br_redova, m1.br_kolona);
        for(int i = 0; i < m1.br_redova; i++)
          for(int j = 0; j < m1.br_kolona; j++)
            m3.elementi[i][j] = m1.elementi[i][j] - m2.elementi[i][j];
        return m3;
      }


    template <typename TipEl>
      Matrica<TipEl> operator *(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
        if(m1.br_kolona != m2.br_redova)
          throw std::domain_error("Matrice nisu saglasne za mnozenje");
        Matrica<TipEl> m3(m1.br_redova, m2.br_kolona);
        for(int i = 0; i < m1.br_redova; i++)
          for(int j = 0; j < m2.br_kolona; j++) {
            m3.elementi[i][j] = TipEl();
            for(int k = 0; k < m1.br_kolona; k++)
              m3.elementi[i][j] += m1.elementi[i][k] * m2.elementi[k][j];
          }
        return m3;
      }


    template <typename TipEl>
      Matrica<TipEl> operator *(const Matrica<TipEl> &m, TipEl d) {
        Matrica<TipEl> m2(m.br_redova, m.br_kolona);
        for(int i = 0; i < m.br_redova; i++)
          for(int j = 0; j < m.br_kolona; j++)
            m2.elementi[i][j] = m.elementi[i][j] * d;
        return m2;
      }

      template <typename TipEl>
      inline Matrica<TipEl> operator *(TipEl d, const Matrica<TipEl> &m) {
        return m * d;
      }


      template <typename TipEl>
      Matrica<TipEl> &Matrica<TipEl>::operator +=(const Matrica &m) {
        if(br_redova != m.br_redova || br_kolona != m.br_kolona)
          throw std::domain_error("Matrice nemaju jednake dimenzije!");
        for(int i = 0; i < br_redova; i++)
          for(int j = 0; j < br_kolona; j++)
            elementi[i][j] += m.elementi[i][j];
        return *this;
      }

    template <typename TipEl>
      Matrica<TipEl> &Matrica<TipEl>::operator -=(const Matrica &m) {
        if(br_redova != m.br_redova || br_kolona != m.br_kolona)
          throw std::domain_error("Matrice nemaju jednake dimenzije!");
        for(int i = 0; i < br_redova; i++)
          for(int j = 0; j < br_kolona; j++)
            elementi[i][j] -= m.elementi[i][j];
        return *this;
      }

    template <typename TipEl>
      Matrica<TipEl> &Matrica<TipEl>::operator *=(const Matrica &m) {
        if(br_kolona != m.br_redova)
            throw std::domain_error("Matrice nisu saglasne za mnozenje");
        Matrica<TipEl> m1 = *this;
        for(int i = 0; i < br_redova; i++)
          for(int j = 0; j < m.br_kolona; j++) {
            elementi[i][j] = TipEl();
            for(int k = 0; k < br_kolona; k++)
              elementi[i][j] += m1.elementi[i][k] * m.elementi[k][j];
          }
        return *this;
    }

    template <typename TipEl>
      Matrica<TipEl> &Matrica<TipEl>::operator *=(TipEl d) {
        for(int i = 0; i < br_redova; i++)
          for(int j = 0; j < br_kolona; j++) elementi[i][j] *= d;
        return *this;
      }



    template <typename TipEl>
      TipEl &Matrica<TipEl>::operator ()(int i, int j) {
        if(i < 1 || i > br_redova || j < 1 || j > br_kolona)
          throw std::range_error("Neispravan indeks");
        return elementi[i - 1][j - 1];
      }

    template <typename TipEl>
      const TipEl Matrica<TipEl>::operator ()(int i, int j) const {
        if(i < 1 || i > br_redova || j < 1 || j > br_kolona)
          throw std::range_error("Neispravan indeks");
        return elementi[i - 1][j - 1];
      }


    template <typename TipEl>
      Matrica<TipEl>::operator std::string() const {
        using std::to_string;
        std::string s = "{";
        for(int i = 0; i < br_redova; i++) {
          s += "{";
          for(int j = 0; j < br_kolona; j++) {
            s += to_string(elementi[i][j]);
            if(j != br_kolona - 1) s += ",";
          }
          s += "}";
          if(i != br_redova - 1) s += ",";
        }
        return s += "}";
      }


   template <typename TipEl>
void Matrica<TipEl>::SacuvajUTekstualnuDatoteku(const std::string &ime) const {
    std::ofstream izlaz(ime);
    if (!izlaz) throw std::logic_error("Problemi pri otvaranju datoteke");

    for (int i = 0; i < br_redova; i++) {
        for (int j = 0; j < br_kolona; j++) {
            izlaz << elementi[i][j];
            if (j < br_kolona - 1) izlaz << ",";
        }
        if (i < br_redova - 1) izlaz << std::endl;
    }

    if (!izlaz) throw std::logic_error("Problemi sa upisom u datoteku");
}

template <typename TipEl>
void Matrica<TipEl>::SacuvajUBinarnuDatoteku(const std::string &ime) const {
    std::ofstream izlaz(ime, std::ios::binary);
    if (!izlaz) throw std::logic_error("Problemi pri otvaranju datoteke");

    izlaz.write(reinterpret_cast<const char*>(&br_redova), sizeof(br_redova));
    izlaz.write(reinterpret_cast<const char*>(&br_kolona), sizeof(br_kolona));
    izlaz.write(reinterpret_cast<const char*>(&ime_matrice), sizeof(ime_matrice));

    for (int i = 0; i < br_redova; i++) {
        for (int j = 0; j < br_kolona; j++) {
            if (!izlaz.write(reinterpret_cast<const char*>(&elementi[i][j]), sizeof(TipEl))) {
                throw std::logic_error("Problemi sa upisom u datoteku");
            }
        }
    }
}

template <typename TipEl>
void Matrica<TipEl>::ObnoviIzTekstualneDatoteke(const std::string &ime) {
    std::ifstream ulaz(ime);
    if (!ulaz) throw std::logic_error("Trazena datoteka ne postoji");

    std::vector<std::vector<TipEl>> temp_elementi;
    std::string linija;

    while (std::getline(ulaz, linija)) {
        std::vector<TipEl> red;
        std::istringstream linija_stream(linija);
        std::string element_str;

        while (std::getline(linija_stream, element_str, ',')) {
            std::istringstream element_stream(element_str);
            TipEl element;
            if (!(element_stream >> element)) {
                throw std::logic_error("Datoteka sadrzi besmislene podatke");
            }
            red.push_back(element);
        }

        if (!red.empty()) {
            temp_elementi.push_back(red);
        }
    }

    if (temp_elementi.empty()) throw std::logic_error("Datoteka je prazna");

    int novi_redovi = temp_elementi.size();
    int novi_kolona = temp_elementi[0].size();

    for (const auto& red : temp_elementi) {
        if (red.size() != novi_kolona) {
            throw std::logic_error("Datoteka sadrzi besmislene podatke");
        }
    }

    TipEl** novi_elementi = AlocirajMemoriju(novi_redovi, novi_kolona);
    try {
        for (int i = 0; i < novi_redovi; i++) {
            for (int j = 0; j < novi_kolona; j++) {
                novi_elementi[i][j] = temp_elementi[i][j];
            }
        }
    } catch (...) {
        DealocirajMemoriju(novi_elementi, novi_redovi);
        throw;
    }

    DealocirajMemoriju(elementi, br_redova);
    elementi = novi_elementi;
    br_redova = novi_redovi;
    br_kolona = novi_kolona;
}

template <typename TipEl>
void Matrica<TipEl>::ObnoviIzBinarneDatoteke(const std::string &ime) {
    std::ifstream ulaz(ime, std::ios::binary);
    if (!ulaz) throw std::logic_error("Trazena datoteka ne postoji");

    int novi_redovi, nove_kolone;
    char novo_ime;

    if (!ulaz.read(reinterpret_cast<char*>(&novi_redovi), sizeof(novi_redovi)) ||
        !ulaz.read(reinterpret_cast<char*>(&nove_kolone), sizeof(nove_kolone)) ||
        !ulaz.read(reinterpret_cast<char*>(&novo_ime), sizeof(novo_ime))) {
        throw std::logic_error("Problemi pri citanju datoteke");
    }

    TipEl** novi_elementi = AlocirajMemoriju(novi_redovi, nove_kolone);
    try {
        for (int i = 0; i < novi_redovi; i++) {
            for (int j = 0; j < nove_kolone; j++) {
                if (!ulaz.read(reinterpret_cast<char*>(&novi_elementi[i][j]), sizeof(TipEl))) {
                    throw std::logic_error("Problemi pri citanju datoteke");
                }
            }
        }
    } catch (...) {
        DealocirajMemoriju(novi_elementi, novi_redovi);
        throw;
    }

    DealocirajMemoriju(elementi, br_redova);
    elementi = novi_elementi;
    br_redova = novi_redovi;
    br_kolona = nove_kolone;
    ime_matrice = novo_ime;
}

template <typename TipEl>
Matrica<TipEl>::Matrica(const std::string &ime, bool vrsta):br_redova(0), br_kolona(0), elementi(nullptr), ime_matrice(0) {
    if (vrsta) {
        ObnoviIzBinarneDatoteke(ime);
    } else {
        ObnoviIzTekstualneDatoteke(ime);
    }
}

void kreirajDatotetkuZbii1()
{
    std::string ime = "matrica.txt";
    std::ofstream dat(ime);
    dat << "1, 2\n3, 4";
    dat.close();
}

void kreirajDatotetkuZbii2()
{
    std::string ime = "matrica.txt";
    std::ofstream dat(ime);
    dat << "1.4, 2.54\n3.7, -4.7";
    dat.close();
}

int main() {
    try {
        Matrica<int> m1(2, 2, 'A');
        std::cout << "Unesite elemente matrice A:\n";
        std::cin >> m1;
        std::cout << "Matrica A:\n" << m1 << std::endl;

        m1.SacuvajUTekstualnuDatoteku("matrica.txt");
        m1.SacuvajUBinarnuDatoteku("matrica.bin");
        Matrica<int> m2(1, 1, 'B');
        Matrica<int> m3(1, 1, 'C');

        m2.ObnoviIzTekstualneDatoteke("matrica.txt");
        m3.ObnoviIzBinarneDatoteke("matrica.bin");

        std::cout<<"B: "<<std::endl; // iz tekstualne
        std::cout << m2;

        std::cout << "\nC:"<<std::endl; //iz binarne
        std::cout << m3;

        Matrica<int> m4("matrica.txt", false); // test konstruktora, tekstualna
        std::cout << "\nD:\n";
        std::cout << m4;

        Matrica<int> m5("matrica.bin", true); // konstr, binarna
        std::cout << "\nE:\n";
        std::cout << m5;
    }
    catch (const std::exception &e) {
        std::cout << "Greska: " << e.what() << std::endl;
    }

    return 0;
}

