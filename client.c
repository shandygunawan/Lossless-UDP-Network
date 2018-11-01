#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
//#define PORT 8080 
#define MAXBUFFER 1024 

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
    if (argc != 6){     // Jumlah parameter tidak sesuai
        printf("usage: %s <filename> <windowsize> <buffersize> <destination_ip> <destination_port>\n",argv[0]);
        printf("  where parameters are:\n");
        printf("  filename\t: the name of the output file.\n");
        printf("  windowsize\t: advertised window size (Total packets can be sent in one stream)\n");
        printf("  buffersize\t: maximum data can be send per frame (in bytes)\n");
        printf("  destination_ip\t\t: IP address of the server\n");
        printf("  destination_port\t\t: port number for sending data stream\n");
        exit(0);
    }

    if (!(checkInt(argv[2]))){      // Window size bukan angka
        printf("ERROR Window size is not a number\n");
        exit(0);
    }
    if (!(checkInt(argv[3]))){      // Buffer size bukan angka
        printf("ERROR Buffer size is not a number\n");
        exit(0);
    }
    if (!(checkInt(argv[5]))){    // Port bukan angka
        printf("ERROR Invalid port\n");
        exit(0);
    }

    // Masukkan nilai parameter ke variabel
    char* filename = argv[1];
    int window_size = atoi(argv[2]);
    int buffer_size = atoi(argv[3]);
    char* ip = argv[4];
    int port = atoi(argv[5]);

    printf("%s %d %d %s %d\n",filename,window_size,buffer_size,ip,port);
    // Creating a socket for both process
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    }

    // Creating child process
    // Note: it is alright to fork instantly if there is no pointer to be copied
    pid_t pid = fork();
    if (pid < 0){   // Child process creation failed, printing the error and exit
        fprintf(stderr,"\n");
        exit(1);
    } else if (pid == 0){   // Child process - Process to handle incoming transmission
        char buffer[MAXBUFFER];
        struct sockaddr_in server_address; 
      
        memset(&server_address, 0, sizeof(server_address)); 
        
        server_address.sin_family = AF_INET;            //IPV4
        server_address.sin_port = htons(port);          //From specified port
        server_address.sin_addr.s_addr = INADDR_ANY;

        int received_data_length, dummy;    //r_d_l keeps the length of received data (NOT THE STRING)
        for (;;){
            //Receives the data and try to print it
            received_data_length = recvfrom(sockfd, (char *)buffer, MAXBUFFER, MSG_WAITALL, (struct sockaddr *) &server_address, &dummy);
            buffer[received_data_length] = '\0';
            printf("Message from server : %s\n\n", buffer);

            if (strcmp(buffer,"EXIT") == 0){    //If socket receives exit string, break out of loop and close everything
                break;
            }
        }
        close(sockfd);

        _exit(0);
    } else {    // Parent process - Process to handle sending transmission (sending frames)
        struct sockaddr_in server_address; 
      
        memset(&server_address, 0, sizeof(server_address)); 
           
        server_address.sin_family = AF_INET;            //IPV4
        server_address.sin_port = htons(port);          //Specified port
        server_address.sin_addr.s_addr = INADDR_ANY;

        FILE * fp;
        char * buffer;
        size_t string_length = 0;
        ssize_t read_status;

        fp = fopen("./test.txt","r");
        for(;;){
            // Gets daa from the file
            read_status = getline(&buffer,&string_length,fp);

            // If we're not EOF yet
            if (read_status != -1){
                // Sends the data
                sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &server_address, sizeof(server_address));
                printf("String sent\n"); 
            } else {
                // Send exit flag to the server
                buffer = "EXIT";
                sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &server_address, sizeof(server_address));
                break;
            }

        }
        fclose(fp);
        close(sockfd);

        exit(0);
    }

    return 0; 
} 