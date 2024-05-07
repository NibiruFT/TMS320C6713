//FIRcirc.c  C program calling ASM function using circular buffer 

#include "DSK6713_AIC23.h"	//codec-DSK support file
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // select input source
#include "bp1750.cof"  	            //BP at 1750 Hz coeff file
int yn = 0;				      //init filter's output

interrupt void c_int11()            //ISR
{
 short sample_data;

 sample_data = (input_sample());      //newest input sample data
 yn = fircircfunc(sample_data,h,N); //ASM func passing to A4,B4,A6
 output_sample((short)(yn>>15));	      //filter's output
 return;				      //return to calling function
}

void main()
{
 comm_intr();			      //init DSK, codec, McBSP
 while(1);                 	      //infinite loop
}

