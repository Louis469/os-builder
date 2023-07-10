#include "simplesound.h"

static void play_sound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 	Div = 1193180 / nFrequence;
 	port_byte_out(0x43, 0xb6);
 	port_byte_out(0x42, (uint8_t) (Div) );
 	port_byte_out(0x42, (uint8_t) (Div >> 8));

 	tmp = port_byte_in(0x61);
  	if (tmp != (tmp | 3)) {
 		port_byte_out(0x61, tmp | 3);
 	}
	uint32_t divisor = 1193180 / timerfreq;
  uint8_t low  = (uint8_t)(divisor & 0xFF);
  uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);
    
  port_byte_out(0x43, 0x36); 
  port_byte_out(0x40, low);
  port_byte_out(0x40, high);
}

static void nosound() {
 	uint8_t tmp = port_byte_in(0x61) & 0xFC;
 
 	port_byte_out(0x61, tmp);
}

void beep(uint32_t nFrequence, uint32_t time) {
 	play_sound(nFrequence);
 	sleep(time);
 	nosound();
}