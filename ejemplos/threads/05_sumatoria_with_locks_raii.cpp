/*
 * Ejemplo de RAII encapsulando la toma y liberacion de un mutex: clase Lock
 *
 * Compilar con g++ -std=c++98 -pedantic 05_sumatoria_with_locks_raii.cpp -lpthread
 *
 * El ejemplo deberia imprimir por pantalla el numero 479340.
 *
 * Para verificar que efectivamente no hay una race condition, correr esto:
     for i in {0..1000}; do ./a.out; done | uniq
 *
 * */

#include <iostream>
#include <vector>
#include <pthread.h>

#define N 10

class Thread {
    private:
        pthread_t thread;
 
        static void *runner(void *data) {
            Thread *self = (Thread*)data;
            self->run();
        }

    public:
        Thread () {}

        void start() {
            pthread_create(&thread, NULL, Thread::runner, this);
        }

        void join() {
            pthread_join(thread, NULL);
        }

        virtual void run() = 0;
        virtual ~Thread() {}
    
    private:
        Thread(const Thread&);
        Thread& operator=(const Thread&);
};

class Mutex {
    private:
	pthread_mutex_t mutex;

    public:
        Mutex() {
            pthread_mutex_init(&this->mutex, NULL);	
        }

        void lock() {
            pthread_mutex_lock(&this->mutex);
        }

        void unlock() {
            pthread_mutex_unlock(&this->mutex);
        }
        
        ~Mutex() {
            pthread_mutex_destroy(&this->mutex);
        }

    private:
        Mutex(const Mutex&);
        Mutex& operator=(const Mutex&);
};

class Lock {
    private:
        Mutex &m;

    public:
        Lock(Mutex &m) : m(m) {
            m.lock();   // En el constructor adquirimos el recurso: lockeamos el mutex
        }

        ~Lock() {
            m.unlock(); // En el destructor liberamos el recurso: deslockeamos el mutex
        }

    private:
        // No tiene sentido copiar locks, forzar a que no se pueda.
        Lock(const Lock&);
        Lock& operator=(const Lock&);
};

class Sum: public Thread {
    private:
        unsigned int *start;
        unsigned int *end;
        
        unsigned int &result;
        Mutex &m; 

    public:
        Sum(unsigned int *start, unsigned int *end, unsigned int &result, Mutex &m) :
            start(start), end(end), result(result), m(m) {}

        virtual void run() {
            unsigned int temporal_sum = 0;
            for (unsigned int *p = start; p < end; ++p) {
                temporal_sum += *p;
            }

            Lock l(m);              // a partir de aqui el mutex es tomado 
            result += temporal_sum; 
                                    // y es liberado aqui cuando la variable "l"
                                    // es destruida por irse de scope.
                                    // Liberacion del mutex automatica!!
        }
};

int main() {
    unsigned int nums[N] = { 132131, 1321, 31371, 30891, 891, 123891, 3171, 30891, 891, 123891};
    unsigned int result = 0;
    Mutex m;
 
    std::vector<Thread*> threads;

    for (int i = 0; i < N/2; ++i) {
        threads.push_back(new Sum(&nums[i*2], &nums[(i+1)*2], result, m));
    }

    for (int i = 0; i < N/2; ++i) {
        threads[i]->start();
    }

    for (int i = 0; i < N/2; ++i) {
        threads[i]->join();
        delete threads[i];
    }
    
    std::cout << result << " ";     // 479340
    std::cout << "\n";

    return 0;
}

