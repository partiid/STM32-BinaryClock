#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include "stm32f3xx_hal.h"


#define BUFFER_SIZE 64

typedef struct 
{
unsigned char buffer[BUFFER_SIZE];
volatile unsigned int head;
volatile unsigned int tail; 
};


/* read data in rx_buffer and inc the tail in rx_buffer */ 
int UartRead(void); 


/* write data to tx buff and inc the head count in tx buffer */ 
void UartWrite(void); 


/* send string in uart */ 
void UartSendString(const char *s); 





#endif 


