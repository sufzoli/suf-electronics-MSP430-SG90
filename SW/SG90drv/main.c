#include <msp430.h> 

/*
 * main.c
 */


#include "serial.h"

signed volatile char rxEchoPos;

int checkSerial()
{
	int i;
	int retvalue = -1;
	// Iterate trough the received characters
	for(;rxEchoPos<rxBuffLen;rxEchoPos++)
	{
		// if it is not an Enter
		if(rxBuffer[rxEchoPos] != 13)
		{
			// Echo the char back
			TimerA_UART_tx(rxBuffer[rxEchoPos]);
		}
		else
		{
			// Echo back correct Enter for the terminal
			TimerA_UART_print("\r\n");
			retvalue = 0;
			// iterate trough the received characters
			for(i = 0; i < rxBuffLen-1; i++)
			{
				// if it is a number
				if(rxBuffer[i] > 47 && rxBuffer[i] < 58)
				{
					// add to the return value
					retvalue = retvalue * 10 + rxBuffer[i] - 48;
				}
			}
			// reset the receive buffer
			rxEchoPos = -1;
			rxBuffLen = 0;
		}
	}
	return retvalue;
}

void moveServo(int deg)
{
	int pwm,k,i;
	// generate PWM signal for the servo
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
	// send back an acknowledge to
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

	// Initialize serial port
	SERIAL_init();
	// Enable interrupt
	_EINT();
	rxEchoPos = 0;

	// Set the servo port to digital output
	P2DIR |= BIT2;

 	int deg; // servo rotation degree

 	// Main loop
 	while(1)
 	{
 		// load the target degree from the serial port
 		deg = checkSerial();
 		// if there is a new value
 		if(deg > -1)
 		{
 			// Move the servo
 			moveServo(deg);
 		}
 		__delay_cycles(1000);
 	}
}
