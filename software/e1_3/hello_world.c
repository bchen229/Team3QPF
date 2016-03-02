/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */
#define RS232_Control (*(volatile unsigned char *)(0x84000200))
#define RS232_Status (*(volatile unsigned char *)(0x84000200))
#define RS232_TxData (*(volatile unsigned char *)(0x84000202))
#define RS232_RxData (*(volatile unsigned char *)(0x84000202))
#define RS232_Baud (*(volatile unsigned char *)(0x84000204))

#include <stdio.h>

void Init_RS232(void)
{
 // set up 6850 Control Register to utilise a divide by 16 clock,
 // set RTS low, use 8 bits of data, no parity, 1 stop bit,
 // transmitter interrupt disabled
 // program baud rate generator to use 115k baud

	RS232_Baud = 0x01 ; //Program Baud rate for 115K
	//RS232_Control = 0x03; // Reset ACIA
	RS232_Control = 0x15; // x16 clock , 8 data , no parity , 1 stop , no interrupt

}

int putcharRS232(int c)
{
// poll Tx bit in 6850 status register. Wait for it to become '1'

// write 'c' to the 6850 TxData register to output the character


	while ( RS232_Status & 0x02 != 0x02 );

		RS232_TxData = c&0xFF;


 return RS232_TxData ; // return c
}
int getcharRS232( void )
{
	int rx;
 // poll Rx bit in 6850 status register. Wait for it to become '1'
 // read received character from 6850 RxData register.
	while ( RS232_Status & 0x01 != 0x01 );

		rx = RS232_RxData ;

	return rx;
}
// the following function polls the 6850 to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read
int RS232TestForReceivedData(void)
{
 // Test Rx bit in 6850 serial comms chip status register
 // if RX bit is set, return TRUE, otherwise return FALSE
	if (RS232_RxData && 0x01 == 0x01){
	return 1;
	}
	else
		return 0;
}

int main()
{
  printf("Hello from Nios II!\n");

  return 0;
}
