#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <deque>
#include <stack>

//OUTPUT DRUGOG ZADATKA
/* 
4 4 6 1 5 4 
3 4 4 5 5 
15 45 60 30 75 
75 225 300 150 375 
13 20 13
30 22 30 
*/

//TRECI ZADATAK
template <typename Iterator1, typename Iterator2>
Iterator1 find_subblock(Iterator1 first1, Iterator1 last1, Iterator2 first2, Iterator2 last2) {
    if (first2 == last2) return first1;
    
    Iterator1 result = last1;
    Iterator2 it2 = first2;
    
    for (Iterator1 it1 = first1; it1 != last1; ++it1) {
        if (*it1 == *it2) {
            if (it2 == first2) result = it1;
            ++it2;
            if (it2 == last2) return result;
        } else {
            it2 = first2;
            result = last1;
        }
    }
    
    return last1;
}

//CETVRTI ZADATAK
bool is_idempotent(const std::vector<std::vector<double>>& matrix) {
    // Check if it's a square matrix
    size_t n = matrix.size();
    if (n == 0) return false;
    
    for (const auto& row : matrix) {
        if (row.size() != n) return false;
    }
    
    // Check A² = A
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < n; ++k) {
                sum += matrix[i][k] * matrix[k][j];
            }
            
            if (std::abs(sum - matrix[i][j]) > 1e-9) {
                return false;
            }
        }
    }
    
    return true;
}

//PETI ZADATAK
auto Polinom(const std::vector<double>& coefficients) {
    return [coefficients](double x) {
        double result = 0.0;
        double power = 1.0; 
        
        for (double coef : coefficients) {
            result += coef * power;
            power *= x;
        }
        
        return result;
    };
}

//SESTI ZADATAK
template <typename Container1, typename Container2>
    auto kronecker_product(const Container1& a, const Container2& b) {

        using ResultType = decltype(a[0] * b[0]);
        
        size_t m = a.size();
        size_t n = b.size();
        
        ResultType** matrix = nullptr;
        try {
            matrix = new ResultType*[m];
            matrix[0] = new ResultType[m * n];
            
            for (size_t i = 1; i < m; ++i) {
                matrix[i] = matrix[0] + i * n;
            }
            
            for (size_t i = 0; i < m; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    matrix[i][j] = a[i] * b[j];
                }
            }
            
            return matrix;

        } catch (const std::bad_alloc&) {
            if (matrix) {
                delete[] matrix[0];
                delete[] matrix;
            }
            throw std::logic_error("Alokacija nije uspjela");
        }
    }

//SEDMI ZADATAK
std::stack<double> multiply_stack(std::stack<double> s, double multiplier) {

    // Reverse the stack using another temporary stack
    std::stack<double> reversed;
    while (!s.empty()) {
        reversed.push(s.top());
        s.pop();
    }
    
    // Multiply and push to result
    while (!reversed.empty()) {
        s.push(reversed.top() * multiplier);
        reversed.pop();
    }
    
    return s;
}

//OSMI ZADATAK
int nadjiMedijan(const std::deque<int>& dek) {
    int n = dek.size();
    for (int i = 0; i < n; i++) {
        int manje_jednako = 0, vece_jednako = 0;
        for (int j = 0; j < n; j++) {
            if (dek[j] <= dek[i]) manje_jednako++;
            if (dek[j] >= dek[i]) vece_jednako++;
        }
        if (manje_jednako >= (n / 2 + 1) && vece_jednako >= (n / 2 + 1))
            return dek[i];
    }
    return -1; 
}
// U ovom slucaju ce vratiti -1

//MAINOVI
int main() {

    //MAIN ZA TRECI ZADATAK
    std::string arr1[] = {"apple", "banana", "cherry", "date", "elderberry"};
    std::string arr2[] = {"cherry", "date"};
    
    auto it = find_subblock(std::begin(arr1), std::end(arr1), std::begin(arr2), std::end(arr2));
    
    if (it != std::end(arr1)) {
        std::cout << "Found at position: " << std::distance(std::begin(arr1), it) << "\n";
    } else {
        std::cout << "Not found\n";
    }

    //MAIN ZA CETVRTI ZADATAK
    std::vector<std::vector<double>> m{{1,0},{0,1}};
    std::cout<<is_idempotent(m);

    //MAIN ZA PETI ZADATAK
    auto poli = Polinom({2, 3, 0, -1, 4});
    std::cout << poli(5); 

    //MAIN ZA SESTI ZADATAK
    try {
        std::vector<int> a = {1, 2, 3};
        std::deque<int> b = {4, 5};
        
        auto product = kronecker_product(a, b);
        
        for (size_t i = 0; i < 3; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                std::cout << product[i][j] << " ";
            }
            std::cout << "\n";
        }
        
        // Clean up
        delete[] product[0];
        delete[] product;

    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    return 0;
}