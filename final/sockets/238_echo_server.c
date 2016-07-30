/*
 * Escriba un programa que acepte conexiones por el puerto 283. 
 * En cada conexión abierta debe esperar paquetes de datos de 513 bytes 
 * y devolverlos al emisor. El programa debe terminar ordenadamente al 
 * recibir algún paquete de 513 letras 'Q'.
 */

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#endif

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

#define PORT "283"
#define BUFFER_LEN 513
#define CONNECTION_MAX 30

typedef struct _server {
	int fd;
	pthread_mutex_t m;
	bool running;
} server_t;

typedef struct _connection {
	int peer_fd;
	pthread_t thread;
	server_t* server;
} connection_t;

void server_init(const char* port, server_t* server) {
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

	listen(server->fd, 20);
	
	pthread_mutex_init(&(server->m), NULL);
	server->running = true;
}

bool server_running(server_t* server) {
	bool ans;
	pthread_mutex_lock(&(server->m));
	ans = server->running;
	pthread_mutex_unlock(&(server->m));
	return ans;
}

void server_halt(server_t* server) {
	pthread_mutex_lock(&(server->m));
	server->running = false;
	pthread_mutex_unlock(&(server->m));
}

bool eoc(char* buffer) {
	for (int i = 0; i < BUFFER_LEN; i++)
		if (buffer[i] != 'Q') return false;
	return true;
}

void* run(void* arg) {
	connection_t* connection = (connection_t*) arg;
	char buffer[BUFFER_LEN + 1];
	memset(buffer, '\0', BUFFER_LEN);
	while (server_running(connection->server) && eoc(buffer)) {
		recv(connection->peer_fd, buffer, BUFFER_LEN, MSG_NOSIGNAL);
		send(connection->peer_fd, buffer, BUFFER_LEN, MSG_NOSIGNAL);
	}
	return NULL;
}

void accept_connections(server_t* server) {
	connection_t* connections = malloc(sizeof(connection_t) * CONNECTION_MAX);
	int i;
	for (i = 0; server_running(server) && i < CONNECTION_MAX; i++) {
		connections[i].peer_fd = accept(server->fd, NULL, NULL);
		connections[i].server = server;
		pthread_create(&(connections[i].thread), NULL, run, &(connections[i]));
	}
	
	for (int j = 0; j < i; j++) {
		shutdown(connections[j].peer_fd, SHUT_RDWR);
		close(connections[j].peer_fd);
		pthread_join(connections[j].thread, NULL);
	}
	
	free(connections);
}

void server_shutdown(server_t* server) {
	shutdown(server->fd, SHUT_RDWR);
	close(server->fd);
	pthread_mutex_destroy(&(server->m));
}

int main(int arg, char* argv[]) {
	server_t server;
	server_init(PORT, &server);

	accept_connections(&server);
	
	server_shutdown(&server);
	return 0;
}
