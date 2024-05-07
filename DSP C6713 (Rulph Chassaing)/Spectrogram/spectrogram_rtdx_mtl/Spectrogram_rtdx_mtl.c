//Spectrogram_rtdx_mtl.c Time-Frequency analysis of signals Using RTDX-MATLAB
#include "dsk6713_aic23.h"						//codec-DSK support file
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;			//set sampling rate
#include <rtdx.h>									//RTDX support file
#include <math.h>
#include "hamming.cof"       					//Hamming window coefficients
#define PTS 256			     					//# of points for FFT
#define PI 3.14159265358979
typedef struct {float real,imag;} COMPLEX;
void FFT(COMPLEX *Y, int n);	   			//FFT prototype
float iobuffer[PTS];   		       			//input and output buffer
float x1[PTS];         		    			   //intermediate buffer
short i;   					       				//general purpose index variable
int j,  curr_block = 0;                	//index variables
short buffercount = 0;     	       		//number of new samples in iobuffer
short flag = 0;        		       			//set to 1 by ISR when iobuffer full
COMPLEX w[PTS];       		       			//twiddle constants stored in w
COMPLEX samples[PTS];  		      			//primary working buffer
RTDX_CreateOutputChannel(ochan);				//create output channel C6x->PC

main()
{
 for (i = 0 ; i<PTS ; i++)	      			//set up twiddle constants in w
  {
   w[i].real = cos(2*PI*i/512.0); 			//Re component of twiddle constants
   w[i].imag =-sin(2*PI*i/512.0); 			//Im component of twiddle constants
  }
 comm_intr();			          				//init DSK, codec, McBSP

 while(!RTDX_isOutputEnabled(&ochan))      //wait for PC to enable RTDX
 	puts("\n\n Waiting . . . ");           //while waiting

 while(1)									//infinite loop
  {
   while (flag == 0) ;            			//wait until iobuffer is full
   flag = 0;                      			//reset flag
   for (i = 0 ; i < PTS ; i++)    			//swap buffers
    {
      samples[i].real=h[i]*iobuffer[i];  	//multiply by Hamming window coeffs
      iobuffer[i] = x1[i];                //process frame to iobuffer
    }
   for (i = 0 ; i < PTS ; i++)
      samples[i].imag = 0.0;	            //imag components = 0
   FFT(samples,PTS);                      //call C-coded FFT function
   for (i = 0 ; i < PTS ; i++)      		//compute square of FFT magnitude
    {
     	x1[i] = (samples[i].real*samples[i].real
	     	   + samples[i].imag*samples[i].imag)/16;  //FFT data scaling
    }
   RTDX_write(&ochan, x1, sizeof(x1)/2); //send 128 samples to PC
  }            							//end of infinite loop
} 										//end of main

interrupt void c_int11()	    				//ISR
{
  output_sample((short)(iobuffer[buffercount]));   //out from iobuffer
  iobuffer[buffercount++]=(short)(input_sample()); //input to iobuffer
  if (buffercount >= PTS)				   				//if iobuffer full
  {
	buffercount = 0;					   					//reinit buffercount
	flag = 1;						   						//reset flag
  }
}



