//IIR_recov.c Voice Descrambler program. Output from IIR_DIP with option 5
//becomes input to board running descrambler

#include "DSK6713_AIC23.h"
#include "lp4000.cof"
Uint32 fs=DSK6713_AIC23_FREQ_16KHZ;
short out_samp=0;		// alternates 1 and 0 each sample
short buffer[512];		// contains valid input data
short BUFFER_SIZE = 512;	// indicates buffer size
short index = 0;		// index for inputting data into buffer
short zero_count = 0;		// holds the number of consecutive '0' inputs
short output_index = 0;		// index for outputting data from buffer
short dly[stages][2] = {0};	// for the IIR filter

interrupt void c_int11()
{
 short i = 0,input, output;
 int un, yn;

 input = (short)input_sample();	// input data
 // we need 512 samples to be close to '0'
 // then we can be sure the following 512 samples are valid
 if (zero_count < (BUFFER_SIZE)) { // if we are waiting for more '0' inputs
   if (input < 4000) {		// see if current input could be a '0'
     zero_count++;		// if so increment count
   } else {
     zero_count = 0;		// if not, reset count
   }
 } else {			// otherwise, data is valid
   buffer[index++] = input;	// add sample to input buffer
   if (index == BUFFER_SIZE) {	// if buffer is full
     index = 0;			// reset index
     zero_count = 0;		// and start looking for another 512 '0's
   }
 }
 // output every other iteration of this ISR
 // out_samp alternates
 if (out_samp == 1) {			// if supposed to output this time,
   output = buffer[output_index++];	// output next value
   out_samp = 0;			// and don't output next time.
   if (output_index == BUFFER_SIZE) output_index = 0;
 } else {				// if not supposed to output,
   out_samp = 1;			// do output next time
 }
 // IIR filter - lowpass at 4 kHz
 for (i = 0; i < stages; i++) {
   un=output-((b[i][0]*dly[i][0])>>15) - ((b[i][1]*dly[i][1])>>15);
   yn=((a[i][0]*un)>>15)+((a[i][1]*dly[i][0])>>15)+((a[i][2]*dly[i][1])>>15);
   dly[i][1] = dly[i][0];
   dly[i][0] = un;
   input = yn;
 }
 output_sample((short)(yn));
 return;
}

void main()
{
  comm_intr();
  while(1);
}
