#include "melody.h"
#include <stdlib.h>

C_melody::C_melody(bool looped):loop(looped),duration(0){}

C_melody::~C_melody(){
	int n = frames.quantity();
	for (int i = 0; i < n; i++){
		delete frames.pop();
	}
}

void C_melody::add_frame(melody_frame frame){
	frames.add(new melody_frame(frame));
	duration += frame.duration;
}

bool C_melody::get_looped(){
	return loop;
}

melody_state C_melody::get_state(unsigned long time){
	if (!duration) return melody_state__error;
	if (time > duration){
		if (!loop){
			return melody_state__error;
		}
		time %= duration;
	}
	int n = frames.quantity();
	melody_frame * frame = NULL;
	for (int i = 0; i < n; i++){
		frame = frames.get(i);
		if (time < frame->duration) return frame->state;
		time -= frame->duration;
	}
	return melody_state__error;
}
