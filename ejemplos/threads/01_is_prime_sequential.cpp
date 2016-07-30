/*
 * Ejemplo de un functor.
 * Se ejecutan varios functors en secuencia.
 *
 * Compilar con g++ -std=c++98 -pedantic 01_is_prime_sequential.cpp
 *
 * */

#include <iostream>
#define N 10

// Functor: una funcion hecha objeto
class IsPrime {
    private:
        unsigned int n;
        bool &result;
    public:
        IsPrime(unsigned int n, bool &result) : n(n), result(result) {}
        void run() {
            for (unsigned int i = 2; i < n; ++i) {
                if (n % i == 0) {
                    result = false;
                    return;
                }
            }

            result = true;
        }
};



int main() {
    unsigned int nums[N] = {0, 1, 2, 3, 4, 5, 13, 132130891, 132130891, 132130871};
    bool results[N]; 

    for (int i = 0; i < N; ++i) {
        // Creamos un functor (function object) con los argumentos de
        // la funcion pero esta no se invoca aqui
        IsPrime is_prime = IsPrime(nums[i], results[i]);

        // Recien aqui se invoca a la funcion "is prime".
        // Los functors permiten retrasar las llamadas a funciones
        is_prime.run();
    }


    for (int i = 0; i < N; ++i) {
        std::cout << results[i] << " ";
    }
    std::cout << "\n";

    return 0;
}


