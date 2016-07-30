/*
 * Ejemplo de como wrappear los threads (pthread) de C en C++
 * con clases y polimorfismo.
 *
 * Se ejecutan varios functors en paralelo.
 *
 * Compilar con g++ -std=c++98 -pedantic 02_is_prime_parallel.cpp -lpthread
 *
 * */

#include <iostream>
#include <vector>
#include <pthread.h>

#define N 10

class Thread {
    private:
        pthread_t thread;
 
        // pthread necesita una funcion como uno de sus argumentos.
        // por ello runner es una funcion (en C++ un metodo static se 
        // lo llama metodo de clase, pero no son distintas a las funciones
        // convencionales de C, de hecho no tienen un this)       
        static void *runner(void *data) {
            Thread *self = (Thread*)data;
            self->run(); // aca esta el truco: como run es virtual
                         // el codigo a ejecutar se resuelve por
                         // polimorfismo. 
                         // Objetos distintos podran correr en sus 
                         // propios threads con esta unica implementacion 
                         // de Thread mientras hereden de Thread y creen
                         // sus propias versiones del metodo run.
        }

    public:
        Thread () {}

        void start() {
            // Lanzamos el thread que correra siempre la misma funcion runner
            // Para que esta clase pueda ejecutar distintos codigos pasamos como
            // argumento this y por polimorfismo se le llamara a run. Es en el metodo
            // run en que cada objeto distinto podra implementar su codigo particular
            // a correr en el thread.
            pthread_create(&thread, NULL, Thread::runner, this);
        }

        void join() {
            pthread_join(thread, NULL);
        }

        virtual void run() = 0; // Virtual puro para forzar una definicion en las clases hijas
        virtual ~Thread() {} // Destructor virtual: siempre hacerlo virtual si pensamos en usar herencia.

    private:
        // No tiene sentido copiar hilos, asi que forzamos a que no se puedan copiar
        Thread(const Thread&);
        Thread& operator=(const Thread&);
};


class IsPrime : public Thread { // Ahora IsPrime puede correr un un thread separado del main.
    private:
        unsigned int n;
        bool &result;
    public:
        IsPrime(unsigned int n, bool &result) : n(n), result(result) {}
        virtual void run() {
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
 
    std::vector<Thread*> threads;

    for (int i = 0; i < N; ++i) {
        // Aca esta la ventaja de usar functors:
        //  - creamos los functors con sus argumentos en el main
        //  - pero la funcion se ejecutara en el hilo lanzado por el metodo start.
        threads.push_back(new IsPrime(nums[i], results[i]));
        threads[i]->start();
    }

    /* ************************************** */
    /* Ahora: Todos los hilos estan corriendo */
    /* ************************************** */

    // Esperamos a que cada hilo termine.
    // Cada join bloqueara al hilo llamante (main) hasta que el hilo
    // sobre el cual se le hace join (threads[i]) termine
    for (int i = 0; i < N; ++i) {
        threads[i]->join();
        delete threads[i];
    }

    /* ********************************************************************** */
    /* Ahora: Todos los hilos terminaron y sus recursos limpiados con el join */
    /* ********************************************************************** */

    for (int i = 0; i < N; ++i) {
        std::cout << results[i] << " ";
    }
    std::cout << "\n";

    return 0;
}


