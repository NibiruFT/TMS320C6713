//firprn.c FIR with internally generated input noise sequence 
#include "DSK6713_AIC23.h"	              // codec support
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;	        //set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // select line in

#include "bs2700f.cof"                //filter coefficient file
#include "noise_gen.h"	            //support file for noise sequence generation 
int fb;                            //feedback variable
shift_reg sreg;                    //shift register 
#define NOISELEVEL 8000            //scale factor for +/- 1 noise sequence
float x[N];                        //filter delay line

int prand(void) 	                  //pseudo-random noise generation
{
  int prnseq;			            
  if(sreg.bt.b0)
    prnseq = -NOISELEVEL;          //scaled negative noise level
  else
    prnseq = NOISELEVEL;           //scaled positive noise level
  fb =(sreg.bt.b0)^(sreg.bt.b1);   //XOR bits 0,1
  fb^=(sreg.bt.b11)^(sreg.bt.b13); //with bits 11,13 -> fb
  sreg.regval<<=1;                 //shift register 1 bit to left
  sreg.bt.b0=fb;                   //close feedback path
  return prnseq;
}

void resetreg(void)                //reset shift register to nominal values
{
  sreg.regval=0xFFFF;              //initial seed value
  fb = 1;                          //initial feedback value
}

interrupt void c_int11()           //interrupt service routine
{                            
  short i;                         //declare index variable
  float yn = 0.0;

  x[0] = (float)(prand());         //get new input into delay line
  for (i=0 ; i<N ; i++)            //calculate filter output
    yn += h[i]*x[i];
  for (i=(N-1) ; i>0 ; i--)        //shuffle delay line contents
    x[i] = x[i-1];
  output_left_sample((short)(yn)); //output to codec
  return;                          //return from interrupt
}

void main()
{
  resetreg();                      //reset shift register to nominal values
  comm_intr();                     //initialise McBSP and AD535
  while (1);                       //infinite loop
}

