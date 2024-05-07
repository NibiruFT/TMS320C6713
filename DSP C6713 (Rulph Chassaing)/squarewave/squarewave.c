//squarewave.c 125 Hz square wave generated using lookup table
#include "dsk6713_aic23.h"          //codec support
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;  // set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // select input
#define LOOPLENGTH 64								
short square_table[LOOPLENGTH] =
{10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000,
-10000,-10000,-10000,-10000,-10000,-10000,-10000,-10000,
-10000,-10000,-10000,-10000,-10000,-10000,-10000,-10000,
-10000,-10000,-10000,-10000,-10000,-10000,-10000,-10000,
-10000,-10000,-10000,-10000,-10000,-10000,-10000,-10000};
short loopindex = 0;

interrupt void c_int11()
{
  output_sample(square_table[loopindex++]);
  if (loopindex >= LOOPLENGTH)
    loopindex = 0;
  return;										}

void main()
{
  comm_intr();
  while(1);    
}
