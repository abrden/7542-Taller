/*
 * Ejemplo de encapsulamiento de un objeto compartido y su mutex en un objeto protegido (monitor)
 *
 * Compilar con g++ -std=c++98 -pedantic 06_sumatoria_with_monitor.cpp -lpthread
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
            m.lock();
        }

        ~Lock() {
            m.unlock();
        }

    private:
        Lock(const Lock&);
        Lock& operator=(const Lock&);
};

class ResultProtected {
    private:
        Mutex m;
        unsigned int result;

    public:
        ResultProtected(): result(0) {}

        void inc(unsigned int s) {
            Lock l(m);      // Este metodo esta protegido. Solo un hilo podra
            result += s;    // acceder a este metodo.
        }

        unsigned int get_val() const {  // Por diseño este metodo no esta protegido
            return result;              // No vale la pena proteger lo que no esta
        }                               // compartido, pero hay que saber bien que
                                        // y que no se comparte.
};

class Sum : public Thread {
    private:
        unsigned int *start;
        unsigned int *end;
        
        ResultProtected &result;        

    public:
        Sum(unsigned int *start, unsigned int *end, ResultProtected &result) :
            start(start), end(end), result(result) {}

        virtual void run() {
            unsigned int temporal_sum = 0;
            for (unsigned int *p = start; p < end; ++p) {
                temporal_sum += *p;
            }

            result.inc(temporal_sum); // No nos encargamos de proteger el recurso 
                                      // compartido (unsigned int result) sino que el
                                      // objeto protegido (monitor) de tipo ResultProtected sera
                                      // el responsable de protegerlo.
        }
};

int main() {
    unsigned int nums[N] = { 132131, 1321, 31371, 30891, 891, 123891, 3171, 30891, 891, 123891};
    ResultProtected result;
 
    std::vector<Thread*> threads;

    for (int i = 0; i < N/2; ++i) {
        threads.push_back(new Sum(&nums[i*2], &nums[(i+1)*2], result));
    }

    for (int i = 0; i < N/2; ++i) {
        threads[i]->start();
    }

    for (int i = 0; i < N/2; ++i) {
        threads[i]->join();
        delete threads[i];
    }
    
    std::cout << result.get_val() << " ";     // 479340
    std::cout << "\n";

    return 0;
}

