#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

template <typename Tip>
void bubble_sort(Tip* niz, int vel) {
    bool swapped;
    for (int i = 0; i < vel - 1; ++i) {
        swapped = false;
        for (int j = 0; j < vel - i - 1; ++j) {
            if (niz[j] > niz[j + 1]) {
                swap(niz[j], niz[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

template <typename Tip>
void selection_sort(Tip* niz, int vel) {
    for (int i = 0; i < vel - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < vel; ++j) {
            if (niz[j] < niz[min_idx]) {
                min_idx = j;
            }
        }
        swap(niz[i], niz[min_idx]);
    }
}

template <typename Tip>
int partition(Tip* niz, int low, int high) {
    Tip pivot = niz[high];                      //ovaj put je uzet zadnji clan kao pivot, cisto radi varijacije 
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (niz[j] < pivot) {
            ++i;
            swap(niz[i], niz[j]);
        }
    }
    swap(niz[i + 1], niz[high]);
    return i + 1;
}

template <typename Tip>
void quick_sort_recursive(Tip* niz, int low, int high) {
    if (low < high) {
        int pi = partition(niz, low, high);
        quick_sort_recursive(niz, low, pi - 1);
        quick_sort_recursive(niz, pi + 1, high);
    }
}

template <typename Tip>
void quick_sort(Tip* niz, int vel) {
    quick_sort_recursive(niz, 0, vel - 1);
}

template <typename Tip>
void merge(Tip* niz, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Tip* L = new Tip[n1];
    Tip* R = new Tip[n2];

    for (int i = 0; i < n1; ++i)
        L[i] = niz[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = niz[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            niz[k] = L[i];
            ++i;
        } else {
            niz[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        niz[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        niz[k] = R[j];
        ++j;
        ++k;
    }

    delete[] L;
    delete[] R;
}

template <typename Tip>
void merge_sort_recursive(Tip* niz, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_recursive(niz, left, mid);
        merge_sort_recursive(niz, mid + 1, right);
        merge(niz, left, mid, right);
    }
}

template <typename Tip>
void merge_sort(Tip* niz, int vel) {
    merge_sort_recursive(niz, 0, vel - 1);
}

void ucitaj(string filename, int*& niz, int& vel) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Problem pri otvaranju datoteke" << endl;
        return;
    }

    vector<int> temp;
    int broj;
    while (file >> broj) {
        temp.push_back(broj);
    }
    
    vel = temp.size();
    niz = new int[vel];
    for (int i = 0; i < vel; ++i) {
        niz[i] = temp[i];
    }
    file.close();
}

void generisi(string filename, int vel) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Problem sa datotekom" << endl;
        return;
    }

    srand(time(0));
    for (int i = 0; i < vel; ++i) {
        file << rand() % 1000 << " "; 
    }
    file.close();
}

void ispis_niza(int* niz, int vel) {
    for (int i = 0; i < vel; ++i) {
        cout << niz[i] << " ";
    }
    cout << endl;
}

int main() {
    int* niz = nullptr;
    int vel = 0;

    string filename = "input.txt";
    string output_filename = "output.txt";

    int n;
    cout << "Unesite broj elemenata za generisanje: ";
    cin >> n;
    generisi(filename, n);

    ucitaj(filename, niz, vel);

    cout << "Učitani niz: ";
    ispis_niza(niz, vel);

    cout << "Izaberite algoritam sortiranja:\n";
    cout << "1. Bubble Sort\n";
    cout << "2. Selection Sort\n";
    cout << "3. Quick Sort\n";
    cout << "4. Merge Sort\n";
    int izbor;
    cin >> izbor;

    clock_t start = clock();
    switch (izbor) {
        case 1:
            bubble_sort(niz, vel);
            break;
        case 2:
            selection_sort(niz, vel);
            break;
        case 3:
            quick_sort(niz, vel);
            break;
        case 4:
            merge_sort(niz, vel);
            break;
        default:
            cout << "Pogrešan izbor!" << endl;
            delete[] niz;
            return 1;
    }
    clock_t end = clock();

    cout << "Sortirani niz: ";
    ispis_niza(niz, vel);

    bool jelSortiran = true;
    for (int i = 1; i < vel; ++i) {
        if (niz[i] < niz[i - 1]) {
            jelSortiran = false;
            break;
        }
    }

    if (jelSortiran) {
        cout << "Niz je ispravno sortiran." << endl;
    } else {
        cout << "Greška: Niz nije ispravno sortiran." << endl;
    }

    ofstream file(output_filename);
    for (int i = 0; i < vel; ++i) {
        file << niz[i] << " ";
    }
    file.close();

    cout << "Vrijeme izvršavanja: " << (end - start) * 1000.0 / CLOCKS_PER_SEC << " ms" << endl;

    delete[] niz;
    return 0;
}