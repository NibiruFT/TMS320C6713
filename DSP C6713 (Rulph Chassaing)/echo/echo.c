//echo.c echo with fixed delay and feedback
#include "DSK6713_AIC23.h"	                // codec support
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;	        // set sampling rate
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DSK6713_AIC23_INPUT_LINE 0x0011
Uint16 inputsource=DSK6713_AIC23_INPUT_MIC; // select input

#define GAIN 0.6                // fraction (0 - 1) of output fed back
#define BUF_SIZE 2000           // this sets length of delay
short buffer[BUF_SIZE];         // storage for previous samples
short input,output,delayed;
int i;                          // index into buffer

interrupt void c_int11()	    // interrupt service routine
{
  input = input_left_sample();  // read new input sample from ADC 
  delayed = buffer[i];          // read delayed value from buffer
  output = input + delayed;     // output sum of input and delayed values
  output_left_sample(output);     
  buffer[i] = input + delayed*GAIN; // store new input and a fraction 
                                // of the delayed value in buffer
  if(++i >= BUF_SIZE) i=0;      // test for end of buffer
  return;                       // return from ISR
}

void main()
{
  comm_intr();                  // init DSK, codec, McBSP
  for(i=0 ; i<BUF_SIZE ; i++)   // clear buffer
    buffer[i] = 0;
  while(1);                     //infinite loop
}

