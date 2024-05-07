//FFTsinetable.c FFT{sine}from table. Calls TI float-point FFT function 

#include "dsk6713_aic23.h"		//codec-DSK support file
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ; //set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // select line in
#include <math.h>
#define N 32				    //number of FFT points 
#define FREQ 8	                	    //select # of points/cycle
#define RADIX 2				    //radix or base  
#define DELTA (2*PI)/N			    //argument for sine/cosine 
#define TAB_PTS 32			    //# of points in sine_table  
#define PI 3.14159265358979
short i = 0;										
short iTwid[N/2];			    //index for twiddle constants 
short iData[N];				    //index for bitrev X 
float Xmag[N];				    //magnitude spectrum of x
typedef struct Complex_tag {float re,im;}Complex;  
Complex W[N/RADIX];			    //array for twiddle constants
Complex x[N]; 				    //N complex data values 
#pragma DATA_ALIGN(W,sizeof(Complex))   //align W 
#pragma DATA_ALIGN(x,sizeof(Complex))	//align x

short sine_table[TAB_PTS] = {0,195,383,556,707,831,924,981,1000,
981,924,831,707,556,383,195,-0,-195,-383,-556,-707,-831,-924,-981,
-1000,-981,-924,-831,-707,-556,-383,-195};

void main()
{
 for( i = 0 ; i < N/RADIX ; i++ ) 
  {
   W[i].re = cos(DELTA*i);		    //real component of W
   W[i].im = sin(DELTA*i);		    //neg imag component
  }						    //see cfftr2_dit
 for( i = 0 ; i < N ; i++ ) 
  {		
   x[i].re=sine_table[FREQ*i % TAB_PTS]; //wrap when i=TAB_PTS
   x[i].im = 0 ;				    //zero imaginary part 
  } 
 digitrev_index(iTwid, N/RADIX, RADIX);//produces index for bitrev() W 
 bitrev(W, iTwid, N/RADIX);		    //bit reverse W 
 cfftr2_dit(x, W, N ) ;			    //TI floating-pt complex FFT 
 
 digitrev_index(iData, N, RADIX);	    //produces index for bitrev() X 
 bitrev(x, iData, N);			    //freq scrambled->bit-reverse X 
 for(i = 0 ; i < N ; i++ ) 
    Xmag[i] = -sqrt(x[i].re*x[i].re+x[i].im*x[i].im ); //magnitude of X

 comm_poll( ) ;				    //init DSK,codec,McBSP 
 while (1)   				    //infinite loop
  {      
   output_left_sample(32000) ;		    //negative spike as reference 	
   for (i = 1; i < N; i++)
      output_left_sample((short)Xmag[i]);      //output magnitude samples                 
  }
}

