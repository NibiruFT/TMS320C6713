//rtdx_vc_FIR.c FIR with RTDX using VC++ to transfer coefficients file
#include "dsk6713_aic23.h"
#include <rtdx.h>
#define RTDX_BUFFER_SIZE 256         		//change for higher filter order
Uint32 fs = DSK6713_AIC23_FREQ_8KHZ;
RTDX_CreateInputChannel(control_channel);	//create input channel
short* pFir;                           	//->filter's Impulse response
short RtdxBuffer[RTDX_BUFFER_SIZE]={0};	//buffer for RTDX
short dly[RTDX_BUFFER_SIZE] = {0};     	//buffer for input samples
short i;
short N;                               	//order of filter
int yn;

interrupt void c_int11()
{
	dly[0] = input_sample();
	yn = 0;
   for(i = 0; i < N; i++)
		yn += pFir[i]*dly[i];
	for(i = N - 1; i > 0; i--)
		dly[i] = dly[i-1];
	output_sample(yn >> 15);
}

void main()
{
   N = 0;                                  //initial filter order
   pFir = &RtdxBuffer[1];             		 //-> 2nd element in RTDX buffer
   comm_intr();
   RTDX_enableInput(&control_channel);		 //enable RTDX input channel
   while(1)                            	 //infinite loop
	{
   if(!RTDX_channelBusy(&control_channel)) //if free, read->buffer
    {                                      //read N and coefficients
     RTDX_read(&control_channel,&RtdxBuffer,sizeof(RtdxBuffer));
     N = RtdxBuffer[0];                	 //extract filter order
    }
   }
}
