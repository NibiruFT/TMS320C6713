//Noisegen_casm.c Pseudo-random noise generation calling ASM function

#include "dsk6713_aic23.h"			//codec-DSK support file
Uint32 fs=DSK6713_AIC23_FREQ_48KHZ; //set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6713_AIC23_INPUT_MIC; // select mic in
int previous_seed;
short pos=16000, neg=-16000;		//scaling noise level

interrupt void c_int11()
{
	previous_seed = noisefunc(previous_seed); //call ASM function

	if (previous_seed & 0x01) output_left_sample(pos);//positive scaling
	else output_left_sample(neg);						//negative scaling
}

void main ()
{
	comm_intr();  										//init DSK, codec, McBSP
	previous_seed = noisefunc(0x7E521603);  //call ASM function
   while (1);								//infinite loop
}
