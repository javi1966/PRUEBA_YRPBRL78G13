#ifndef __BT_JY104_H
#define __BT_JY104_H


//funciones soporte para modulo bluetooh BT_JY
#define RX_BUFF_SIZE 64
#define RxBuffLen             ((unsigned char)(RxBuffer.wrPtr - RxBuffer.rdPtr))

typedef struct _CommsBuffer
 {
    unsigned char rdPtr;                                     // Read Pointer
    unsigned char wrPtr;                                     // Write Pointer
    volatile unsigned char buffer[RX_BUFF_SIZE];             // The Buffer
}CommsBuffer, *CommsBufferPtr;

static CommsBuffer RxBuffer  = {0, 0, };                       // UART2 Rx buffer
static CommsBufferPtr RxBufferPtr = &RxBuffer;

//*********************************************************************************************
void initBT();
void startBT();
void stopBT();
int BT_ReceiveData(unsigned char *rxbuf, unsigned short rxnum);
int BT_SendData(unsigned char* txbuf, unsigned short txnum);
void UART2_InitTxBuffers(void);
__interrupt void MD_INTSR2(void);
__interrupt void MD_INTSRE2(void);
__interrupt void MD_INTST2(void);

#endif
