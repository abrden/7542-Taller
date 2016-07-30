/* 
 * Implemente un programa C que reciba por línea de comandos 3 parámetros:
 * IP, PUERTO y ARCHIVO; y que transmita el archivo binario <ARCHIVO> 
 * al puerto <PUERTO> de la dirección <IP>, utilizando protocolo TCP/IP.
 * No valide errores.
 */

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_LEN 10

typedef struct _socket {
	int fd;
} socket_t;

void client_init(socket_t* client, const char* hostname, const char* port) {
	struct addrinfo hints, *ptr;
	
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
	hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
	hints.ai_flags = 0;     /* AI_PASSIVE for server           */
	
	getaddrinfo(hostname, port, &hints, &ptr);
	
	client->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	
	connect(client->fd, ptr->ai_addr, ptr->ai_addrlen);
	
	freeaddrinfo(ptr);
}

void file_transmit(socket_t* client, const char* file_name) {
	char buffer[BUFFER_LEN + 1];
	memset(buffer, '\0', BUFFER_LEN);
	
	FILE* fp = fopen(file_name, "rb");
	
	for (!feof(fp)) {
		fread(buffer, sizeof(char), BUFFER_LEN, fp);
		send(client->fd, buffer, BUFFER_LEN, MSG_NOSIGNAL);
		memset(buffer, '\0', BUFFER_LEN);
	}
	
	fclose(fp);
}

void client_destroy(socket_t* client) {
	shutdown(client->fd, SHUT_RDWR);
	close(client->fd);
}

int main(int argc, char* argv[]) {
	socket_t client;
	client_init(&client, argv[1], argv[2]);
	
	file_transmit(&client, argv[3]);
	
	client_destroy(&client);
	return 0;
}
