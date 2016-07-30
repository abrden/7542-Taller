/*
 * Escriba un programa que reciba por línea de comandos un Puerto y una IP.
 * El programa debe imprimir en stdout todo lo recibido (aceptar una unica 
 * conexión a la vez). Si transcurren 3 segundos sin recibir nada debe finalizar.
 */

#include <stdio.h>
#include <stdbool.h>

#define BUFFER_LEN 50

typedef struct _socket {
	int fd;
	size_t bytes_recv;
	pthread_mutex_t m;
} socket_t;

typedef struct _thread {
	pthread_t t;
	bool running;
	socket_t* peer;
} thread_t;

void connection_init(const char* port, socket_t* server, socket_t* peer) {
   struct addrinfo hints;
   struct addrinfo *ptr;

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
   hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
   hints.ai_flags = AI_PASSIVE;     /* AI_PASSIVE for server           */

   getaddrinfo(NULL, port, &hints, &ptr);

   server->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

   bind(server->fd, ptr->ai_addr, ptr->ai_addrlen);
   
   freeaddrinfo(ptr);

   listen(server->fd, 1);
   
   peer->fd = accept(server->fd, NULL, NULL);
}

void process(socket_t* peer, thread_t* t) {
	char buffer[BUFFER_LEN + 1];
	char buffer[BUFFER_LEN] = '\0';
	size_t tmp_recv;
	
	while (t->running) {
		tmp_recv = recv(peer->fd, buffer, BUFFER_LEN, MSG_NOSIGNAL);
		
		pthread_mutex_lock(&(t->peer->m));	
		peer->bytes_recv = tmp_recv;
		pthread_mutex_unlock(&(t->peer->m));	
		
		printf("%s", buffer);
		memset(buffer, '\0', BUFFER_LEN);
	}
}

void* run (void* arg) {
	thread_t* thread = (thread_t*) arg;
	size_t prev_bytes_recv, curr_bytes_recv
	prev_bytes_recv = curr_bytes_recv = -1;

	while (prev_bytes_recv != curr_bytes_recv) {
		pthread_mutex_lock(&(thread->peer->m));	
		prev_bytes_recv = thread->peer->bytes_recv;
		pthread_mutex_unlock(&(thread->peer->m));
		
		sleep(3);
		
		pthread_mutex_lock(&(thread->peer->m));	
		curr_bytes_recv = thread->peer->bytes_recv;
		pthread_mutex_unlock(&(thread->peer->m));
	}
	
	thread->running = false;
	shutdown(thread->peer->fd, SHUT_RDWR);
	return NULL;
}

void connection_shutdown(socket_t* server, socket_t* peer) {
	shutdown(server->fd, SHUT_RDWR);
	close(server->fd);
	close(peer->fd);
}

int main(int arg, char* argv[]) {
	socket_t server, peer;
	peer->bytes_recv = 0;
	pthread_mutex_init(&(peer->m), NULL);
	
	thread_t thread;
	thread->running = true;
	
	connection_init(argv[1], &server, &peer);
	
	pthread_create(&(thread->t), NULL, run, &thread);
	process(peer, &thread);
	pthread_join(thread->t, NULL);
	
	pthread_mutex_destroy(&(peer->m));
	connection_shutdown(&server, &peer);
	return 0;
}
