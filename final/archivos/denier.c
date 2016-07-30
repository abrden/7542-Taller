/*
 * Escriba un pequeño programa que, sin validar errores, acepte una 
 * única conexión TCP en el puerto 1972 y reciba paquetes de no mas de 
 * 600 bytes, terminado en '0x00'. Cada paquete recibido debe ser 
 * devuelto con todos sus bits negados. Terminar al recibir un paquete nulo.
 */

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#endif

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define PORT "1972"
#define CLIENT_QTTY 1
#define PACKET_SIZE 600

typedef struct _server {
	int fd;
} socket_t;

void accept_connection(socket_t* server, socket_t* peer) {
   struct addrinfo hints;
   struct addrinfo* ptr;
	
   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
   hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
   hints.ai_flags = AI_PASSIVE;     /* AI_PASSIVE for server           */

   getaddrinfo(NULL, PORT, &hints, &ptr);

   server->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

   bind(server->fd, ptr->ai_addr, ptr->ai_addrlen);
   
   freeaddrinfo(ptr);

   listen(server->fd, CLIENT_QTTY);
   
   peer->fd = accept(server->fd, NULL, NULL); 
}

void receive_packet(socket_t* peer, char* packet) {
	char char_buff;
	recv(peer->fd, &char_buff, 1, MSG_NOSIGNAL);
	for (int i = 0; i < PACKET_SIZE && char_buff != 0; i++) {
		packet[i] = char_buff;
		recv(peer->fd, &char_buff, 1, MSG_NOSIGNAL);
	}
	packet[i] = '\0';
}

bool null_packet(char* packet) {
	for (int i = 0; i < PACKET_SIZE; i++)
		if (packet[i] != '\0') return false;
	return true;
}

bool reset_packet(char* packet) {
	for (int i = 0; i < PACKET_SIZE; i++)
		packet[i] = '\0';
}

void neg_packet(socket_t* peer, char* packet) {
	char nchar_buff;
	for (int i = 0; packet[i] != '\0'; i++) {
		nchar_buff = ~packet[i];
		send(peer->fd, &nchar_buff, 1, MSG_NOSIGNAL);
	}
	nchar_buff = '\0';
	send(peer->fd, &nchar_buff, 1, MSG_NOSIGNAL);
}

int main() {
   socket_t server;
   socket_t peer;
   accept_connection(&server, &peer);
   
   char packet[PACKET_LEN + 1];
   packet[PACKET_LEN] = '\0';
   receive_packet(&peer, packet);
   while (!null_packet(packet)) {
      neg_packet(&peer, packet);
      reset_buffer(packet);
      receive_packet(&peer, packet);
   }
   
   shutdown((&peer)->fd, SHUT_RDWR);
   close((&peer)->fd);
   shutdown((&server)->fd, SHUT_RDWR);
   close((&server)->fd);
   return 0;
}
