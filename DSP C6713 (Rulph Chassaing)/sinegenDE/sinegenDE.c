//sinegenDE.c generates sinusoid using difference equations 

#include "DSK6713_AIC23.h"		//codec support
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // select input
#include <math.h>
#define FREQ 2000
#define SAMPLING_FREQ 8000
#define AMPLITUDE 10000
#define PI 3.14159265358979

float y[3] = {0.0, 0.0, 0.0};
float a1;

interrupt void c_int11()            //ISR
{
  y[0] =(y[1]*a1)-y[2];
 y[2] = y[1];                             //update y1(n-2)
 y[1] = y[0];                             //update y1(n-1)
 output_left_sample((short)(y[0]*AMPLITUDE));             //output result
 return;					//return to main
}

void main()
{
y[1] = sin(2.0*PI*FREQ/SAMPLING_FREQ);
a1 = 2.0*cos(2.0*PI*FREQ/SAMPLING_FREQ);
 comm_intr();                  		  //init DSK, codec, McBSP
 while(1);                     		  //infinite loop
}
