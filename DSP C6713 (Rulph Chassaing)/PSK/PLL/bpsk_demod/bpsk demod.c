//bpsk_demod.c Input from 1st DSK running phase-shifted sine

#include "dsk6713_aic23.h"			//codec-DSK support file
#include <math.h>
Uint32 fs=DSK6713_AIC23_FREQ_16KHZ;		//set sampling rate
#define NUMSAMP 16			// # Samples per Symbol
#define PI 3.1415926
short sample_data;			// input sample
short r[10000]={0};			// buffer to view recieved data
short ri=0;					// received buffer index
short r_symbol[NUMSAMP];	// buffer to received one period of signal
short SBind=0;				// symbol buffer index
float phiBuf[1000]={0};		// buffer to view phi estimates
short phiBind=0;			// phi buffer index
float y1, y2;				// correlation vectors
float phi=PI;				// phase estimate
float damp=1;				// damping factor

interrupt void c_int11()            //interrupt service routine
{
 int i, max=1;
 sample_data = (short) input_sample(); //receive sample
 r[ri++] = sample_data;				 
 r_symbol[SBind++] = sample_data;	// put sample in symbol buffer
 if(ri >= 10000)	  ri=0;					// reset buffer index
 if(SBind == NUMSAMP)				// after one period is received
 {									// then perform phi estimate
  SBind = 0;						// reset buffer index
  y1 = 0;
  y2 = 0;
  for(i=0; i<NUMSAMP; i++)		// correlate received symbol
   {
    y1 += r_symbol[i]*sin(2*PI*i/NUMSAMP + phi - 0.1);
    y2 += r_symbol[i]*sin(2*PI*i/NUMSAMP + phi + 0.1);
    if(r_symbol[i] > max) 	max=r_symbol[i];
   }		
  y1 = y1 / max;					// normalize correlation coefs
  y2 = y2 / max;
  phi = phi + 0.4*(y2 - y1);	// determine new estimate for phi
  if(phi < 1)	  phi=phi+2*PI; // normalize phi
  if(phi > (2*PI+1))  phi=phi-2*PI;
  phiBuf[phiBind++]=phi;			// put phi in buffer for viewing
  if(phiBind >= 1000)	 phiBind = 0;// resent buffer index
 }
 output_sample(phi);	
}

void main()
{
  comm_intr();  while(1);                	//init DSK, codec, McBSP
                     	//infinite loop
}


 
