//BPSK.c Real IN
#include "dsk6713_aic23.h"				//codec-DSK support file
#include <math.h>
Uint32 fs=DSK6713_AIC23_FREQ_32KHZ;	//set sampling rate
#define NUMSAMP 4						//# samples per symbol
#define MAX_DATA_LENGTH 10				//size of mod/demod vector
short encSeqNum=0, encSymbolVal=0; 		//# encoded bits/symbol index
short sin_table[NUMSAMP]={0,10000,0,-10000};
short sample_data;  short bits[16]={0};  short outval=1;

interrupt void c_int11()            //interrupt service routine
{
 int i;
 short j=0;
 sample_data=(short)input_sample(); //input sample
 if(encSeqNum == 32)						//decimate 32kHz to 1kHZ
 {
  encSeqNum = 0;
  if((sample_data>1000) || (sample_data<-1000)) {//if above noise threshold
  for(i=0;i<8;i++)  bits[i]=(sample_data&(1<<i))?1:-1;} //8 significant bits
  else {for(i=0;i<8;i++) bits[i]=0;} //get next bit
 }
 outval = (short) bits[j];
 output_sample(outval*sin_table[encSymbolVal++]);//output next sample
 if(encSymbolVal>=NUMSAMP) {encSymbolVal=0;	j++;} //reset encSymbolVal
 encSeqNum++;
 if (j==8)   j=0;							//start next sample
}
void main()
{comm_intr();     while(1);}			//init DSK/infinite loop



 
