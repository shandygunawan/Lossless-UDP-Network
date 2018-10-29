#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "data.h"
#include "dataHandler.cpp"
#include "slidingWindow.h"

int main(int argc, char *argv[]) {

	// ARGUMENTS HANDLING
	char* filename = argv[1];
	unsigned int window_size = atoi(argv[2]);
	unsigned int buffer_size = atoi(argv[3]);
	char* dest_IP = argv[4];
	int dest_port = atoi(argv[5]);

	// SOCKET
	int udp_socket;
	struct sockaddr_in client_address;
	unsigned char buffer[buffer_size];

	FILE* file = fopen(filename, "r");
	if (file == NULL){
		printf("File not found!");
		exit(0);
	}

	if( (udp_socket = socket(AF_INET, SOCK_DGRAM, 0) ) < 0){
		perror(error_message);
		exit(0);
	}

	// TIMEOUT
	// (nanti aja)

	memset(&serve_address, 0, sizeof(server_address));
	memset(&client_address, 0, sizeof(client_address));

	// FILLING CLIENT INFORMATION
	client_address.sin_family = AF_INET; //IPv4
	client_address.sin_port = htons(dest_port);
	client_address.sin_addr.s_addr = inet_addr(dest_IP);

	return 0;
}
