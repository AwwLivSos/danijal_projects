#include <algorithm>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <limits>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <iostream>

class Vector{
    std::vector<double> v;
    static void testKompatibilnost(const Vector &v1, const Vector &v2){
        if(v1.NElems() != v2.NElems()) throw std::domain_error("Incompatible formats");
    }
public:
    explicit Vector(int n);
    Vector(std::initializer_list<double> l);
    int NElems() const {return v.size();};
    double &operator[](int i) {return v[i];};
    double operator[](int i) const {return v[i];};
    double &operator()(int i);
    double operator()(int i) const;
    double Norm() const;
    friend double VectorNorm(const Vector &v);
    double GetEpsilon() const {return 10*(this->Norm())* std::numeric_limits<double>::epsilon();}
    void Print(char separator = '\n', double eps = -1) const;
    friend void PrintVector(const Vector &v, char separator, double eps);
    friend Vector operator +(const Vector &v1, const Vector &v2);
    Vector &operator +=(const Vector &v);
    friend Vector operator -(const Vector &v1, const Vector &v2);
    Vector &operator -=(const Vector &v);
    friend Vector operator *(double s, const Vector &v);
    friend Vector operator *(const Vector &v, double s);
    Vector &operator *=(double s);
    friend double operator *(const Vector &v1, const Vector &v2);
    friend Vector operator /(const Vector &v, double s);
    Vector &operator /=(double s);
};

Vector::Vector(int n){
    if(n<=0) throw std::range_error("Bad dimension");
    v.resize(n, 0);
}

Vector::Vector(std::initializer_list<double> l){
    if(l.size()==0) throw std::range_error("Bad dimension");
    for(double el : l) v.push_back(el);
}

double &Vector::operator()(int i){
    if(i<=0 || i>this->NElems()) throw std::range_error("Invalid index");
    return v[i-1];
}

double Vector::operator()(int i) const{                                         //isti fazon
    if(i<=0 || i>this->NElems()) throw std::range_error("Invalid index");
    return v[i-1];
}

double Vector::Norm() const{
    double sumaKvadrata = 0.0;                                                  //dodano da zapravo i bude double
    std::for_each(v.begin(), v.end(), [&sumaKvadrata](double element){          //GENIJALNO 
        return sumaKvadrata += element*element;
    });
    return std::sqrt(sumaKvadrata);                                             //doktorski fancy urađeno, bez direktnih petlji 
}

double VectorNorm(const Vector &v){
    double sumaKvadrata = 0.0;                                             
    std::for_each(v.v.begin(), v.v.end(), [&sumaKvadrata](double element){   
        return sumaKvadrata += element*element;
    });
    return std::sqrt(sumaKvadrata);
}

void Vector::Print(char separator, double eps) const{                           //mozda ima neko inteligentnije grananje al stas
    double granica = (eps<0) ? this->GetEpsilon() : eps;
    
    for(int i=0; i<this->NElems(); i++){
        std::cout<<(std::fabs(v[i])<granica ? 0 : v[i]);                        //nece nikad bit manje od GetEpsilon jer kad se pomnozi masinski epsilon sa necim jako malim bit ce jos manji valjda?

        if(i!=this->NElems()-1 || separator=='\n') std::cout<<separator;
    }
}

void PrintVector(const Vector &v, char separator = '\n', double eps = -1){
    v.Print(separator, eps);                                                    //sto da ne jelde?
}

Vector operator +(const Vector &v1, const Vector &v2){
    auto rez = v1;                                                              //plitka kopija, duboka kopija. Koga briga kad nema dinamickih dijelova
    return rez+=v2;
}

Vector& Vector::operator +=(const Vector &v){
    Vector::testKompatibilnost(*this, v);
    for(int i=0; i<v.NElems(); i++) this->v[i] += v[i];                         //kada se isto zovu, onda atribut gubi
    return *this;
}

Vector operator -(const Vector &v1, const Vector &v2){
    auto rez = v1;
    return rez-=v2;
}

Vector &Vector::operator -=(const Vector &v){
    Vector::testKompatibilnost(*this, v);
    for(int i=0; i<v.NElems(); i++) this->v[i] -= v[i];                         //kada se isto zovu, onda atribut gubi
    return *this;
}

Vector operator *(double s, const Vector &v){
    Vector rez = v;                                                             //ko šiša konstruktore lmao
    return rez*=s;
}

Vector operator *(const Vector &v, double s){                                   //inline? ma nema potrebe
    return (s*v);
}

Vector &Vector::operator *=(double s){
    for(auto &x : this->v) x*=s;
    return *this;    
}

double operator *(const Vector &v1, const Vector &v2){                          //kazu da ima neki mocan "inner_product" iz numerical biblioteke al neka
    Vector::testKompatibilnost(v1, v2);
    double rez=0.0;
    for(int i=0; i<v1.NElems(); i++){
        rez += (v1[i] * v2[i]);
    }
    return rez;
}

Vector operator /(const Vector &v, double s){
    Vector rez = v;
    return rez/=s;
}

Vector &Vector::operator /=(double s){
    if(s==0) throw std::domain_error("Division by zero");
    this->operator *=(1./s);
    return *this; 
}

void temeljitiTestKonstruktora(){
    try{
        Vector a(10);

        try{
            Vector b(0);
        }
        catch(const std::exception &e){
            std::cout<<std::endl<<e.what()<<", za konstrukciju Vector(0)";
        }

        try{
            Vector c(-10);
        }
        catch(const std::exception &e){
            std::cout<<std::endl<<e.what()<<", za konstrukciju Vector(-10)";
        }

        Vector d{1, 2, 3, 4, 5};

        try{
            Vector f{};
        }
        catch(const std::exception &e){
            std::cout<<std::endl<<e.what()<<", za konstrukciju Vector{}\n";
        }
        
        a.Print(',');
        std::cout<<std::endl;
        PrintVector(d);
        Vector g{-0.00001, -0.00002, -0.00003};
        g.Print(',', 0.0001);
        std::cout<<std::endl;
        Vector h{1e-300, 2e-300, 3e-300};
        h.Print();
        //rutinski test za ispis
    }
    catch(...){
        std::cout<<"Nesto ne valja";
        throw;
    }
    std::cout<<"\nSve ok!"<<std::endl<<std::endl;
}

void temeljitiTestOperatora(){
    try{
        Vector v{1.0, 2.0, 3.0};
        //rutinski test velicine
        if(v.NElems() != 3) std::cout<<"Kako ovo nije dobro?\n";
        else std::cout<<"Ok velicna!\n";

        for(int i=0; i<3; i++) std::cout<<v[i]<<" ";
        std::cout<<"\nTest [] je ok"<<std::endl;
        //ovo dvoje bi trebalo isto ispisati
        for(int i=1; i<=3; i++) std::cout<<v(i)<<" ";
        std::cout<<"\nTest () je ok"<<std::endl;

        const Vector w{4, 5, 6};

        for(int i=0; i<3; i++) std::cout<<w[i]<<" ";
        std::cout<<"\nTest [] za const verziju je ok"<<std::endl;
        //ovo dvoje bi trebalo isto ispisati
        for(int i=1; i<=3; i++) std::cout<<w(i)<<" ";
        std::cout<<"\nTest () za const verziju je ok"<<std::endl;

        try{
            v(0);
        }
        catch(std::exception &e){
            std::cout<<e.what()<<", za pristup elementu sa v(0)"<<std::endl;
        }

        try{
            v(4);
        }
        catch(std::exception &e){
            std::cout<<e.what()<<", za pristup elementu sa v(4), gdje vektor ima 3 elementa";
        }
        //također rutinski testovi
        std::cout<<std::endl<<v.GetEpsilon()<<" "<<VectorNorm(w);
    }
    catch(...){
        std::cout<<"Nesto ne valja";
        throw;
    }
    std::cout<<"\nSve ok!"<<std::endl<<std::endl;
}

void temeljitiTestSabiranjeOduzimanje(){
    try{
        const Vector a{1,2,3}, b{4,5,6};
        Vector c{7,8,9,10};

        auto d = a+b;
        //d treba biti 5,7,9
        d.Print('k');
        //usput testirani i drugi znakovi
        std::cout<<std::endl;
        c+={1,1,1,1};
        //automatska konverzija koja zapravo radi!
        //c treba biti 8,9,10,11
        c.Print(' ');
        std::cout<<std::endl;
        d-={10,10,10};
        //sada se treba ispisati -5,-3,-1
        d.Print('+');
        std::cout<<std::endl;
        Vector z=b-a;
        //z je 3,3,3
        z.Print('$');
        std::cout<<std::endl;

        try{
            c+=a;
        }
        catch(std::exception &e){
            std::cout<<e.what()<<", za dodavanje vektora koji nema istu dimenziju kao originalni"<<std::endl;
        }
        try{
            Vector f = b-c;
        }
        catch(std::exception &e){
            std::cout<<e.what()<<", za stvaranje novog vektora koji je sastavljen od 2 vektora razlicitih dimenzija";
        }
        
    }
    catch(...){
        std::cout<<"Nesto ne valja";
        throw;
    }
    std::cout<<"\nSve ok!"<<std::endl<<std::endl;
}

void temeljitiTestMnozenjeDijeljenje(){
    try{
        Vector a {10.1,20.2,30.3};
        a*=1.5;
        a.Print(' ');
        std::cout<<" ; ";
        a/=1.5;
        a.Print(' ');
        std::cout<<std::endl;           //mnozenja i dijeljenja su takva da ce na kraju morati biti isto kao originalno

        Vector b = 2.5*a;
        b.Print(' ');
        std::cout<<" ; ";
        Vector c = b/2.5;
        c.Print(' ');
        std::cout<<std::endl;

        std::cout<<std::setprecision(16)<<c*a<<" "<<a.Norm()*c.Norm()<<std::endl;
        //trebalo bi biti isto, mada ima razlika u 18. decimali

        Vector t{10,10,10,10};
        try{
            auto u =t*a;
        }
        catch(std::exception &e){
            std::cout<<e.what();
        }
    }
    catch(...){
        std::cout<<"Nesto ne valja";
        throw;
    }
    std::cout<<"\nSve ok!"<<std::endl<<std::endl;
}

class Matrix{
    std::vector<std::vector<double>> mat;
    static void provjeriKompatibilnostSabiranje(const Matrix &m1, const Matrix &m2){
        if(m1.NRows() != m2.NRows() || m1.NCols() != m2.NCols()) throw std::domain_error("Incompatible formats");
    }
public:
    Matrix(int m, int n);
    Matrix(const Vector &v);
    Matrix(std::initializer_list<std::vector<double>> l);
    int NRows() const{return mat.size();}
    int NCols() const{return mat[0].size();}
    double *operator[](int i) {return mat[i].data();}
    const double *operator[](int i) const {return mat[i].data();}
    double &operator()(int i, int j);
    double operator()(int i, int j) const;
    double Norm() const;
    friend double MatrixNorm(const Matrix &m) {return m.Norm();}
    double GetEpsilon() const {return 10*this->Norm()*std::numeric_limits<double>::epsilon();}
    void Print(int width = 10, double eps = -1) const;
    friend void PrintMatrix(const Matrix &m, int width = 10, double eps = -1) {m.Print(width, eps);}

    friend Matrix operator +(const Matrix &m1, const Matrix &m2);
    Matrix &operator +=(const Matrix &m);
    friend Matrix operator -(const Matrix &m1, const Matrix &m2);
    Matrix &operator -=(const Matrix &m);
    friend Matrix operator *(double s, const Matrix &m);
    friend Matrix operator *(const Matrix &m, double s);
    Matrix &operator *=(double s);
    friend Matrix operator *(const Matrix &m1, const Matrix &m2);
    Matrix &operator *=(const Matrix &m);
    friend Vector operator *(const Matrix &m, const Vector &v);
    friend Matrix Transpose(const Matrix &m);
    void Transpose();
};

Matrix::Matrix(int m, int n){
    if(m<=0 || n<=0) throw std::range_error("Bad dimension");
    mat.resize(m);
    for(auto &vecs : mat) vecs.resize(n, 0);
}

Matrix::Matrix(const Vector &v){
    mat.resize(v.NElems());
    int i=0;
    for(auto &vecs : mat) vecs.push_back(v[i++]); 
}

Matrix::Matrix(std::initializer_list<std::vector<double>> l){
    if(l.size()==0) throw std::range_error("Bad dimension");
    for(std::vector<double> vecs : l){
        if(vecs.size()!=l.begin()->size()) throw std::logic_error("Bad matrix");
        if(vecs.size()==0) throw std::range_error("Bad dimension");
    }
    for(std::vector<double> v : l) mat.push_back(v);
}

double &Matrix::operator()(int i, int j){
    if (i<=0 || j<=0 || i>NRows() || j>NCols()) throw std::range_error("Invalid index");
    return mat[i-1][j-1];
}

double Matrix::operator()(int i, int j) const{
    if (i<=0 || j<=0 || i>NRows() || j>NCols()) throw std::range_error("Invalid index");
    return mat[i-1][j-1];
}

double Matrix::Norm() const{
    double suma{};
    for(int i=0; i<NRows(); i++){
        for(int j=0; j<NCols(); j++)
            suma+=mat[i][j]*mat[i][j];                                      //ovdje necemo koristiti napredne fazone
    }
    return std::sqrt(suma);
}


void Matrix::Print(int width, double eps) const{
    double granica = (eps<0) ? this->GetEpsilon() : eps;

    for(auto v : mat){
        for(auto el : v)
            std::cout<<std::setw(width)<<(std::fabs(el)<granica ? 0 : el);
        std::cout<<std::endl;
    }
}

Matrix &Matrix::operator+=(const Matrix &m){
    provjeriKompatibilnostSabiranje(*this, m);
    for(int i=0; i<NRows(); i++){
        for(int j=0; j<NCols(); j++)
            mat[i][j]+=m[i][j];
    }
    return *this;
}

Matrix operator+(const Matrix &m1, const Matrix &m2){
    Matrix rez=m1;
    return rez+=m2;
}

Matrix &Matrix::operator-=(const Matrix &m){
    provjeriKompatibilnostSabiranje(*this, m);
    for(int i=0; i<NRows(); i++){
        for(int j=0; j<NCols(); j++)
            mat[i][j]-=m[i][j];
    }
    return *this;
}

Matrix operator-(const Matrix &m1, const Matrix &m2){
    Matrix rez=m1;
    return rez-=m2;
}

Matrix &Matrix::operator*=(double s){
    for(int i=0; i<NRows(); i++){
        for(int j=0; j<NCols(); j++)
            mat[i][j]*=s;
    }
    return *this;
}

Matrix operator *(double s, const Matrix &m){
    auto rez=m;
    return rez*=s;
}

Matrix operator *(const Matrix &m, double s){
    return s*m;
}

Matrix operator*(const Matrix &m1, const Matrix &m2){
    if(m1.NCols() != m2.NRows()) throw std::domain_error("Incompatible formats");
    Matrix m3(m1.NRows(), m2.NCols());

    for(int i=0; i<m1.NRows(); i++){
        for(int j=0; j<m2.NCols(); j++){
            for(int k=0; k<m1.NCols(); k++)
                m3[i][j] += m1[i][k] * m2[k][j];
        }
    }

    return m3;
}

Matrix &Matrix::operator *=(const Matrix &m){
    return *this = *this * m;
}

Vector operator*(const Matrix &m, const Vector &v){
    if(m.NCols() != v.NElems()) throw std::domain_error("Incompatible formats");
    Vector rez(m.NRows());

    for(int i=0; i<m.NRows(); i++) {
        for(int j=0; j<m.NCols(); j++) {
            rez[i] += m[i][j] * v[j];
        }
    }
    return rez;
}

void Matrix::Transpose(){
    if(NRows() == NCols()){
        for(int i=0; i<NRows(); i++) {
            for(int j=i+1; j<NCols(); j++)                      //+1 da ne mijenja džaba elemente na dijagonali
                std::swap(mat[i][j], mat[j][i]);
        }
    }
    else{
        Matrix temp(NCols(), NRows());

        for (int i=0; i<NRows(); i++) {
            for (int j=0; j<NCols(); j++)
                temp[j][i] = (*this)[i][j];
        }
        *this = temp;
    }
}

Matrix Transpose(const Matrix &m){
    auto rez = m;
    rez.Transpose();
    return rez;
}

void temeljitiTestKonstruktoraMatrice() {
    try {
        Matrix a(2, 3);  //normala

        try {
            Matrix b(0, 3);  //nula redovi
        } catch (const std::exception &e) {
            std::cout << e.what() << ", za Matrix(0, 3)\n";
        }

        try {
            Matrix c(3, 0);  //nula kolone
        } catch (const std::exception &e) {
            std::cout << e.what() << ", za Matrix(3, 0)\n";
        }

        try {
            Matrix d(0, 0);  //nula dimenzije
        } catch (const std::exception &e) {
            std::cout << e.what() << ", za Matrix(0, 0)\n";
        }

        Matrix e{{1, 2, 3}, {4, 5, 6}};
        e.Print(5);  //test printanja sa sirinom

        Matrix f{Vector{1, 2, 3}};
        PrintMatrix(f, 5);  //printanje po friend funkciji
    }
    catch(...){
        std::cout<<"Nesto ne valja";
        throw;
    }
    std::cout<<"\nSve ok za konstruktore matrice!"<<std::endl<<std::endl;
}

void temeljitiTestOperatoraMatrice() {
    try {
        const Matrix m{{1, 2, 3}, {4, 5, 6}};       //samo cemo const testirati

        //testiranje za uglaste zagrade
        for (int i=0; i<m.NRows(); i++) {
            for (int j=0; j<m.NCols(); j++)
                std::cout << m[i][j] << " ";
            std::cout << std::endl;
        }
        std::cout << "[] operator test prosao\n";

        //testiranje za obicne okrugle
        for (int i=1; i<=m.NRows(); i++) {
            for (int j=1; j<=m.NCols(); j++)
                std::cout << m(i, j) << " ";
            std::cout << std::endl;
        }
        std::cout << "() operator test prosao\n";

        try {
            std::cout << m(0, 1); 
        } catch (const std::exception &e) {
            std::cout << e.what() << ", za Matrix(0, 1)\n";
        }

        try {
            std::cout << m(3, 1);  
        } catch (const std::exception &e) {
            std::cout << e.what() << ", za Matrix(3, 1)\n";
        }

        std::cout<<"Rutinski testovi za normu i epsilon: "<<m.Norm()<<" "<<MatrixNorm(m)<<std::endl<<m.GetEpsilon();
    }
    catch(...){
        std::cout<<"Nesto ne valja";
        throw;
    }
    std::cout<<"\nSve ok za operatore matrice!"<<std::endl<<std::endl;
}

void temeljitiTestSabiranjaOduzimanjaMatrice() {
    try {
        Matrix a{{1, 2, 3}, {4, 5, 6}};
        Matrix b{{6, 5, 4}, {3, 2, 1}};
        auto c = a + b;
        c.Print(5); 
        std::cout<<std::endl;

        a += b;
        a.Print(5);
        std::cout<<std::endl;

        auto d = b - a;
        d.Print(5);
        std::cout<<std::endl;

        try {
            Matrix e{{1, 2}, {3, 4}};
            auto f = a + e;
        } catch (const std::exception &e) {
            std::cout << e.what() << ", za sabiranje nekompatibilnih matrica\n";
        }

        Matrix negativnaMatrix{{-1, -2, -3}, {-4, -5, -6}};
        a-=b;
        auto h = a + negativnaMatrix;
        h.Print(5);
        std::cout<<"Trebale bi biti sve nule\n";

        Matrix samotnjak{{5}};
        auto i = samotnjak + samotnjak;
        i.Print(5);
        std::cout<<"jedan broj haha\n";

        Matrix velka{{1e9, 2e9}, {3e9, 4e9}};
        Matrix maja{{1e-9, 2e-9}, {3e-9, 4e-9}};
        auto j = velka - maja;
        j.Print();
        std::cout<<"Ostace iste velike vrijednosti, naravno\n";

    }
    catch(...){
        std::cout<<"Nesto ne valja";
        throw;
    }
    std::cout<<"\nSve ok za sabiranje i oduzinmaje u matricama!"<<std::endl<<std::endl;
}

void temeljitiTestMnozenjaMatrice() {
    try {
        Matrix a{{1, 2, 3}, {4, 5, 6}};
        Matrix b{{7, 8}, {9, 10}, {11, 12}};
        auto c = a * b;
        c.Print(); 
        std::cout<<std::endl;

        a *= 2.5;
        a.Print();
        std::cout<<std::endl;

        Matrix d = 0.5 * b;
        d.Print();  //skalarno mnozenje SLijeva
        std::cout<<std::endl;

        Matrix e = b * 3;
        e.Print();  //skalarno mnozenje ZDesna
        std::cout<<std::endl;

        Vector v{1, 2, 3};
        Vector result = a * v;
        result.Print(' ');  //matrix-vector mnozenje
        std::cout<<std::endl;

        try {
            Matrix f{{1, 2}, {3, 4}};
            auto g = f * v; 
        } catch (const std::exception &e) {
            std::cout << e.what() << ", za nekompatibilna matrix-vector mnozenja\n";
        }

        Matrix negativnaMatrix{{-1, -2, -3}, {-4, -5, -6}};
        auto i = negativnaMatrix * (-1);
        i.Print();
        std::cout<<std::endl;

        Matrix samotnjak{{5}};
        auto j = samotnjak * samotnjak;
        j.Print();
        std::cout<<std::endl;

        Matrix identity{{1, 0}, {0, 1}};
        auto k = identity * identity;
        k.Print();
        std::cout<<std::endl;

    }
    catch(...){
        std::cout<<"Nesto ne valja";
        throw;
    }
    std::cout<<"\nSve ok za mnozenje u matricama!"<<std::endl<<std::endl;
}

void temeljitiTestTransponovanjaMatrice() {
    try {
        Matrix a{{1, 2, 3}, {4, 5, 6}};
        a.Print(3);
        std::cout<<std::endl;
        Matrix b = Transpose(a);
        b.Print(3); 
        std::cout<<std::endl;

        b.Transpose();
        b.Print(3); 
        std::cout<<std::endl;       //dupla transponovacija

        Matrix c{{1, 2}, {3, 4}};
        c.Print(3);
        std::cout<<std::endl;
        c.Transpose();
        c.Print(3);
        std::cout<<std::endl;

    } 
    catch(...){
        std::cout<<"Nesto ne valja";
        throw;
    }
    std::cout<<"\nSve ok za transponovanje matrica!"<<std::endl<<std::endl;
}

int main(){
    temeljitiTestKonstruktora();
    temeljitiTestOperatora();
    temeljitiTestSabiranjeOduzimanje();
    temeljitiTestMnozenjeDijeljenje();

    temeljitiTestKonstruktoraMatrice();
    temeljitiTestOperatoraMatrice();
    temeljitiTestSabiranjaOduzimanjaMatrice();
    temeljitiTestMnozenjaMatrice();
    temeljitiTestTransponovanjaMatrice();

    /*Vector v{1,2,3};
    Matrix m1{{1,2,3},{4,5,6},{7,8,9}};
    Matrix m2{{1,2,3},{4,5,6}};
    m1*=2;
    m2.Transpose();
    m2.Print();*/

    return 0;
}