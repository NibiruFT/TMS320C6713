//rtdx_lv_filter.c RTDX using LABVIEW for filter design and plotting DSK output
#include <rtdx.h>                     			//RTDX support
#include "target.h"                    		//init target
#define kBUFFER_SIZE 48                      //RTDX read/write buffers
#define kTAPS 51
double gFIRHistory [kTAPS+1];
double gFIRCoefficients [kTAPS];
int input[kBUFFER_SIZE], output[kBUFFER_SIZE];
int gain;
double FIRFilter (double val, int nTaps, double* history, double* coefs);
int  ProcessData (int* output, int* input, int gain);

RTDX_CreateInputChannel(cinput);          	//create RTDX input data channel
RTDX_CreateInputChannel(ccoefs);          	//input channel for coefficients
RTDX_CreateOutputChannel(coutput);        	//output channel DSK-->PC(Labview)

void main()
{
 int i;
 TARGET_INITIALIZE();                    		//init target for RTDX
 RTDX_enableInput(&cinput);                	//enable RTDX channels
 RTDX_enableInput(&ccoefs);
 RTDX_enableOutput(&coutput);
 gFIRCoefficients[0] = 1.0;
 for (i = 1; i<kTAPS; i++)
	 gFIRCoefficients[i] = 0.0;
 for (;;)												//infinite loop
  {
   while(!RTDX_read(&cinput, input, sizeof(input)));//wait for new input buffer
   if (!RTDX_channelBusy(&ccoefs))           //if new set of coefficients
      RTDX_readNB(&ccoefs,&gFIRCoefficients,sizeof(gFIRCoefficients));//read it
   ProcessData (output, input, 1);           //filtering on DSK
   RTDX_write(&coutput, &output, sizeof(output));//output from DSK->LABVIEW
  }
}
int ProcessData (int *output,int *input,int gain)//scale waveform/calls filter
{
 int i;
 double filtered;
 for(i=0; i<kBUFFER_SIZE; i++) {
   filtered = FIRFilter(input[i]*gain,kTAPS,gFIRHistory,gFIRCoefficients);
   output[i] = (int)(filtered + 0.5);} 		//scale output
 return 0;
}

double FIRFilter (double val,int nTaps,double* history,double* coefs)//FIR
{
 double temp, filtered_val, hist_elt;
 int	i;
 hist_elt = val;
 filtered_val = 0.0;
 for (i = 0; i<nTaps; i++)
  {
   temp = history[i];
  	filtered_val += hist_elt * coefs[i];
   history[i] = hist_elt;
   hist_elt = temp;
  }
 return filtered_val;
}




