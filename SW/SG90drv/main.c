#include <msp430.h> 

/*
 * main.c
 */


#include "serial.h"

unsigned volatile char rxEchoPos;

int checkSerial()
{
	int i;
	int retvalue = -1;
	for(;rxEchoPos<rxBuffLen;rxEchoPos++)
	{
		if(rxBuffer[rxEchoPos] != 13)
		{
			TimerA_UART_tx(rxBuffer[rxEchoPos]);
		}
		else
		{
			TimerA_UART_print("\r\n");
			retvalue = 0;
			for(i = 0; i < rxBuffLen-1; i++)
			{
				if(rxBuffer[i] > 47 && rxBuffer[i] < 58)
				{
					retvalue = retvalue * 10 + rxBuffer[i] - 48;
				}
			}
			rxEchoPos = -1;
			rxBuffLen = 0;
		}
	}
	return retvalue;
}

void moveServo(int deg)
{
	int pwm,k,i;
	pwm = 60 + deg;
	for(k=0;k<50;k++)
	{
		P2OUT |= BIT2;
		for(i=0;i<pwm;i++)
			__delay_cycles(125);
		P2OUT &= ~BIT2;
		for(i=0;i<2000-pwm;i++)
			__delay_cycles(125);
	}
	TimerA_UART_print("ACK\r\n");
}


void main (void)
{
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

    // Set 16MHz clock
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
	// Startup Delay
	_delay_cycles(2000);

	SERIAL_init();
	_EINT();
	rxEchoPos = 0;

	P2DIR |= BIT2;

 	int i,j,k;

 	int deg;

 	int pwm;

 	// moveServo(0);

 	while(1)
 	{
 		/*
 		for(j=0;j<=180;j+=10)
 		{
 			moveServo(j);
 		}
		__delay_cycles(10000000);
		*/

 		deg = checkSerial();
 		if(deg > -1)
 		{
 			moveServo(deg);
 		}
 		__delay_cycles(1000);
 		/*
 		for(j=0;j<=180;j+=10)
 		{
 			pwm = 60 + j;
 			for(k=0;k<50;k++)
 			{
 				P1OUT |= BIT4;
 				for(i=0;i<pwm;i++)
 					__delay_cycles(125);
 				P1OUT &= ~BIT4;
 				for(i=0;i<2000-pwm;i++)
 					__delay_cycles(125);
 				checkSerial();
 			}
 			__delay_cycles(1000);
 		}

		__delay_cycles(10000000);
//		TimerA_UART_print("Cycle Finished\r\n");
 		 */
 	}
}
