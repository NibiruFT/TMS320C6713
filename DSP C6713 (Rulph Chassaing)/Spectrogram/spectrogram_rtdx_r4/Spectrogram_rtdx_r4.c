//spectrogram_rtdx_r4.c Time-Frequency analysis of signals with spectrogram
#include "dsk6713_aic23.h"
#include <rtdx.h>
#include <math.h>
#include "hamming.cof"
#define N 256  		           		//# of complex FFT points
#define II 3.14159265359			//define pi
#pragma DATA_ALIGN(x, 8);      		//double-word aligned
#pragma DATA_ALIGN(w, 8);
Uint32 fs = DSK6713_AIC23_FREQ_8KHZ;
float x[2*N];               		//real/img consecutively aligned
float w[3*N/2];                    	//complex twiddle factors
short input_buffer[N] = {0};		//to store input samples..same as x
float output_buffer[N] = {0};		//to store magnitude of FFT
int count; 							//used in digit reversal
short buffer_count = 0;
short nFlag = 1;						//when to begin the FFT
short i, j;
double delta = 2*II/N;
unsigned short JIndex[4*N];
unsigned short IIndex[4*N];
RTDX_CreateOutputChannel(ochan); 	//out channel for C6x->PC data transfer

interrupt void c_int11()			//ISR
{
 input_buffer[buffer_count] = input_sample();
 output_sample((int)input_buffer[buffer_count++]);
 if(buffer_count >= N) //if accum more than N points
  {
   buffer_count = 0;					//reset buffer_count
   nFlag = 0;							//flag to signal completion
   for(i = 0; i < N; i++)
	{
     x[2*i] = h[i]*input_buffer[i]; // real part of input
	 x[2*i+1] = 0;					// imaginary part of input
	}
  }
}

void main(void)
{
 for(j = 0; j < 3*N/4; j++)	 //generate twiddle constants
  {
   w[2*j+1] = sin(j*delta);
   w[2*j] = cos(j*delta);
  }
 R4DigitRevIndexTableGen(N,&count,IIndex,JIndex);//index for digit reversal
 comm_intr();				//init DSK
 while(!RTDX_isOutputEnabled(&ochan));//wait to enable RTDX
 while(1)			//infinite loop
  {
   while(nFlag); 	//wait for ISR to finish accum samples
   nFlag = 1;
   cfftr4_dif(x, w, N);	//TI ASM FFT function
   digit_reverse((double *)x,IIndex,JIndex,count);//digit reversal
   for(j = 0; j < N; j++)
	 output_buffer[j]= sqrt(x[2*j]*x[2*j]+x[2*j+1]*x[2*j+1]);
   RTDX_write(&ochan,output_buffer,sizeof(output_buffer)/2);//DSK->PC
  };
}
