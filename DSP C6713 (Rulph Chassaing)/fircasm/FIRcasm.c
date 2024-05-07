//FIRCASM.c FIR C program calling ASM function fircasmfunc.asm

#include "DSK6713_AIC23.h"	//codec-DSK support file
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;	//set sampling rate
#include "bp41.cof"              //BP @ Fs/8 coefficient file
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // select mic in
int yn = 0;				   //initialize filter's output
short dly[N];        		   //delay samples 

interrupt void c_int11()         //ISR
{
	dly[N-1] = input_left_sample(); //newest sample @bottom buffer
	yn = fircasmfunc(dly,h,N); //to ASM func through A4,B4,A6
   	output_left_sample((short)(yn>>15));   //filter's output
	return;			   //return from ISR
}

void main()
{
 	short i;

	for (i = 0; i<N; i++)
  	dly[i] = 0;                //init buffer for delays
	comm_intr();               //init DSK, codec, McBSP
	while(1);                  //infinite loop
}

