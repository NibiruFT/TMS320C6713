//BPSK_sim.c BPSK simulation Mod/Demod. DSK Output sequence-->Input
#include "dsk6713_aic23.h"					//codec-DSK support file
#include <math.h>
#include <stdio.h>
Uint32 fs=DSK6713_AIC23_FREQ_16KHZ;		//set sampling rate
#define PI 3.1415926
#define N 16									//# samples per symbol
#define MAX_DATA_LENGTH 10					//size of mod/demod vector
#define STABILIZE_LEN 10000				//# samples for stabilization
float phi_1[N];								//basis function 1
short r[N] = {0};								//received signal
int rNum=0,   beginDemod=0;			   //# of received samples/demod flag
short encSeqNum=0,  decSeqNum=0;			//# encoded/decoded bits
short encSymbolVal=0,decSymbolVal=0;	//encoder/decoder Symbol Index
short encodeSeq[MAX_DATA_LENGTH]={1,0,1,1,0,0,0,1,0,1};//encoded sequence
short decodeSeq[MAX_DATA_LENGTH];		//decoded sequence
short sigAmp[2] = {-10000, 10000};  	//signal amplitude
short buffer[N*(MAX_DATA_LENGTH+3)];	//received vector for debugging
short buflen=0,   stabilizeOutput=0;
interrupt void c_int11()            	//interrupt service routine
{
 int i,  outval= 0;
 short X = 0;
 if(stabilizeOutput++ < STABILIZE_LEN) //delay start to Stabalize
 {
  r[0] = input_sample();
  output_sample(0);
  return;
  }
 if(encSeqNum < MAX_DATA_LENGTH) 		//modulate data sequence
 {
  outval = (int) sigAmp[encodeSeq[encSeqNum]]*phi_1[encSymbolVal++];
  if(encSymbolVal>=N) {encSeqNum++;   encSymbolVal=0; }
  output_sample(outval);
 }
 else	output_sample(0);						//0if MAX_DATA_LENGTH exceeded
 r[rNum++] = (short) input_sample(); 	//input signal
 buffer[buflen++] = r[rNum - 1];
 if(beginDemod)				    			//demod received signal
 {
  if(decSeqNum<2 && rNum==N)  {			//account for delay in signal
		decSeqNum ++;   rNum = 0; }
  if(rNum == N)								//synchronize to symbol length
  {
   rNum = 0;
   for(i=0; i<N; i++)						//correlate with basis function
		X += r[i]*phi_1[i];
	decodeSeq[decSeqNum-2] = (X >= 0) ? 1: 0;	//do detection
	if(++decSeqNum == MAX_DATA_LENGTH+2)		//print received sequence
	{
	 for(i=0; i<decSeqNum-2; i++)
		printf("Received Value: %d\n", decodeSeq[i]);
	 exit(0);
	}
  }
 }
 else  { beginDemod = 1; rNum = 0; }
}
void main()
{
	int i;
  	comm_intr();                  		//init DSK, codec, McBSP
  	for(i=0; i<=N; i++)
		phi_1[i] = sin(2*PI*i/N);			//basis function
	while(1);                     		//infinite loop
}


 
