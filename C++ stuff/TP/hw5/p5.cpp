//TP 2023/2024: Zadaća 5, Zadatak 5
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>

template <typename TipEl>
    class Matrica {
        int br_redova, br_kolona;
        TipEl **elementi;
        char ime_matrice;
        static TipEl **AlocirajMemoriju(int br_redova, int br_kolona);
        static void DealocirajMemoriju(TipEl **elementi, int br_redova);
        void KopirajElemente(TipEl **elementi);
        void CitajTekst(const std::string &ime_fajla);
        void CitajBinarno(const std::string &ime_fajla);                    //maksuz metode za konstruktor, za Obnovi metode zovemo njih + dealokaciju
    public:
        Matrica(int br_redova, int br_kolona, char ime = 0);
        Matrica(const Matrica &m);
        Matrica(Matrica &&m);
        ~Matrica() { DealocirajMemoriju(elementi, br_redova); }
        Matrica &operator =(const Matrica &m);
        Matrica &operator =(Matrica &&m);
        Matrica(const std::string &fajl, bool binarna);

        void SacuvajUTekstualnuDatoteku(const std::string &ime_fajla) const;
        void SacuvajUBinarnuDatoteku(const std::string &ime_fajla) const;
        void ObnoviIzTekstualneDatoteke(const std::string &ime_fajla);
        void ObnoviIzBinarneDatoteke(const std::string &ime_fajla);

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
    void Matrica<TipEl>::DealocirajMemoriju(TipEl** elementi, int br_redova){
        for(int i=0; i<br_redova; i++) delete [] elementi[i];
        delete [] elementi;
    }

template <typename TipEl>
    Matrica<TipEl>::Matrica(int br_redova, int br_kolona, char ime) : br_redova(br_redova), br_kolona(br_kolona), ime_matrice(ime), elementi(AlocirajMemoriju(br_redova, br_kolona)) {}

template <typename TipEl>
    Matrica<TipEl>::Matrica(const std::string &fajl, bool binarna) : br_kolona(0), br_redova(0), elementi(nullptr){
        if(binarna) CitajBinarno(fajl);                      
        else CitajTekst(fajl);
    }

template <typename TipEl>
    void Matrica<TipEl>::KopirajElemente(TipEl **elementi){
        for(int i = 0; i < br_redova; i++)
            for(int j = 0; j < br_kolona; j++) Matrica::elementi[i][j] = elementi[i][j];
    }

template <typename TipEl>
    Matrica<TipEl>::Matrica(const Matrica &m) : br_redova(m.br_redova), br_kolona(m.br_kolona), ime_matrice(m.ime_matrice), elementi(AlocirajMemoriju(m.br_redova, m.br_kolona)){
        KopirajElemente(m.elementi);
    }

template <typename TipEl>
    Matrica<TipEl>::Matrica(Matrica &&m) : br_redova(m.br_redova), br_kolona(m.br_kolona), ime_matrice(m.ime_matrice), elementi(m.elementi){
        m.br_redova=0;
        m.elementi=nullptr;
    }

template <typename TipEl>
    Matrica<TipEl> &Matrica<TipEl>::operator =(const Matrica &m){
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
    Matrica<TipEl> &Matrica<TipEl>::operator =(Matrica &&m){
        if(this!=&m){
            std::swap(ime_matrice, m.ime_matrice);
            std::swap(br_redova, m.br_redova);
            std::swap(br_kolona, m.br_kolona);
            std::swap(elementi, m.elementi);
        }
        return *this;
    }

template <typename TipEl>
    std::istream &operator >>(std::istream &tok, Matrica<TipEl> &m){
        for(int i=0; i<m.br_redova; i++){
            for(int j=0; j<m.br_kolona; j++){
                if(&tok == &std::cin) std::cout<<"("<<i+1<<","<<j+1<<") = ";
                tok>>m.elementi[i][j];
            }
        }
        return tok;
    }

template <typename TipEl>
    std::ostream &operator <<(std::ostream &tok, const Matrica<TipEl> &m){
        int sirina = tok.width();
        for(int i=0; i<m.br_redova; i++){
            for(int j=0; j<m.br_kolona; j++){
                std::cout<<std::setw(sirina)<<m.elementi[i][j];
            }
            std::cout<<std::endl;
        }
        return tok;
    }    

template <typename TipEl>
    Matrica<TipEl> operator +(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
        if(m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona) throw std::domain_error("Matrice nemaju jednake dimenzije!");
        Matrica<TipEl> m3(m1.br_redova, m1.br_kolona);
        for(int i = 0; i < m1.br_redova; i++)
            for(int j = 0; j < m1.br_kolona; j++)
                m3.elementi[i][j] = m1.elementi[i][j] + m2.elementi[i][j];
        return m3;
    }

template <typename TipEl>
    Matrica<TipEl> operator -(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
        if(m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona) throw std::domain_error("Matrice nemaju jednake dimenzije!");
        Matrica<TipEl> m3(m1.br_redova, m1.br_kolona);
        for(int i = 0; i < m1.br_redova; i++)
            for(int j = 0; j < m1.br_kolona; j++)
                m3.elementi[i][j] = m1.elementi[i][j] - m2.elementi[i][j];
        return m3;
    }

template <typename TipEl>
    Matrica<TipEl> operator *(const Matrica<TipEl> &m1, const Matrica<TipEl> &m2) {
        if(m1.br_kolona != m2.br_redova) throw std::domain_error("Matrice nisu saglasne za mnozenje");
        Matrica<TipEl> m3(m1.br_redova, m1.br_kolona);
        for(int i = 0; i < m2.br_kolona; i++){
            for(int j = 0; j < m1.br_kolona; j++){
                m3.elementi[i][j]=TipEl();
                for(int k=0; k<m1.br_kolona; k++) m3.elementi[i][j]+=m1.elementi[i][k]*m2.elementi[k][j];
            }
        }
        return m3;
    }

template <typename TipEl>
    Matrica<TipEl> operator *(const Matrica<TipEl> &m1, TipEl d) {
        Matrica<TipEl> m2(m1.br_redova, m1.br_kolona);
        for(int i = 0; i < m1.br_redova; i++)
            for(int j = 0; j < m1.br_kolona; j++)
                m2.elementi[i][j] = m1.elementi[i][j] *d;
        return m2;
    }

template <typename TipEl>
    Matrica<TipEl> operator *( TipEl d, const Matrica<TipEl> &m1) {
        return m1*d;
    }

template <typename TipEl>
    Matrica<TipEl> &Matrica<TipEl>::operator +=(const Matrica &m) {
        if(br_redova != m.br_redova || br_kolona != m.br_kolona) throw std::domain_error("Matrice nemaju jednake dimenzije!");
        for(int i = 0; i < br_redova; i++)
            for(int j = 0; j < br_kolona; j++)
                elementi[i][j] += m.elementi[i][j];
        return *this;
    }

template <typename TipEl>
    Matrica<TipEl> &Matrica<TipEl>::operator -=(const Matrica &m) {
        if(br_redova != m.br_redova || br_kolona != m.br_kolona) throw std::domain_error("Matrice nemaju jednake dimenzije!");
        for(int i = 0; i < br_redova; i++)
            for(int j = 0; j < br_kolona; j++)
                elementi[i][j] -= m.elementi[i][j];
        return *this;
    }

template <typename TipEl>
    Matrica<TipEl> &Matrica<TipEl>::operator *=(const Matrica &m) {
        if(br_kolona != m.br_redova) throw std::domain_error("Matrice nisu saglasne za mnozenje");
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
        if(i < 1 || i > br_redova || j < 1 || j > br_kolona) throw std::range_error("Neispravan indeks");
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
    void Matrica<TipEl>::SacuvajUTekstualnuDatoteku(const std::string &ime_fajla) const{
        std::ofstream izlaz(ime_fajla);
        if(!izlaz) throw std::logic_error("Datoteka ne postoji");               //mozda zamijeniti error poruku
        for(int i=0; i<br_redova; i++){
            for(int j=0; j<br_kolona; j++){
                izlaz << elementi[i][j];
                if(j!=br_kolona-1) izlaz << ",";
            }
            izlaz << std::endl;
        }
        if (!izlaz) throw std::logic_error("Problemi sa upisom u datoteku");     //ovo mozda bude moralo gore ako bude belaj
    }

template <typename TipEl>
    void Matrica<TipEl>::SacuvajUBinarnuDatoteku(const std::string &ime_fajla) const{
        std::ofstream izlaz(ime_fajla, std::ios::binary);
        if(!izlaz) throw std::logic_error("Datoteka ne postoji");
        izlaz.write(reinterpret_cast<const char*>(&br_redova), sizeof(br_redova));
        izlaz.write(reinterpret_cast<const char*>(&br_kolona), sizeof(br_kolona));
        for(int i = 0; i < br_redova; i++) {
            for(int j = 0; j < br_kolona; j++) {
                izlaz.write(reinterpret_cast<const char*>(&elementi[i][j]), sizeof(TipEl));
            }
        }
        if(!izlaz) throw std::logic_error("Problemi sa upisom u datoteku"); 
    }

template <typename TipEl>                                           //nek se ubije ova metoda i drugi autotest
    void Matrica<TipEl>::CitajTekst(const std::string &ime_fajla){
        std::ifstream tok(ime_fajla);
        if(!tok) throw std::logic_error("Trazena datoteka ne postoji");  

        int red{}, col{};
        std::string lajn;

        //prvi prolaz za određivanje velicine
        while(std::getline(tok, lajn)){
            std::istringstream strim(lajn);
            std::string element;
            int trenutna=0;
            while(std::getline(strim, element, ',')) {
                std::istringstream el_str(element);
                TipEl n;
                if(!(el_str>>n)){
                    throw std::logic_error("Datoteka sadrzi besmislene podatke"); 
                }
                trenutna++;
            }
            if(col==0) col=trenutna;
            else if(trenutna != col) throw std::logic_error("Datoteka sadrzi besmislene podatke"); 
            red++;
        }

        if(col==0 || red==0) throw std::logic_error("Datoteka sadrzi besmislene podatke"); 

        elementi = AlocirajMemoriju(red, col);
        br_kolona=col;
        br_redova=red;

        //drugi prolaz za elemente
        tok.clear();
        tok.seekg(0);
        for(int i=0; i<br_redova; i++){
            if(!std::getline(tok, lajn)){
                DealocirajMemoriju(elementi, br_redova);
                throw std::logic_error("Datoteka sadrzi besmislene podatke"); 
            }

            std::istringstream strim(lajn);
            for(int j=0; j<br_kolona; j++){
                std::string element;
                if(!std::getline(strim, element, ',')){
                    DealocirajMemoriju(elementi, br_redova);
                    throw std::logic_error("Datoteka sadrzi besmislene podatke"); 
                }
                std::istringstream element_strima(element);
                TipEl n;
                if(!(element_strima>>n)){
                    DealocirajMemoriju(elementi, br_redova);
                    throw std::logic_error("Datoteka sadrzi besmislene podatke"); 
                }
                elementi[i][j] = n;
            }
        }
    }

template <typename TipEl>
    void Matrica<TipEl>::CitajBinarno(const std::string &ime_fajla){
        std::ifstream tok(ime_fajla, std::ios::binary);
        if(!tok) throw std::logic_error("Trazena datoteka ne postoji");  

        int redovi{}, kolone{};
        tok.read(reinterpret_cast<char*>(&redovi), sizeof(redovi));
        tok.read(reinterpret_cast<char*>(&kolone), sizeof(kolone));

        elementi = AlocirajMemoriju(redovi, kolone);
        br_redova=redovi;
        br_kolona=kolone;
        for(int i = 0; i < redovi; i++) {
            for(int j = 0; j < kolone; j++) {
                tok.read(reinterpret_cast<char*>(&elementi[i][j]), sizeof(TipEl));
            }
        }
        if(!tok) DealocirajMemoriju(elementi, redovi), throw std::logic_error("Problemi pri citanju datoteke"); 
    }

template <typename TipEl>
    void Matrica<TipEl>::ObnoviIzTekstualneDatoteke(const std::string &ime_fajla){
        DealocirajMemoriju(elementi, br_redova);                             
        elementi=nullptr;
        br_redova = 0;
        br_kolona = 0;
        CitajTekst(ime_fajla);
    }

template <typename TipEl>
    void Matrica<TipEl>::ObnoviIzBinarneDatoteke(const std::string &ime_fajla){
        DealocirajMemoriju(elementi, br_redova);                         
        elementi=nullptr;
        CitajBinarno(ime_fajla);
    }

int main (){
    try {
        int r, k;
        std::cout << "Unesite broj redova i kolona za matricu: ";
        std::cin>>r>>k;
        Matrica<double> m(r, k, 'A');
        std::cout << "Unesite elemente matrice:\n";
        std::cin >> m;

        std::string tekst, binarno;
        std::cout << "Unesite ime tekstualne datoteke: ";
        std::cin>>tekst;
        std::cout << "Unesite ime binarne datoteke: ";
        std::cin>>binarno;
        m.SacuvajUTekstualnuDatoteku(tekst);
        m.SacuvajUBinarnuDatoteku(binarno);

        Matrica<double> mtxt(tekst, false);
        std::cout << "Ucitana matrica iz tekstualne datoteke:\n" << mtxt;
        Matrica<double> mbin(binarno, true);
        std::cout << "Ucitana matrica iz binarne datoteke:\n" << mtxt;
    }
    catch(const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
	return 0;
}