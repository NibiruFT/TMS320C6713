//Sinegencasm.c Sine gen using DE with asm function

#include "dsk6713_aic23.h"			//codec-DSK support file
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // select input source

short y[3] = {0, 15137, 11585};		//y(1)=sinwT (f=1.5kHz)
short A = 12540;							//A=2*coswT * 2^14
short n = 2;							

interrupt void c_int11()				//interrupt service routine
{
	sinegencasmfunc(&y[0], A);			//calls ASM function
	output_sample(y[n]);
	return;
}

void main()
{
  comm_intr();                  		//init DSK, codec, McBSP
  while(1);							      //infinite loop
}




 
