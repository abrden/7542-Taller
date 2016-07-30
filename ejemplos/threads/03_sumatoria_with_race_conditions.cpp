/*
 * Ejemplo de una race condition: accesos de lecto/escritura a una misma variable u objeto
 * por multiples hilos que terminan dejando a la variable/objeto en un estado inconsistente
 *
 * Compilar con g++ -std=c++98 -pedantic 03_sumatoria_with_race_conditions.cpp -lpthread
 *
 * El ejemplo deberia imprimir por pantalla el numero 479340.
 * Como siempre se suman los mismos numeros el resultado final 479340 deberia ser
 * siempre el mismo pero debido a la race condition, el resultado puede variar.
 *
 * Para tratar de ver el bug (puede ser dificil de triggerearlo), correr esto:
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

class Sum: public Thread {
    private:
        unsigned int *start;
        unsigned int *end;
        
        unsigned int &result;

    public:
        Sum(unsigned int *start, unsigned int *end, unsigned int &result) :
            start(start), end(end), result(result) {}

        virtual void run() {
            unsigned int temporal_sum = 0;
            for (unsigned int *p = start; p < end; ++p) {
                temporal_sum += *p;
            }

            result += temporal_sum; // aca esta la race condition:
                                    // multiples instancias del functor Sum corriendo
                                    // cada uno el metodo run en threads en paralelo
                                    // terminaran escribiendo a la **variable compartida**
                                    // "result". Es esta linea la critical section
                                    // que habria que proteger. 
        }
};

int main() {
    unsigned int nums[N] = { 132131, 1321, 31371, 30891, 891, 123891, 3171, 30891, 891, 123891};
    unsigned int result = 0; 
 
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
    
    std::cout << result << " ";     // 479340
    std::cout << "\n";

    return 0;
}
