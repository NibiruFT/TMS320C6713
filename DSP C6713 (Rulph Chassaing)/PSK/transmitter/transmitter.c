//transmitter.c Transmits voice as a BPSK signal
#include "dsk6713_aic23.h"							//codec-DSK support file
#include <math.h>
#include "lp1500.cof"								//1500 Hz coeff lowpass filter
Uint32 fs=DSK6713_AIC23_FREQ_48KHZ;				//set sampling rate
#define NUMSAMP	4									//# samples per Symbol
#define MAX_DATA_LENGTH 10							//size of Mod/Demod vector
#define NUM_BITS 8									//number of bits per sample
#define SYNC_INTERVAL 100							//interval between sync bits
short encSeqNum	= 8;								//number of encoded bits
short encSymbolVal = 0;								//encoder symbol index
short sin_table[NUMSAMP]={0,1000,0,-1000};	//for carrier
short bits[8];											//holds encoded sample
short sampleBuffer[2000];							//to view sample
short sIndex = 0;										//index sampleBuffer
short syncSequence[8]={1,1,1,-1,1,-1,-1,1};	//syncronization sequence
short outval=1;										//bit value to be encoded
short encodeVal = 0;									//filtered input value
int yn = 0;				   							//init filter's output
short gain=10;											//gain on output
short syncTimer = 0;									//tracks time between syncs
#define LEFT  0										//setup left/right channel
#define RIGHT 1
union {Uint32 uint;	short channel[2];} AIC23_data;

interrupt void c_int11()            			//interrupt service routine
{
 int i;
 short sample_data;
 sample_data = input_sample();
 yn = fircircfunc(sample_data,h,N); 		 	//asm func passing to A4,B4,A6
 if(encSymbolVal >= NUMSAMP)						//increment through waveform
 {
  encSymbolVal = 0;
  encSeqNum++;
 }
 if(encSeqNum == NUM_BITS)							//when all 8 bits sent
 {   														//get a new sample
  encSeqNum = 0;
  if(syncTimer++ >= SYNC_INTERVAL) 				//determine whether
  {								 						//to send sync sequence
   syncTimer = 0;
   for(i=0; i<8; i++)			 					//put sync sequence in bit
      bits[i] = syncSequence[i];
  }
  else
  {	//get the bits
   encodeVal = (short) (yn >> 15);
   for(i=8; i<16; i++)								//encode input sequence
      bits[i-8]=(encodeVal & (1<<i)) ? 1 : -1; //shift
  }
  sampleBuffer[sIndex++] = encodeVal;
  if(sIndex >= 2000) 	sIndex = 0;
 }
 outval = (short) bits[encSeqNum];
 AIC23_data.channel[RIGHT]=gain*sin_table[encSymbolVal];//carrier->Right Channel
 AIC23_data.channel[LEFT]=gain*outval*sin_table[encSymbolVal++];//data->Left
 output_sample(AIC23_data.uint); 				//output to both channels
}
void main(){
 comm_intr();   
 while(1); }        				//init,infinite loop



 
