#ifndef __AD_AN13_H
#define __AD_AN13_H


void initADI3();
void AD_ANI3read(unsigned short *buffer);
__interrupt void int_ANI3(void);

#endif