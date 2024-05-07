//GraphicEQ.c Graphic Equalizer using TI floating-point FFT functions

#include "DSK6713_AIC23.h"		//codec-DSK support file
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;		//set sampling rate
#include <math.h>
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // select mic in
#include "GraphicEQcoeff.h"       //time-domain FIR coefficients
#define PI 3.14159265358979
#define PTS 256                   //number of points for FFT
//#define SQRT_PTS 16
#define RADIX 2			
#define DELTA (2*PI)/PTS
typedef struct Complex_tag {float real,imag;} COMPLEX;
#pragma DATA_ALIGN(W,sizeof(COMPLEX))
#pragma DATA_ALIGN(samples,sizeof(COMPLEX))
#pragma DATA_ALIGN(h,sizeof(COMPLEX))
COMPLEX W[PTS/RADIX] ;		    //twiddle array 
COMPLEX samples[PTS];
COMPLEX h[PTS];
COMPLEX bass[PTS], mid[PTS], treble[PTS];
short buffercount = 0;            //buffer count for iobuffer samples  
float iobuffer[PTS/2];            //primary input/output buffer
float overlap[PTS/2];		    //intermediate result buffer
short i;                          //index variable 
short flag = 0;                   //set to indicate iobuffer full
float a, b;                       //variables for complex multiply
short NUMCOEFFS = sizeof(lpcoeff)/sizeof(float);
short iTwid[PTS/2] ;		    
float bass_gain = 1.0;            //initial gain values 
float mid_gain = 0.0;             //change with GraphicEQ.gel
float treble_gain = 1.0;

interrupt void c_int11(void)      //ISR
{
 output_left_sample((short)(iobuffer[buffercount]));
 iobuffer[buffercount++] = (float)((short)input_left_sample());
 if (buffercount >= PTS/2)        //for overlap-add method iobuffer
  {                               //is half size of FFT used
   buffercount = 0;
   flag = 1;
  }
}
     
main()
{
 digitrev_index(iTwid, PTS/RADIX, RADIX);	
 for( i = 0; i < PTS/RADIX; i++ )
  {
   W[i].real = cos(DELTA*i);
   W[i].imag = sin(DELTA*i);
  }
 bitrev(W, iTwid, PTS/RADIX);     //bit reverse W

 for (i=0 ; i<PTS ; i++)
  {
   bass[i].real = 0.0;
   bass[i].imag = 0.0;
   mid[i].real = 0.0;
   mid[i].imag = 0.0;
   treble[i].real = 0.0;
   treble[i].imag = 0.0;
  }
 for (i=0; i<NUMCOEFFS; i++)      //same # of coeff for each filter
  {                             
   bass[i].real = lpcoeff[i];     //lowpass coeff
   mid[i].real =  bpcoeff[i];     //bandpass coeff
   treble[i].real = hpcoeff[i];   //highpass coef
  }                        
 
 cfftr2_dit(bass,W,PTS);          //transform each band 
 cfftr2_dit(mid,W,PTS);			  //into frequency domain
 cfftr2_dit(treble,W,PTS); 
 
 comm_intr();                     //initialise DSK, codec, McBSP
 while(1) 				    //frame processing infinite loop
  {
   while (flag == 0);             //wait for iobuffer full
          flag = 0;
   for (i=0 ; i<PTS/2 ; i++)      //iobuffer into samples buffer  
    {                               
     samples[i].real = iobuffer[i];
     iobuffer[i] = overlap[i];    //previously processed output 
    }					    //to iobuffer
   for (i=0 ; i<PTS/2 ; i++)       
    {                             //upper-half samples to overlap
     overlap[i] = samples[i+PTS/2].real;
     samples[i+PTS/2].real = 0.0; //zero-pad input from iobuffer
    }                             
   for (i=0 ; i<PTS ; i++)                          
     samples[i].imag = 0.0;       //init samples buffer
   
   cfftr2_dit(samples,W,PTS);
   
   for (i=0 ; i<PTS ; i++)        //construct freq domain filter 
    {                             //sum of bass,mid,treble coeffs 
    h[i].real = bass[i].real*bass_gain + mid[i].real*mid_gain
		  + treble[i].real*treble_gain;
    h[i].imag = bass[i].imag*bass_gain + mid[i].imag*mid_gain
		  + treble[i].imag*treble_gain;
    }          
   for (i=0; i<PTS; i++)          //frequency-domain representation
    {                             //complex multiply samples by h
     a = samples[i].real;
     b = samples[i].imag;
     samples[i].real = h[i].real*a - h[i].imag*b;
     samples[i].imag = h[i].real*b + h[i].imag*a;
    }
   
   icfftr2_dif(samples,W,PTS);
   
   for (i=0 ; i<PTS ; i++) 
      samples[i].real /= PTS;
   for (i=0 ; i<PTS/2 ; i++)      //add 1st half to overlap
      overlap[i] += samples[i].real;
  }                               //end of infinite loop
}                                 //end of main()
