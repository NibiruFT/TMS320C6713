//FFTr2.c FFT using TI's optimized FFT function and real-time input

#include "dsk6713_aic23.h"
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ; //set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // select line in
#include <math.h>
#define N 256				    	//number of FFT points 
#define RADIX 2				    	//radix or base  
#define DELTA (2*PI)/N			    //argument for sine/cosine 
#define PI 3.14159265358979
short i = 0;										
short iTwid[N/2];					//index for twiddle constants W 
short iData[N];						//index for bitrev X 
float Xmag[N];						//magnitude spectrum of x
typedef struct Complex_tag {float re,im;}Complex;  
Complex W[N/RADIX];			    	//array for twiddle constants
Complex x[N]; 				    	//N complex data values 
#pragma DATA_ALIGN(W,sizeof(Complex))   //align W on boundary 
#pragma DATA_ALIGN(x,sizeof(Complex))	//align input x on boundary

void main() 
{
 for( i = 0 ; i < N/RADIX ; i++ ) 
  {
   W[i].re = cos(DELTA*i);			//real component of W
   W[i].im = sin(DELTA*i);			//neg imag component
  }						    		//see cfftr2_dit
 digitrev_index(iTwid, N/RADIX, RADIX);//produces index for bitrev() W 
 bitrev(W, iTwid, N/RADIX);		       //bit reverse W 
 
 comm_poll();				    	//init DSK,codec,McBSP
 for(i=0; i<N; i++)
 	Xmag[i] = 0;					//init output magnitude
 while (1) 							//infinite loop
 {   				    	       
  for( i = 0 ; i < N ; i++ ) 
   {
    x[i].re = (float)((short)input_left_sample()); //external input
    x[i].im = 0.0 ;				    //zero imaginary part                  
    if(i==0)   output_sample(32000);				//negative spike for reference
    else
    	output_left_sample((short)Xmag[i]);	//output magnitude
   } 
  
  cfftr2_dit(x, W, N ) ;			    	//TI floating-pt complex FFT 
  digitrev_index(iData, N, RADIX);		//produces index for bitrev() X 
  bitrev(x, iData, N);			    	//freq scrambled->bit-reverse x
  for (i =0; i<N; i++)
    Xmag[i] = -sqrt(x[i].re*x[i].re+x[i].im*x[i].im)/32; //magnitude of X
 }
}


