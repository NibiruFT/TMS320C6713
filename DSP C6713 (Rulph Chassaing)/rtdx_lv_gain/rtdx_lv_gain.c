//rtdx_lv_gain.c RTDX using LABVIEW to control gain from DSK of generated sine wave
#include <rtdx.h>                      		     //RTDX support
#include "target.h"                    		     //init target
#define kBUFFER_SIZE 49

RTDX_CreateInputChannel(cinput);                  //create RTDX input channel
RTDX_CreateInputChannel(cgain);                   //input channel for gain value
RTDX_CreateOutputChannel(coutput);                //channel for scaled output

void Gain (int *output,int *input,int gain)		  //scale array of input array
{
 int i;
 for(i=0; i<kBUFFER_SIZE; i++)
    output[i]=input[i]*gain;                      //scaled output
}

void main()
{	
 int input[kBUFFER_SIZE];
 int output[kBUFFER_SIZE];
 int gain = 5;                                    //initial gain setting
	
 TARGET_INITIALIZE();                             //init target for RTDX
 RTDX_enableInput(&cgain);                        //enable RTDX channels
 RTDX_enableInput(&cinput);                       //for input array
 RTDX_enableOutput(&coutput);                     //for output array
 for (;;)													  //infinite loop
  {
   if (!RTDX_channelBusy(&cgain))            	  //if new gain value
     RTDX_readNB(&cgain, &gain, sizeof(gain));    //read it
	while(!RTDX_read(&cinput,input,sizeof(input)));//wait for input waveform
	Gain (output, input, gain);                    //call function to scale
	RTDX_write(&coutput, &output, sizeof(input));  //scaled output from DSK->host
  }
}
