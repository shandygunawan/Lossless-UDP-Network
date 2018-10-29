#include <stdio.h>
#include <stdlib.h>
#include "data.h"

char generatePacketChecksum() {
	
}

Packet createPacket(char soh, int seq_number, int data_length, char data[], char input_checksum){
	Packet P;

	SOH(P) = soh;
	sequenceNumber(P) = seq_number;
	dataLength(P) = data_length;
	checksum(P) = input_checksum;

	int i;
	for(i = 0; i < data_length; i++){
		data(P,i) = data[i];
	}

	return P;
}

Response createResponse(char ack, int next_seq_number, char input_checksum){
	Response R;

	ACK(R) = ack;
	nextSequenceNumber(R) = next_seq_number;
	checksum(R) = input_checksum;

	return R;
}