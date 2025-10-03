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

    void Chop(double eps = -1);
    bool EqualTo(const Vector &v, double eps = -1) const;
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

double Vector::operator()(int i) const{                                         
    if(i<=0 || i>this->NElems()) throw std::range_error("Invalid index");
    return v[i-1];
}

double Vector::Norm() const{
    double sumaKvadrata = 0.0;                                                  
    std::for_each(v.begin(), v.end(), [&sumaKvadrata](double element){          
        return sumaKvadrata += element*element;
    });
    return std::sqrt(sumaKvadrata);                                             
}

double VectorNorm(const Vector &v){
    double sumaKvadrata = 0.0;                                             
    std::for_each(v.v.begin(), v.v.end(), [&sumaKvadrata](double element){   
        return sumaKvadrata += element*element;
    });
    return std::sqrt(sumaKvadrata);
}

void Vector::Print(char separator, double eps) const{                           
    double granica = (eps<0) ? this->GetEpsilon() : eps;
    
    for(int i=0; i<NElems(); i++){
        std::cout<<(std::fabs(v[i])<granica ? 0 : v[i]);                        

        if(i!=NElems()-1 || separator=='\n') std::cout<<separator;
    }
}

void PrintVector(const Vector &v, char separator = '\n', double eps = -1){
    v.Print(separator, eps);                                                    
}

Vector operator +(const Vector &v1, const Vector &v2){
    auto rez = v1;                                                              
    return rez+=v2;
}

Vector& Vector::operator +=(const Vector &v){
    Vector::testKompatibilnost(*this, v);
    for(int i=0; i<v.NElems(); i++) this->v[i] += v[i];                         
    return *this;
}

Vector operator -(const Vector &v1, const Vector &v2){
    auto rez = v1;
    return rez-=v2;
}

Vector &Vector::operator -=(const Vector &v){
    Vector::testKompatibilnost(*this, v);
    for(int i=0; i<v.NElems(); i++) this->v[i] -= v[i];                        
    return *this;
}

Vector operator *(double s, const Vector &v){
    Vector rez = v;                                                            
    return rez*=s;
}

Vector operator *(const Vector &v, double s){                                  
    return (s*v);
}

Vector &Vector::operator *=(double s){
    for(auto &x : this->v) x*=s;
    return *this;    
}

double operator *(const Vector &v1, const Vector &v2){                         
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

void Vector::Chop(double eps){
    double granica = (eps<0) ? this->GetEpsilon() : eps;
    for(int i=0; i<NElems(); i++){
        if(std::fabs(v[i])<granica) v[i]=0;                        
    }
}

bool Vector::EqualTo(const Vector &v, double eps) const{
    if(NElems() != v.NElems()) return false;

    double granica = (eps<0) ? this->GetEpsilon() : eps;
    for(int i=0; i<NElems(); i++){
        if(std::fabs(v[i] - this->v[i]) > granica) return false;                     
    }
    return true;
}

class Matrix{
    std::vector<std::vector<double>> mat;
    static void provjeriKompatibilnostSabiranje(const Matrix &m1, const Matrix &m2){
        if(m1.NRows() != m2.NRows() || m1.NCols() != m2.NCols()) throw std::domain_error("Incompatible formats");
    }

    void Pivotiziraj(Vector &b);

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
    friend void PrintMatrix(const Matrix &m, int width, double eps);

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

    void Chop(double eps = -1);
    bool EqualTo(const Matrix &m, double eps = -1) const;
    friend Matrix LeftDiv(Matrix m1, Matrix m2);
    friend Vector LeftDiv(Matrix m, Vector v);
    friend Matrix operator /(const Matrix &m, double s);
    Matrix &operator /=(double s);
    friend Matrix operator /(Matrix m1, Matrix m2);
    Matrix &operator /=(Matrix m);
    double Det() const;
    friend double Det(Matrix m);
    void Invert();
    friend Matrix Inverse(Matrix m);
    void ReduceToRREF(int *k = nullptr);        //da bi funkcionisalo skracivanje sa rang i rref, potreban je default argument za ovu metodu da ne mijenjamo njeno pozivanje
    friend Matrix RREF(Matrix m);
    int Rank() const;
    friend int Rank(Matrix m);
};

void PrintMatrix(const Matrix &m, int width = 10, double eps = -1) {m.Print(width, eps);}

void Matrix::Pivotiziraj(Vector &b){                    //pivotizacija urađena po pseudokodu u predavanjima
    auto eps = GetEpsilon();
    for(int k=0; k<NRows(); k++){
        int p = k;
        for(int i = k+1; i<NRows(); i++){
            if(std::fabs(mat[i][k]) > std::fabs(mat[p][k])) p=i;
        }
        if(std::fabs(mat[p][k]) < eps) throw std::domain_error("Divisor matrix is singular");
        if(p!=k){
            std::swap(mat[k], mat[p]);
            std::swap(b[k], b[p]);
        }
        for(int i=k+1; i<NRows(); i++){
            auto mi = mat[i][k]/mat[k][k];
            for(int j=k+1; j<NRows(); j++){
                mat[i][j] -= (mi * mat[k][j]);
            }
            b[i] -= (mi*b[k]);
        }
    }
}

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
            suma+=mat[i][j]*mat[i][j];                                    
    }
    return std::sqrt(suma);
}


void Matrix::Print(int width, double eps) const{
    double granica = (eps<0) ? this->GetEpsilon() : eps;

    for(auto v : mat){
        for(auto el : v)
            std::cout<<std::setw(el>=0 || std::fabs(el)<granica ? width : width+1)<<(std::fabs(el)<granica ? 0 : el);
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
            for(int j=i+1; j<NCols(); j++)                     
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

void Matrix::Chop(double eps){
    double granica = (eps<0) ? this->GetEpsilon() : eps;
    for(int i=0; i<NRows(); i++){
        for(int j=0; j<NCols(); j++)
            if(std::fabs(mat[i][j]) < eps) mat[i][j] = 0;
    }
}

bool Matrix::EqualTo(const Matrix &m, double eps) const{
    double granica = (eps<0) ? this->GetEpsilon() : eps;
    if (m.NCols() != NCols() || m.NRows() != NRows()) return false;

    for(int i=0; i<NRows(); i++){
        for(int j=0; j<NCols(); j++)
            if(std::fabs(mat[i][j] - m[i][j]) > granica) return false;
    }
    return true;
}

Matrix LeftDiv(Matrix m1, Matrix m2){                       //za ovu funkciju ne mogu reusat pivotizaciju zbog jako sitnih promjena al valjda mogu logiku
    if(m1.NRows() != m1.NCols()) throw std::domain_error("Divisor matrix is not square");
    if(m1.NRows() != m2.NRows()) throw std::domain_error("Incompatible formats");

    double eps = m1.GetEpsilon();
    for(int k=0; k<m1.NRows(); k++){

        int p = k;                              //kopiranje logike kao u pivotizaciji s vektorom, dzaba i pisao onu funkciju
        for(int i = k+1; i<m1.NCols(); i++)
            if(std::fabs(m1.mat[i][k]) > std::fabs(m1.mat[p][k])) p=i;
        
        if(std::fabs(m1.mat[p][k]) < eps) throw std::domain_error("Divisor matrix is singular");

        if(p!=k){           //mozda pokusam kasnije sa nekom typename, typeid, auto, default argument ili slicno konstrukcijom u pivotizaciji, mozda ne bude potrebe da prepisujem
            std::swap(m1.mat[k], m1.mat[p]);
            std::swap(m2.mat[k], m2.mat[p]);
        }
        for(int i=k+1; i<m1.NCols(); i++){
            double mi = m1.mat[i][k]/m1.mat[k][k];
            for(int j=k+1; j<m1.NCols(); j++)
                m1.mat[i][j] -= (mi * m1.mat[k][j]);
            for(int j=0; j<m2.NCols(); j++)
                m2.mat[i][j] -= (mi*m2.mat[k][j]);
        }
    }

    for(int k = 0; k<m2.NCols(); k++){              //standardna back supstitucija
        for(int i=m1.NRows()-1; i>=0; i--){
            for(int j=i+1; j<m1.NCols(); j++){
                m2[i][k] -= m1[i][j] * m2[j][k];
            }
            m2[i][k]/=m1[i][i];
        }
    }
    return m2;
}

Vector LeftDiv(Matrix m, Vector v){         //OVA RADI KAKO TREBA!!!1!!!11!
    if(m.NRows() != m.NCols()) throw std::domain_error("Divisor matrix is not square");
    if(v.NElems() != m.NRows()) throw std::domain_error("Incompatible formats");

    m.Pivotiziraj(v);                       //PRVO PIVOTIZACIJA

    for(int i=m.NRows()-1; i>=0; i--){
        for(int j=i+1; j<m.NCols(); j++){
            v[i] -= m[i][j] * v[j];         //OVDJE STANDARDNE OPERACIJE
        }
        v[i]/=m[i][i];     
    }
    return v;
}

Matrix operator /(const Matrix &m, double s){
    auto rez = m;
    return rez/=s;
}

Matrix &Matrix::operator /=(double s){
    if(s==0) throw std::domain_error("Division by zero");
    this->operator *=(1./s);
    return *this;
}

Matrix operator /(Matrix m1, Matrix m2){
    auto rez = m1;
    return rez/=m2;
}

Matrix &Matrix::operator /=(Matrix m) {
    if (m.NCols() != m.NRows())
        throw std::domain_error("Divisor matrix is not square");
    if (this->NCols() != m.NCols())
        throw std::domain_error("Incompatible formats");

    int n = m.NRows();   
    int m_rows = this->NRows();

    for (int k = 0; k < n; k++) {
        int p = k;
        for (int i = k + 1; i < n; i++) {
            if (std::fabs(m.mat[k][i]) > std::fabs(m.mat[k][p]))
                p = i;
        }

        if (std::fabs(m[k][p]) < m.GetEpsilon())
            throw std::domain_error("Divisor matrix is singular");

        if (p != k) {
            for (int i = 0; i < n; i++)
                std::swap(m[i][p], m[i][k]);
            for (int i = 0; i < m_rows; i++)
                std::swap((*this)[i][p], (*this)[i][k]);
        }

        for (int i = k + 1; i < n; i++) {
            double mi = m[k][i] / m[k][k];
            for (int j = k + 1; j < n; j++)
                m[j][i] -= mi * m[j][k];
            for (int j = 0; j < m_rows; j++)
                (*this)[j][i] -= mi * (*this)[j][k];
        }
    }

    for (int k = 0; k < m_rows; k++) {
        for (int i = n - 1; i >= 0; i--) {
            double s = (*this)[k][i];
            for (int j = i + 1; j < n; j++)
                s -= m[j][i] * (*this)[k][j];
            (*this)[k][i] = s / m[i][i];
        }
    }

    return *this;
}


double Matrix::Det() const{
    if(NCols() != NRows()) throw std::domain_error("Matrix is not square");
    Matrix temp(*this);

    double det=1;
    for(int k=0; k<NRows(); k++){
        int p=k;
        for(int i=k+1; i<NRows(); i++){
            if(std::fabs(temp[i][k]) > std::fabs(mat[p][k])) p=i;
        }
        if(std::fabs(temp[p][k]) < GetEpsilon()) return 0;
        if(p!=k){
            std::swap(temp.mat[k], temp.mat[p]);
            det*=-1;
        }
        det*=temp[k][k];
        for(int i=k+1; i<NRows(); i++){
            auto mi = temp[i][k]/temp[k][k];
            for(int j=k+1; j<NRows(); j++){
                temp[i][j] -= (mi * temp[k][j]);
            }
        }
    }

    return det;
}

double Det(Matrix m){
    return m.Det();
}

void Matrix::Invert(){
    if(NCols() != NRows()) throw std::domain_error("Matrix is not square");
    std::vector<int> w(NRows());

    for(int k=0; k<NRows(); k++){
        int p=k;
        for(int i=k+1; i<NRows(); i++){
            if(std::fabs(mat[i][k]) > std::fabs(mat[p][k])) p=i;
        }
        if(std::fabs(mat[p][k]) < GetEpsilon()) throw std::domain_error("Matrix is singular");
        if(p!=k) std::swap(mat[k], mat[p]);
        w[k] = p;
        double mi = mat[k][k];
        mat[k][k] = 1;
        for(int j=0; j<NRows(); j++) mat[k][j]/=mi;
        for(int i=0; i<NRows(); i++){
            if(i!=k){
                mi = mat[i][k];
                mat[i][k] =0;
                for(int j=0; j<NRows(); j++) mat[i][j] -= (mi*mat[k][j]);
            }
        }
    }
    for(int j=NRows()-1; j>=0; j--){
        int p=w[j];
        if(p!=j) 
            for(int i=0; i<NRows(); i++) std::swap(mat[i][p], mat[i][j]);
    }
}

Matrix Inverse(Matrix m){
    m.Invert();
    return m;
}

//metoda ce primiti pokazivac koji ce imati vrijednost k po pseudokodu
void Matrix::ReduceToRREF(int *k){      //po pseudokodu, uz malu malecku izmjenu
    int lokalno_k;                      //potrebna je neka lokalna varijabla jer inace bi dereferencirali nullptr sto je big no no
    if(k==nullptr) k=&lokalno_k;        //ova provjera sluzi ako radimo rang, pri cemu bi promjena k na lokalno onda bi se izgubila vrijednost ranga i ne bi se mogla vratiti
    (*k)=-1;                            //ostatak varijeble k se koristi najnormalnije
    int l=-1;
    auto eps = GetEpsilon();

    std::vector<bool> w(NCols(), 0);

    while(*k<NRows() && l<NCols()){
        l++;
        (*k)++;
        double v=0;
        int p = 0;
        while(v<eps && l<NCols()){
            p=*k;
            for(int i=*k; i<NRows(); i++){ 
                if(std::fabs(mat[i][l]) > v){
                    v=std::fabs(mat[i][l]);
                    p=i;
                }
            }
            if(v<eps) l++;
        }

        if(l<NCols()){
            w[l] = true;
            if(p!=*k) std::swap(mat[*k], mat[p]);
            double mi = mat[*k][l];
            for(int j=l; j<NCols(); j++) mat[*k][j]/=mi;
            for(int i=0; i<NRows(); i++){
                if(i!=*k){
                    mi = mat[i][l];
                    for(int j=l; j<NCols(); j++) 
                        mat[i][j] -= (mi * mat[*k][j]);
                }
            }
        }
    }
}

Matrix RREF(Matrix m){
    auto rez = m;
    rez.ReduceToRREF();
    return rez;
}

int Matrix::Rank() const{       //genijalnooo, prvi put da sam uspio umanjit kod
    auto temp = *this;
    auto eps = GetEpsilon();
    int rang{};                 //posto nakon rref-a k ima vrijednost ranga, samo cemo pozvati rref za temp matricu i poslati pokazivac koji ce na kraju biti rang

    temp.ReduceToRREF(&rang);

    return rang;
}

int Rank(Matrix m){
    return m.Rank();
}

class LUDecomposer{
    Matrix LU;
    Vector w;
public:
    LUDecomposer(Matrix m);
    void Solve(const Vector &b, Vector &x) const;
    Vector Solve(Vector b) const;
    void Solve(const Matrix &b, Matrix &x) const;
    Matrix Solve(Matrix b) const;
    Matrix GetCompactLU() const {return LU;}
    Matrix GetL() const;
    Matrix GetU() const;
    Vector GetPermuation() const {return w;}
};

LUDecomposer::LUDecomposer(Matrix m) : LU(m.NRows(), m.NCols()), w(m.NRows()){
    if(m.NRows() != m.NCols()) throw std::domain_error("Matrix is not square");

    int n = m.NRows();
    for(int i = 0; i < n; i++) w[i] = i;

    for(int j = 0; j < n; j++) {                                        //efekat da prolazimo kroz kolone a ne redove je postignut tako sto prva petlja ima brojac j a druga i, sto intuitivno postane jasno 
        for(int i = 0; i <= j; i++) {
            double s = m[i][j];
            for(int k = 0; k < i; k++) s -= (m[i][k]*m[k][j]);
            m[i][j] = s;
        }
        int p = j;
        for(int i = j+1; i < n; i++) {
            double s = m[i][j];
            for(int k = 0; k < j; k++) s -= (m[i][k]*m[k][j]);
            m[i][j] = s;
            if(std::fabs(s) > std::fabs(m[p][j])) p = i;
        }
        if(std::fabs(m[p][j]) < m.GetEpsilon())
            throw std::domain_error("Matrix is singular");
        if(p != j) {
            for(int k = 0; k < n; k++) std::swap(m[p][k], m[j][k]);
        }
        w[j]=p;
        double mi = m[j][j];
        for(int i = j+1; i < n; i++) m[i][j] /= mi;
    }
    LU = m;
}

void LUDecomposer::Solve(const Vector &b, Vector &x) const{
    if(LU.NRows() != b.NElems() || LU.NRows() != x.NElems()) throw std::domain_error("Incompatible formats");

    x=b;            //ima smisla valjda

    for(int i=0; i<b.NElems(); i++){
        int p = w[i];
        double s = x[p];  
        x[p] = x[i];
        for (int j = 0; j < i; j++) s -= (LU[i][j] * x[j]);
        x[i] = s;
    }
    for (int i = x.NElems() - 1; i >= 0; i--) {
        double s = x[i];
        for (int j = i + 1; j < x.NElems(); j++) s -= (LU[i][j] * x[j]);
        x[i] = s / LU[i][i];
    }
}

Vector LUDecomposer::Solve(Vector b) const{
    if (LU.NRows() != b.NElems()) throw std::domain_error("Incompatible formats");

    for (int i = 0; i < b.NElems(); i++) {
        int p = w[i];
        double s = b[p];
        b[p] = b[i];
        for (int j = 0; j < i; j++)
            s -= (LU[i][j] * b[j]);
        b[i] = s;
    }

    for (int i = b.NElems() - 1; i >= 0; i--) {
        double s = b[i];
        for (int j = i + 1; j < b.NElems(); j++)
            s -= (LU[i][j] * b[j]);
        b[i] = s / LU[i][i];
    }

    return b; 
}

void LUDecomposer::Solve(const Matrix &b, Matrix &x) const{
    if (x.NCols() != b.NCols()) throw std::domain_error("Incompatible formats");
    if (x.NRows() != LU.NRows()) throw std::domain_error("Incompatible formats");
    if (LU.NCols() != x.NCols() || LU.NRows() != b.NRows()) throw std::domain_error("Incompatible formats");

    for(int j=0; j<b.NCols(); j++){
        Vector kol(b.NCols());
        for(int i=0; i<b.NRows(); i++) kol[i] = b[i][j];
        
        Vector solv = Solve(kol);

        for(int i=0; i<x.NRows(); i++) x[i][j] = solv[i];
    }
}

Matrix LUDecomposer::Solve(Matrix b) const {
    Matrix x(b.NRows(), b.NCols());
    Solve(b, x);
    return x;
}

Matrix LUDecomposer::GetL() const {
    Matrix L(LU.NRows(), LU.NCols());
    for(int i = 0; i < L.NRows(); i++) {
        for(int j = 0; j < L.NCols(); j++) {
            if(j < i)
                L[i][j] = LU[i][j];
            else if(i == j)
                L[i][j] = 1;
            else
                L[i][j] = 0;
        }
    }
    return L;
}

Matrix LUDecomposer::GetU() const {
    Matrix L(LU.NRows(), LU.NCols());
    for(int i = 0; i < L.NRows(); i++) {
        for(int j = 0; j < L.NCols(); j++) {
            if(j >= i)
                L[i][j] = LU[i][j];
            else
                L[i][j] = 0;
        }
    }
    return L;
}

class QRDecomposer{
    Matrix House;
    Vector diagonal;
    
    Vector GetColumn(const Matrix& m, int colIdx) const{
        Vector col(m.NRows());              
        for (int i = 0; i < m.NRows(); i++) col[i] = m[i][colIdx];
        return col;
    }
    void SetColumn(Matrix& m, int colIdx, const Vector& col) const{
        for (int i = 0; i < m.NRows(); i++) m[i][colIdx] = col[i];
    }
    Matrix Quiraj(Matrix m, bool jelTrans) const;                  //jos jedan pokusaj skracivanja
    Vector Quiraj(Vector v, bool jelTrans) const;
    //ovo su samo pomocne funkcijice koje su mozda mogle i biti koirstene za ostale metode, ali ovdje su posluzile za multqwith
public:
    QRDecomposer(Matrix m);
    void Solve(const Vector &b, Vector &x) const;
    Vector Solve(Vector b) const;
    void Solve(Matrix &b, Matrix &x) const;
    Matrix Solve(Matrix b) const;
    Vector MulQWith(Vector v) const {return Quiraj(v, false);}      //ovdje koristimo zadani flag, uz naravno i izbor vektora i matrice
    Matrix MulQWith(Matrix m) const {return Quiraj(m, false);}
    Vector MulQTWith(Vector v) const {return Quiraj(v, true);}
    Matrix MulQTWith(Matrix m) const {return Quiraj(m, true);}
    Matrix GetQ() const;
    Matrix GetR() const;
};

QRDecomposer::QRDecomposer(Matrix m): diagonal(m.NRows()), House(m.NRows(), m.NCols()) {
        if(m.NRows() < m.NCols()) throw std::domain_error("Invalid matrix format");
        Matrix v(m.NRows(), m.NCols());
        for(int k = 0; k < m.NCols(); k++) {
            double s = 0;
            for(int i = k; i < m.NRows(); i++) s += m[i][k] * m[i][k];
            s = std::sqrt(s);

            double mi = std::sqrt(s*(s+std::fabs(m[k][k])));
            if(m[k][k] < 0) s = -s;
            if(mi < m.GetEpsilon()) throw std::domain_error("Matrix is singular");
            v[k][k] = (m[k][k] + s) / mi;
            for(int i = k+1; i < m.NRows(); i++)
                v[i][k] = m[i][k] / mi; 

            m[k][k] = -s;
            for(int j = k+1; j < m.NCols(); j++) {
                s = 0;
                for(int i = k; i < m.NRows(); i++)
                    s += m[i][j] * v[i][k];
                for(int i = k; i < m.NRows(); i++)
                    m[i][j] -= s * v[i][k];
            }
        }
        for(int i = 0; i < House.NRows(); i++) {
            for(int j = 0; j < House.NCols(); j++) {
                if(j <= i)
                    House[i][j] = v[i][j];
                else
                    House[i][j] = m[i][j];
            }
        }
        for(int i = 0; i < diagonal.NElems(); i++)
            diagonal[i] = m[i][i];
    }

void QRDecomposer::Solve(const Vector &b, Vector &x) const {
    if(House.NRows() != House.NCols()) throw std::domain_error("Matrix is not square");
    if(b.NElems() != House.NRows()) throw std::domain_error("Incompatible formats");
    if(House.NCols() != x.NElems()) throw std::domain_error("Incompatible formats");
    
    Vector b1 = b;
    Vector QTb = MulQTWith(b1);
    for(int k = 0; k < House.NRows(); k++) {
        for(int i = House.NCols()-1; i >= 0; i--) {
            double s = QTb[i];
            for(int j = i+1; j < House.NCols(); j++)
                s -= House[i][j] * x[j];
            x[i] = s / diagonal[i];
        }
    }
}

Vector QRDecomposer::Solve(Vector b) const {
    Vector x(b.NElems());
    Solve(b, x);
    return x;
}

void QRDecomposer::Solve(Matrix &b, Matrix &x) const {
        if(House.NRows() != House.NCols()) throw std::domain_error("Matrix is not square");
        if(b.NRows() != House.NRows()) throw std::domain_error("Incompatible formats");
        if(b.NCols() != x.NCols()) throw std::domain_error("Incompatible formats");
        if(x.NRows() != House.NCols()) throw std::domain_error("Incompatible formats");
        for(int i = 0; i < b.NCols(); i++) {
            Vector bCol(b.NRows());
            for(int j = 0; j < b.NRows(); j++)
                bCol[j] = b[j][i];
            Vector xCol = Solve(bCol);
            for(int j = 0; j < x.NRows(); j++)
                x[j][i] = xCol[j];
        }
    }

Matrix QRDecomposer::Solve(Matrix b) const {
    Matrix x(b.NRows(), b.NCols());
    Solve(b, x);
    return x;
}
                                //flag za transponovanje koristimo da znamo kakvu vrstu opearcije radimo, ista se koristi i dole
Vector QRDecomposer::Quiraj(Vector v, bool jelTrans) const {
    if (v.NElems() != House.NRows()) throw std::domain_error("Incompatible formats");

    for (int k = (jelTrans ? 0 : House.NCols() - 1); 
        (jelTrans ? k < House.NCols() : k >= 0); 
        (jelTrans ? k++ : k--)) {                   //vrlo nekonvencionalno, ali dodali smo uvjet za pocetak i kretanje u petlji, u zavinsosti od oznacenog flaga, jer u transponovanju idemo ozada npr
        
        double s = 0;
        for (int i = k; i < House.NRows(); i++)     //ostale su komande po pseudokodu
            s += House[i][k] * v[i];
        for (int i = k; i < House.NRows(); i++)
            v[i] -= s * House[i][k];
    }
    return v;
}

Matrix QRDecomposer::Quiraj(Matrix m, bool jelTrans) const {            //isti fazon samo za cijelu matricu
    if (m.NRows() != House.NRows()) throw std::domain_error("Incompatible formats");

    for (int i = 0; i < m.NCols(); i++) {
        Vector col = GetColumn(m, i); //uzmi tu kolonu
        col = Quiraj(col, jelTrans);  //primjeni transformaciju za vektore
        SetColumn(m, i, col);         //postavi dobijeni rez, mozda viska funkcija al ljepse izgleda malo
    }
    return m;
}                                          

Matrix QRDecomposer::GetR() const{
    int cols = House.NCols();
    Matrix r(cols, cols);
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < cols; j++) {
            if (j > i) r[i][j] = House[i][j];
        }
    }
    for(int i=0; i<r.NCols(); i++) r[i][i] = diagonal[i];
    return r;
}

Matrix QRDecomposer::GetQ() const{
    Matrix V(House);                        //ne kontam potrebu za ovim
    Matrix Q(House.NRows(), House.NCols());

    for(int i = 0; i < V.NRows(); i++)
        for(int j = 0; j < V.NCols(); j++)
            if(j > i) V[i][j] = V[j][i];

    for(int j = 0; j < V.NCols(); j++) {
        for(int i = 0; i < V.NRows(); i++) Q[i][j] = 0;
        Q[j][j] = 1;
        
        for(int k = V.NCols()-1; k >= 0; k--) {
            double s = 0;
            for(int i = k; i < V.NRows(); i++)
                s += V[i][k] * Q[i][j];
            for(int i = k; i < V.NRows(); i++)
                Q[i][j] -= s * V[i][k];
        }
    }
    return Q;
}

void temeljitiTestNovihMetoda(){
    try {
        Matrix A{{1, 2}, {3, 2}};
        Matrix AChop{{1, 2}, {3, 2}};
        Matrix B{{3, 2}, {0, 1}};
        Matrix SingularMatrix{{1, 2}, {2, 4}};
        Vector v{3, 5};

        std::cout << "Originalna matrica A:\n";
        A.Print();
        AChop.Chop(3);
        std::cout << "\nMatrica A nakon Chop(3):\n";
        AChop.Print();

        std::cout << "\nA.EqualTo(A): " << (A.EqualTo(A) ? "true" : "false") << "\n";

        std::cout << "\nLeftDiv(A, B):\n";
        PrintMatrix(LeftDiv(A, B));

        std::cout << "\nLeftDiv(A, v):\n";
        auto LeftDivVectorResult = LeftDiv(A, v);
        LeftDivVectorResult.Print();

        auto DivisionResult = A / 2.0;
        std::cout << "\nA / 2.0:\n";
        DivisionResult.Print();

        auto DivMatrix = A;
        DivMatrix /= B;
        std::cout << "\nA /= B:\n";
        DivMatrix.Print();

        std::cout << "\nDeterminanta A: " << A.Det() << "\n";
        
        std::cout << "Determinanta singularne matrice: " << SingularMatrix.Det() << "\n";

        auto InvertedA = Inverse(A);
        std::cout << "\nInverzna matrica A:\n";
        InvertedA.Print(10);

        auto RREFMatrix = A;
        RREFMatrix.ReduceToRREF();
        std::cout << "\nRREF A:\n";
        RREFMatrix.Print(10);
        std::cout << "\nRank A: " << A.Rank() << "\n";

        try {
            Matrix c{{1,2,3},{4,5,6}};
            Det(c);
        } catch(std::exception &e) {
            std::cout << e.what() <<", za pravougaonu matricu\n";
        }

        try {
            Inverse(SingularMatrix);
        } catch(std::exception &e) {
            std::cout << e.what() <<", za singularnu matricu\n";
        }

    } catch (std::exception &e) {
        std::cout << "Novi belaj: " << e.what() << "\n";
        throw;
    }
    std::cout << "\nSve ok, za nove metode matrice!\n";
}

void temeljitiTestLUDecomposer() {
    try {
        Matrix A{{4, 3}, {6, 3}};
        LUDecomposer lu(A);

        std::cout << "Originalna matrica A:\n";
        A.Print(10);

        auto CompactLU = lu.GetCompactLU();
        std::cout << "\nKompaktni LU:\n";
        CompactLU.Print(10);

        auto L = lu.GetL();
        auto U = lu.GetU();
        std::cout << "\nMatrica L:\n";
        L.Print(10);
        std::cout << "\nMatrica U:\n";
        U.Print(10);

        // Provjera A = L * U (sa permutacijom)
        auto ReconstructedA = L * U;
        std::cout << "\nRekonstruisana A (L * U):\n";
        ReconstructedA.Print(10);

        Vector b{5, 6};
        auto x = lu.Solve(b);
        std::cout << "\nRješenje za Ax = b:\n";
        x.Print(' ');

        auto Ax = A * x;
        std::cout << "\nRekonstruisan b (A * x):\n";
        Ax.Print(' ');
        std::cout << "\nRazlika: b - A * x: " << (b - Ax).Norm() << "\n";

        Matrix B{{7, 8}, {9, 10}};
        auto X = lu.Solve(B);
        std::cout << "\nRješenje za AX = B:\n";
        X.Print(10);

        auto AB = A * X;
        std::cout << "\nRekonstruisana B (A * X):\n";
        AB.Print(10);
        std::cout << "Razlika: B - A * X: " << (B - AB).Norm() << "\n";

        // Testiranje edgeova
        try {
            Matrix InvalidMatrix(2, 3); 
            LUDecomposer invalidLU(InvalidMatrix);
        } catch (std::exception &e) {
            std::cout << e.what() << ", za non-square matrix\n";
        }

        try {
            Matrix SingularMatrix{{1, 2}, {2, 4}}; 
            LUDecomposer singularLU(SingularMatrix);
        } catch (std::exception &e) {
            std::cout << e.what() << ", za singular matrix\n";
        }

    } catch (...) {
        std::cout << "Neki novi belaj";
        throw;
    }
}

void temeljitiTestQRDecomposer() {
    try {
        Matrix A{{12, -51, 4}, {6, 167, -68}, {-4, 24, -41}};
        QRDecomposer qr(A);

        auto Q = qr.GetQ();
        auto R = qr.GetR();

        std::cout << "Matrix Q:\n";
        Q.Print(10); 
        std::cout << "\nMatrix R:\n";
        R.Print(10);

        //potvrdi A = Q * R
        auto ReconstructedA = Q * R;
        std::cout << "\nRekonstruisana A (Q * R):\n";
        ReconstructedA.Print(10);
        std::cout << "Razlika: A - Q * R: " << (A - ReconstructedA).Norm() << "\n";

        Vector b{1, 2, 3};
        Vector x = qr.Solve(b);
        std::cout << "\nRjesenje Vector x (Ax = b):\n";
        x.Print(' ');

        auto Ax = A * x;
        std::cout << "\nRekonstruisan b (A * x):\n";
        Ax.Print(' ');

        std::cout << "Razlika: b - A * x: " << (b - Ax).Norm() << "\n";

        Matrix B{{1, 2},{3, 4},{5, 6}};
        auto X = qr.Solve(B);
        std::cout << "\nRjesenje Matrix X (AX = B):\n";
        X.Print(10);

        auto AB = A * X;
        std::cout << "\nRekonstruisana B (A * X):\n";
        AB.Print(10);

        std::cout << "Razlika B - A * X: "  << (B - AB).Norm() << "\n";

        //testiranje edgeva
        try {
            Matrix InvalidMatrix(2, 3); 
            QRDecomposer invalidQR(InvalidMatrix);
        } catch (const std::exception &e) {
            std::cout << e.what() << ", za non-square matrix\n";
        }

        try {
            Matrix SingularMatrix{{1, 2}, {2, 4}}; 
            QRDecomposer singularQR(SingularMatrix);
        } catch (const std::exception &e) {
            std::cout << e.what() << ", za singular matrix\n";
        }

    } catch (...) {
        std::cout << "Neki novi belaj";
        throw;
    }
    std::cout << "\nSve ok, za QRDecomposer!\n";
}

int main(){
    /* Vector v1{1,2,6,58};
    Vector v2{1,2,6,58};
    Vector v3{1,2,5};
    Vector v4{1,1,1,1};

    std::cout<<v1.EqualTo(v2)<<v1.EqualTo(v3)<<v1.EqualTo(v4); */

    /* Matrix matrix = {
        {12.5, 3.2, -6.4, 8.1, -9.2},
        {-3.1, 7.5, 5.6, -4.4, 2.3},
        {4.7, -8.6, 9.8, 1.5, -3.9},
        {6.3, -4.1, 2.9, 5.5, -1.8},
        {-7.2, 3.9, -5.3, 4.7, 8.8}
    };
    matrix.Print();
    Matrix c = Inverse(matrix);
    std::cout<<std::endl;
    PrintMatrix(c); */

    /* Matrix C{{2,3,5},{2,3,7},{4,1,8}}, jedinicna{{1,0,0},{0,1,0},{0,0,1}};
    auto u = C * Inverse(C);
    PrintMatrix(u);
    std::cout<<std::endl;
    jedinicna.Print();
    std::cout<<std::endl<<jedinicna.EqualTo(u); */

    /* Matrix a = {{1,2},{3,4}};
    std::cout<<Rank(a)<<std::endl;
    Matrix b ={{5,6},{7,8}};
    Matrix z = a/b;
    PrintMatrix(z);
    
    std::cout<<std::endl;
    a.Transpose();
    b.Transpose();
    x.Transpose();
    PrintMatrix(x); */

    Matrix a = {{1,2},{3,4}};
    Matrix b = {{5,6},{7,8}};
   /*  Matrix c = a/b;
    Matrix d = LeftDiv(a,b);
    c.Print();
    d.Print();  */

    LUDecomposer lu(a);
    auto z = lu.GetCompactLU();
    //z.Print();

   /*  Matrix A{{0,3,2},{4,6,1},{3,1,7}},jedinicna{{1,0,0},{0,1,0},{0,0,1}};
    LUDecomposer lol(A);
    lol.GetCompactLU().Print();
    std::cout<<std::endl;
    PrintMatrix(lol.GetU()+lol.GetL() - jedinicna); */

    Matrix A{{0,3,2},{4,6,1},{3,1,7}};
    QRDecomposer qr(A);
    PrintMatrix(qr.GetQ());
    std::cout<<std::endl;
    PrintMatrix(qr.GetR());
    Matrix B{{0,3},{4,6},{3,1}};
    QRDecomposer qr2(B);
    //std::cout<<B.EqualTo(qr2.GetQ() * qr2.GetR());;	

    QRDecomposer kuer(a);
    Matrix p = kuer.GetR();
    //p.Print();
    
    return 0;
}