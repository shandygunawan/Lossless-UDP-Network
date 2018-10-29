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

int checkInt(char* text){
/* Checks if the string is a number or not */
    int flag = 1;

    for (int i = 0; i < strlen(text); i++){
        if ((text[i] < 0x30) || (text[i] > 0x39)){
            flag = 0;
            break;
        }
    }

    return flag;
}

int main(int argc, char *argv[]) {

	if (argc != 5){     //Jumlah parameter tidak sesuai
        printf("usage: %s <filename> <windowsize> <buffersize> <port>\n",argv[0]);
        printf("  where parameters are:\n");
        printf("  filename\t: the name of the output file.\n");
        printf("  windowsize\t: advertised window size (Total packets can be sent in one stream)\n");
        printf("  buffersize\t: maximum data can be send per frame (in bytes)\n");
        printf("  port\t\t: port number of the incoming connection\n");
        exit(0);
    }

    if (!(checkInt(argv[2]))){      // Window size bukan berupa angka
        printf("ERROR Window size is not a number\n");
        exit(0);
    }
    if (!(checkInt(argv[3]))){      // Buffer size bukan berupa angka
        printf("ERROR Buffer size is not a number\n");
        exit(0);
    }
    if (!(checkInt(argv[4]))){      // Port bukan berupa angka
        printf("ERROR Invalid port\n");
        exit(0);
    }


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

	printf("%s %d %d %s %d\n",filename,window_size,buffer_size,ip,port);
	return 0;
}
