#ifndef __MCP6S92_H
#define __MCP6S92_H

#define CS_MCP6S22     P5_bit.no5
#define PM_CS_MCP6S22  PM5_bit.no5


void initMCP62s92();
int MCP6s92Send(unsigned char *txbuf, unsigned short txnum);
void setChanMCP6s92(unsigned char chan);
int setGainMCP6S92(unsigned char gain);



#endif 