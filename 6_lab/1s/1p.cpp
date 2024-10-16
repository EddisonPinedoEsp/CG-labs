#include <iostream>
#include <vector>

using namespace std;

// Función para calcular combinaciones
unsigned long long comb(int n, int k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    vector<unsigned long long> C(k + 1, 0);
    C[0] = 1;
    for (int i = 1; i <= n; ++i) {
        for (int j = min(i, k); j > 0; --j) {
            C[j] = C[j] + C[j - 1];
        }
    }
    return C[k];
}

unsigned long long number_of_triangulations(int N) {
    // Caso base: Un polígono con menos de 3 lados no puede ser triangulado
    if (N < 3) {
        return 0;
    }
    
    // Calcular el número de Catalan para N-2
    int n = N - 2;
    unsigned long long catalan_number = comb(2 * n, n) / (n + 1);
    
    return catalan_number;
}

int main() {
    // Ejemplos de uso:
    cout << number_of_triangulations(3) << endl;  // Output: 5
    cout << number_of_triangulations(4) << endl;  // Output: 14
    cout << number_of_triangulations(5) << endl;  // Output: 14
    return 0;
}