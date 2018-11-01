#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
//#define PORT 8080 
#define MAXLINE 1024 

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

int main(int argc, char** argv) { 
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

    // Masukkan nilai parameter ke variabel
    char* filename = argv[1];
    int window_size = atoi(argv[2]);
    int buffer_size = atoi(argv[3]);
    int port = atoi(argv[4]);

    printf("%s %d %d %d\n",filename,window_size,buffer_size,port);

    //Creating socket
    int sockfd; 
    struct sockaddr_in server_address, client_address; 
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&server_address, 0, sizeof(server_address)); 
    memset(&client_address, 0, sizeof(client_address)); 
    
    //NOTE: server_address is the address where ./server runs
    server_address.sin_family = AF_INET;         //IPv4
    server_address.sin_port = htons(port);          //From specified port
    server_address.sin_addr.s_addr = INADDR_ANY;    //Accepts any connection
      
    // Bind the socket with the server address information
    if (bind(sockfd, (const struct sockaddr *)&server_address, sizeof(server_address)) < 0){ 
        perror("bind failed"); 
        exit(EXIT_FAILURE);
    } 
      
    int received_data_length, dummy;
    char buffer[MAXLINE];
    for(;;){
        //Receive the data transmitted by client
        received_data_length = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &client_address, &dummy); 
        buffer[received_data_length] = '\0';
        printf("Message from client : %s\n", buffer); 

        if (strcmp(buffer,"EXIT") == 0){
            //Client sends EXIT flag, send EXIT flag back to client and terminate server
            char * exit = "EXIT";

            sendto(sockfd, (const char *)exit, strlen(exit), MSG_CONFIRM, (const struct sockaddr *) &client_address, sizeof(client_address));
            break;
        } else {
            //Client not sending the EXIT flag, send response
            char * response = "Message received";

            sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (const struct sockaddr *) &client_address, sizeof(client_address)); 
            printf("Response sent.\n");
        }
    }
      
    return 0; 
} 