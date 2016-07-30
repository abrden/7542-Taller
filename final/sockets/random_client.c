/* 
 * Implemente un programa que reciba por línea de comandos una dirección
 * IP y un puerto; y transmita 50 paquetes de 10 bytes aleatorios a esa
 * dirección/puerto antes de terminar ordenadamente.
 */

#include <stdio.h>
#include <stdlib.h>

#define PACKETS_QTTY 50
#define PACKET_LEN 10

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

void send_random_packets(socket_t* client) {
	char buffer[PACKET_LEN + 1];
	buffer[PACKET_LEN] = '\0';
	
	for (int i = 0; i < PACKETS_QTTY; i++) {
		for (int j = 0; j < PACKET_LEN; j++)
			buffer[j] = 'a' + rand() % 26;
		send(client->fd, buffer, PACKET_LEN, MSG_NOSIGNAL);
	}
}

void client_destroy(socket_t* client) {
	shutdown(client->fd, SHUT_RDWR);
	close(client->fd);
}

int main(int argc, char* argv[]) {
	socket_t client;
	client_init(&client, argv[1], argv[2]);
	
	send_random_packets(&client);
	
	client_destroy(&client);
	return 0;
}
