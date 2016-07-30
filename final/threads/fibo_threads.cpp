/*
 * Escriba un ejemplo en el cual un thread genere los nros. de la serie 
 * de fibonacci a razón de 1 cada 2 segundos; y otro thread del mismo 
 * proceso los imprima al mismo ritmo. Ambos threads deben coordinar el 
 * acceso a la memoria y deben correr sincronizados de forma que la serie
 * se imprima correctamente.
 */

#include <iostream>
#include <pthread.h>
#include <unistd.h>

#define LOOPS 100

class Thread {
	private:
		pthread_t thread;
		
		static void* runner(void* arg) {
			((Thread*)arg)->run();
			return NULL;
		}
		
	public:
		void start() {
			pthread_create(&(this->thread), NULL, Thread::runner, this);
		}
		void join() {
			pthread_join(this->thread, NULL);
		}
		virtual void run() = 0;
		virtual ~Thread() {}
};

class Mutex {
	private:
		pthread_mutex_t mutex;
	public:
		Mutex() { pthread_mutex_init(&(this->mutex), NULL); }
		void lock() { pthread_mutex_lock(&(this->mutex)); }
		void unlock() { pthread_mutex_unlock(&(this->mutex)); }
		~Mutex() { pthread_mutex_destroy(&(this->mutex)); }
};

class Lock {
	private:
		Mutex& mutex;
	public:
		Lock(Mutex& mutex) : mutex(mutex) { this->mutex.lock(); }
		~Lock() { this->mutex.unlock(); }
};

class ResultProtected {
	private:
		Mutex m;
		int result;
	public:
		ResultProtected() : result(-1) {}
		int get() {
			Lock l(this->m);
			return this->result;
		}
		int set(int new_result) {
			Lock l(this->m);
			this->result = new_result;
		}
		~ResultProtected() {}
};

int fibo(int n) {
	if (n == 0 || n == 1) return n;
	return fibo(n - 1) + fibo(n - 2);
}

class Thinker : public Thread {
	private:
		ResultProtected& result;
		int fibo_order;
	public:
		Thinker(ResultProtected& result) : result(result), fibo_order(0) {}
		void run() {
			for (int i = 0; i < LOOPS; ++i) {
				this->result.set(fibo(this->fibo_order));
				this->fibo_order++;
				sleep(2);
			}
		}
		~Thinker() {}
};

class Printer : public Thread {
	private:
		ResultProtected& result;
	public:
		Printer(ResultProtected& result) : result(result) {}
		void run() {
			for (int i = 0; i < LOOPS; ++i) {
				std::cout << this->result.get() << std::endl;
				sleep(2);
			}
		}
		~Printer() {}
};

int main(void) {
	ResultProtected r;
	Thinker t(r);
	Printer p(r);
	
	t.start();
	p.start();
	
	t.join();
	p.join();
	
	return 0;
}
