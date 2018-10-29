#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

struct Sliding_Window {
	unsigned int sending_window_size;
	unsigned int last_ack_received;
	unsigned int last_frame_sent;
}

#define SWS(S) (S).sending_window_size;
#define LAR(S) (S).last_ack_received;
#define LFS(S) (S).last_frame_sent;

void createSlidingWindow(Sliding_Window *sw, unsigned int window_size){
	sw->sending_window_size = window_size;
	sw->last_ack_received = 0;
	sw->last_frame_sent = 0;
}

void createSlidingWindow(Sliding_Window *sw, unsigned int window_size, unsigned last_ack_received, unsigned last_frame_sent) {
	sw->sending_window_size = window_size;
	sw->last_ack_received = last_ack_received;
	sw->last_frame_sent = last_frame_sent;
}

void receiveAck(Sliding_Window *sw, int ack) {
	if(LAR(*sw) < ack) {
		LAR(*sw) = ack;
	}
	else {
		LAR(*sw);
	}
}

void sendFrames(Sliding_Window sw, int frame_count){
	// validate frame_count < SWS(sw)
	if(frame_count >= SWS(sw)) {
		frame_count = SWS(sw);
	}

	// validate LFS - LAR <= SWS
	if( (LFS(sw) + frame_count) > (SWS(sw) + LAR(sw)) ) {
		frame_count = SWS(sw) + LAR(sw) - LFS(sw);
	}

	LFS(sw) = LFS(sw) + frame_count;
}

#endif