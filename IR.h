#ifndef __IR_H
#define __IR_H


extern unsigned char rx_packet[14];
extern unsigned char rx_index;
extern unsigned int pulses_seen;
extern unsigned char rx_fronthalf;

extern unsigned char got_packet;
extern long PulseWidth[70];	


void initIR();
void startIR();
void TMR04start(void);
void TMR04stop(void);
void TMR04set(unsigned int regvalue);
int  TMR04get();
unsigned char decodeIR();

#endif