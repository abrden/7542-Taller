/*
 * Ejemplo de Mutex
 *
 * Compilar con g++ -std=c++98 -pedantic 04_sumatoria_with_mutex.cpp -lpthread
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
            // Tomamos (adquirimos) el mutex. 
            // Cualquier otro hilo (incluido el nuestro)  que quiera 
            // tomar este mutex se bloqueara hasta que nosotros llamemos
            // a unlock.
            pthread_mutex_lock(&this->mutex);
        }

        void unlock() {
            // Liberamos el mutex para que otros hilos lo puedan tomar.
            pthread_mutex_unlock(&this->mutex);
        }
        
        ~Mutex() {
            pthread_mutex_destroy(&this->mutex);
        }

    private:
        // No tiene sentido copiar mutexs, asi que forzamos a que no se pueda.
        Mutex(const Mutex&);
        Mutex& operator=(const Mutex&);
};

class Sum: public Thread {
    private:
        unsigned int *start;
        unsigned int *end;
        
        unsigned int &result;
        Mutex &m; // Referencia a un mutex: nada de andar copiando un mutex

    public:
        Sum(unsigned int *start, unsigned int *end, unsigned int &result, Mutex &m) :
            start(start), end(end), result(result), m(m) {}

        virtual void run() {
            unsigned int temporal_sum = 0;
            for (unsigned int *p = start; p < end; ++p) {
                temporal_sum += *p;
            }

            m.lock();               // a partir de aqui un hilo y solo un hilo puede
            result += temporal_sum; // pasar a esta seccion critica,
            m.unlock();             // seccion que termina en esta linea
        }
};

int main() {
    unsigned int nums[N] = { 132131, 1321, 31371, 30891, 891, 123891, 3171, 30891, 891, 123891};
    unsigned int result = 0;
    Mutex m; // Un unico mutex: NO un mutex por hilo
 
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

