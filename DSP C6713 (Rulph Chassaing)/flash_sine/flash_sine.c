//Flash_sine.c Sine generation to illustrate use of flash

#include "dsk6713_aic23.h"				//codec-DSK support file
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;	//set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // select mic in
short loop = 0,  gain = 10;
short sine_table[8] = {0,707,1000,707,0,-707,-1000,-707}; //sine values

interrupt void c_int11()            //interrupt service routine
{
  output_sample(sine_table[loop]*gain);
  if (++loop > 7) loop = 0;         //if end of buffer,reinit index
  return;
}

void main()
{
  comm_intr();                  		//init DSK, codec, McBSP
  while(1);                     		//infinite loop
}
 
