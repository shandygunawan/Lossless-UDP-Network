#ifndef __DATA_C__
#define __DATA_C__

// DATA STRUCTURE
// Frame
struct packet {
    char SOH;
    int sequenceNumber;
    int dataLength;
    char data[1024];
    char checksum;
};

// ACK
struct response {
    char ACK;
    int nextSequenceNumber;
    char checksum;
};


// CONSTANTS
#define ASCII_SOH 0x01
#define ASCII_ACK 0x06
#define TIMEOUT 2000


// SELECTORS
// Frame
#define SOH(X) X.SOH;
#define sequenceNumber(X) X.sequenceNumber;
#define dataLength(X) X.dataLength;
#define data(X,i) X.data[i];
#define checksum(X) X.checksum;

// ACK
#define ACK(X) X.ACK;
#define nextSequenceNumber(X) X.nextSequenceNumber;
#define checksum(X) X.checksum;

#endif