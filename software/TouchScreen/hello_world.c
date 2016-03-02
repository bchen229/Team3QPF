#define Touch_Control (*(volatile unsigned char *)(0x84000230))
#define Touch_Status (*(volatile unsigned char *)(0x84000230))
#define Touch_Transmit (*(volatile unsigned char *)(0x84000232))
#define Touch_Recieve (*(volatile unsigned char *)(0x84000232))
#define Touch_Baud (*(volatile unsigned char *)(0x84000234))


#include <stdio.h>
#include "sys/alt_alarm.h"

/*****************************************************************************
** Initialise touch screen controller
*****************************************************************************/
void Init_Touch(void)
{
 // Program 6850 and baud rate generator to communicate with touchscreen
 // send touchscreen controller an "enable touch" command
	Touch_Control = 0x15;
	Touch_Baud = 0x05;
	usleep(4000); // sleep for 4 milliseconds
 // enable touch
	putcharTouch(0x55);
	putcharTouch(0x01);
	putcharTouch(0x12);
	usleep(4000);
}

/*****************************************************************************
** test if screen touched
*****************************************************************************/
int ScreenTouched(void)
{
 // return TRUE if any data received from 6850 connected to touchscreen
 // or FALSE otherwise
	return (Touch_Recieve == 0x80);
}

/*****************************************************************************
** wait for screen to be touched
*****************************************************************************/
void WaitForTouch()
{
	while(!ScreenTouched());
}

/* a data type to hold a point/coord */
typedef struct { int x, y; } Point ;
/*****************************************************************************
* This function waits for a touch screen press event and returns X,Y coord
*****************************************************************************/
Point GetPress(void)
{
	Point p1;
	 int buf[4];
	 int i;
	 int yOffset = 125;
	 // wait for a pen down command then return the X,Y coord of the point
	 // calibrated correctly so that it maps to a pixel on screen
	 WaitForTouch();
	 // pick up reponse packets
	 for(i = 0; i < 4; i++){
		 buf[i] = getcharTouch();
	 }
	 // parse x and y coordinates
	 p1.x = (buf[1] << 7) | buf[0];
	 p1.y = (buf[3] << 7) | buf[2];
	 // compute screen coordinates
	 p1.x = p1.x * 799/4095;
	 p1.y = (p1.y - yOffset) * 479/(4095 - yOffset);
	 printf("x = %d ", p1.x);
	 printf("y = %d\n", p1.y);
	 return p1;
}

/*****************************************************************************
* This function waits for a touch screen release event and returns X,Y coord
*****************************************************************************/
Point GetRelease(void)
{
	Point p1;
	 int buf[4];
	 int i;
	 int yOffset = 150;
	 // wait for a pen down command then return the X,Y coord of the point
	 // calibrated correctly so that it maps to a pixel on screen
	 WaitForTouch();
	 // pick up reponse packets
	 for(i = 0; i < 4; i++){
		 buf[i] = getcharTouch();
	 }
	 // parse x and y coordinates
	 p1.x = (buf[1] << 7) | buf[0];
	 p1.y = (buf[3] << 7) | buf[2];
	 // compute screen coordinates
	 p1.x = p1.x * 799/4095;
	 p1.y = (p1.y - yOffset) * 479/(4095 - yOffset);
	 printf("x = %d ", p1.x);
	 printf("y = %d\n", p1.y);
	 return p1;
}

int putcharTouch(int c)
{
// poll Tx bit in 6850 status register. Wait for it to become '1'
// write 'c' to the 6850 TxData register to output the character
	while ( (Touch_Status & 0x02) != 0x02 );
	Touch_Transmit = c&0xFF;
	return Touch_Transmit; // return c
}

int getcharTouch( void )
{
	int rx;
 // poll Rx bit in 6850 status register. Wait for it to become '1'
 // read received character from 6850 RxData register.
	while ( (Touch_Status & 0x01) != 0x01 );
	rx = Touch_Recieve;
	return rx;
}

int main()
{
  printf("Hello from Nios II!\n");
  Init_Touch();
//  WaitForTouch();
//  printf("done");
//  int c;
//  while(1){
//	  c = getcharTouch();
//	  printf("%d ", c);
//  }
  while(1){
	  GetPress();
  	  GetRelease();
  }
  return 0;
}

