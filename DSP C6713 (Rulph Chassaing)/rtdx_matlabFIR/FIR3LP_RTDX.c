//FIR3LP_RTDX.c FIR of 3 LP with different BWs using RTDX with MATLAB

#include "lp600.cof"		 	    //coeff file LP @ 600 Hz 
#include <rtdx.h>
#include <stdio.h>
#include "target.h"
int yn = 0;				 //initialize filter's output
short dly[N];            //delay samples
short h[N]; 			 //filter characteristics 1xN 
short loop = 0;
short sine_table[32] = {0,195,383,556,707,831,924,981,1000,
						981,924,831,707,556,383,195,0,
					   -195,-383,-556,-707,-831,-924,-981,-1000,
					   -981,-924,-831,-707,-556,-383,-195};
short amplitude = 10;
#define BUFFER_SIZE 256
int buffer[BUFFER_SIZE];
int inputsample;
int outputsample;
short j = 0;

RTDX_CreateInputChannel(ichan);   		//create input channel
RTDX_CreateOutputChannel(ochan);   		//create output channel

void main()
{
 short i;

 TARGET_INITIALIZE();
 RTDX_enableInput(&ichan); 		//enable RTDX channel
 RTDX_enableOutput(&ochan); 		//enable RTDX channel
 for (i=0; i<N; i++)
  { 
	dly[i] = 0;                 //init buffer
	h[i] = hlp600[i];  	    	//start addr of LP600 coeff
  }
 while(1)     		   	    	//infinite loop
  {
   inputsample=rand()+amplitude*(sine_table[loop]);//gen input
   if (loop < 31) ++loop;
   else loop = 0;
   //FIR filter section
   dly[0] = inputsample;     	 	//newest input @ top of buffer
   yn = 0;                         //initialize filter output
   if (!RTDX_channelBusy(&ichan))  {
		RTDX_readNB(&ichan,&h[0],N*sizeof(short));}
   for (i = 0; i< N; i++)
	 	yn +=(h[i]*dly[i]);			//y(n) += h(LP#,i)*x(n-i)
   for (i = N-1; i > 0; i--)       //starting @ bottom of buffer
	   	dly[i] = dly[i-1];          //update delays with data move
   outputsample = (yn >> 15);	    //output filter
   buffer[j] = outputsample;	//store output->buffer
   j++;
   if (j==BUFFER_SIZE) {
    	j = 0;
	    while (RTDX_writing != NULL) {}  //waiting for rtdx write to complete
		RTDX_write( &ochan, &buffer[0], BUFFER_SIZE*sizeof(int) );
   }
    
  }
}

