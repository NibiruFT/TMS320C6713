//rtdx_matlabFFT.c RTDX-MATLAB for data transfer from PC->DSK (with loop)

#include "dsk6713_aic23.h"   								//codec-DSK support file
#include <rtdx.h>            								//RTDX support file
Uint32 fs=DSK6713_AIC23_FREQ_16KHZ;                //set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6713_AIC23_INPUT_LINE; // select mic in

RTDX_CreateOutputChannel(ochan); //create out channel for C6x-->PC data transfer

void main()
{
 short i,input_data[256]={0};                      //input array size 256

 comm_poll();													//init DSK,codec,McBSP

 IRQ_globalEnable();		  		//enable global interrupt-RTDX is interrupt-driven
 IRQ_nmiEnable();          								//enable NMI interrupt
 while(!RTDX_isOutputEnabled(&ochan)) 					//wait for PC to enable RTDX
 	   puts("\n\n Waiting... ");					      //while waiting

 while(1)														// infinite loop
 {
  i=0;
  while (i<256)                  						//for 256 samples
    {
     input_data[i] = input_left_sample();					//input from LEFT channel
  	  output_left_sample(input_data[i++]);					//output to LEFT channel
    }
  RTDX_write(&ochan,input_data,sizeof(input_data));//send 256 samples DSK-->PC
 }
}

