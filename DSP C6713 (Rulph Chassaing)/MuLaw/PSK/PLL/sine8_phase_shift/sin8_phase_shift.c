//sin8_phase_shift Sine generation. Illustrate phase shift 

#include "dsk6713_aic23.h"				//codec-DSK support file
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;	//set sampling rate
short loop = 0;
short sine_table[8]={0,707,1000,707,0,-707,-1000,-707};//sine values
short phase_change_idx = 0;

interrupt void c_int11()            //interrupt service routine
{
  output_sample(sine_table[loop]);
  if (loop < 7)  ++loop;          //reinit index loop
  else  loop = 0;
  if (phase_change_idx++ >= 50*8)	   //phase shift every 50 periods
  {
   if (loop == 7) 	loop = 0;      //skip a value every 50 periods
   else	  loop++;
   phase_change_idx = 0;
  }
  return;
}

void main()
{
  comm_intr();  while(1); 				//init DSK/infinite loop
}
 
