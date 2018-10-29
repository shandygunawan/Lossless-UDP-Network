#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int main(int argc, char** argv){
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
}