/*
 * Escriba un pequeño programa que implemente un servidor UDP. 
 * El mismo debe recibir paquetes de datos de 8 bytes e imprimirlos en 
 * formato Hexadecimal (un pquete por renglón). El servidor debe cerrarse 
 * al recibir un paquete de 8 bytes 00h.
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

#define PORT "8080"
#define HOSTNAME "localhost"
#define BUFFER_LEN 8

typedef struct _server {
	int fd;
} server_t;

void server_init(server_t* server, struct addrinfo* hints, struct addrinfo* ptr) {
	memset(hints, 0, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
	hints->ai_socktype = SOCK_DGRAM;  /* UDP 							 */
	hints->ai_flags = AI_PASSIVE;     /* AI_PASSIVE for server           */

	getaddrinfo(HOSTNAME, PORT, hints, &ptr);

	server->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	bind(server->fd, ptr->ai_addr, ptr->ai_addrlen);

	listen(server->fd, 20);
}

bool null_buffer(const char* buffer) {
	for (int i = 0; i < BUFFER_LEN; i++)
		if (buffer[i] != '\0') return false;
	return true;
}

void process(server_t* server) {
	char buffer[BUFFER_LEN + 1];
	long int hexa_buffer;
	
	do {
		memset(buffer, '\0', BUFFER_LEN);
		recvfrom(server->fd, buffer, BUFFER_LEN, MSG_NOSIGNAL, NULL, NULL);
		// No me importa quien me mando el mensaje, no me piden contestar
		hexa_buffer = strtol(buffer, NULL, 16);
		printf("%lu\n", hexa_buffer);
	} while (!null_buffer(buffer));
}

void server_shutdown(server_t* server, struct addrinfo* ptr) {
	close(server->fd);
	freeaddrinfo(ptr);
}

int main(int arg, char* argv[]) {
	struct addrinfo hints;
	struct addrinfo *ptr;
	server_t server;
	server_init(&server, &hints, ptr);

	process(&server);
	
	server_shutdown(&server, ptr);
	return 0;
}

