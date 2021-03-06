/*
 * serial.h
 *
 *  Created on: 2013.12.22.
 *      Author: zoli
 */

#ifndef SERIAL_H_
#define SERIAL_H_

//------------------------------------------------------------------------------
// Hardware-related definitions
//------------------------------------------------------------------------------
#define UART_TXD   0x02                     // TXD on P1.1 (Timer0_A.OUT0)
#define UART_RXD   0x04                     // RXD on P1.2 (Timer0_A.CCI1A)

//------------------------------------------------------------------------------
// Conditions for 9600 Baud SW UART, SMCLK = 12MHz
//------------------------------------------------------------------------------
// #define UART_TBIT_DIV_2     (12000000 / (9600 * 2))
// #define UART_TBIT           (12000000 / 9600)

// #define UART_TBIT_DIV_2     (12000000 / (115200 * 2))
// #define UART_TBIT           (12000000 / 115200)

#define UART_TBIT_DIV_2     (16000000 / (9600 * 2))
#define UART_TBIT           (16000000 / 9600)



extern unsigned char rxBuffer[10];
extern unsigned char rxBuffLen;

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------
void SERIAL_init();
void TimerA_UART_init(void);
void TimerA_UART_tx(unsigned char byte);
void TimerA_UART_print(char *string);

#endif /* SERIAL_H_ */
