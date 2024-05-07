//Spectrogram_rtdx_mtl.c Core program. Time-Frequency analysis Using RTDX-MATLAB
. . . See FFT256c.c
#include <rtdx.h>									//RTDX support file
#include "hamming.cof"       					//Hamming window coefficients
RTDX_CreateOutputChannel(ochan);				//create output channel C6x->PC

main()
{
 //calculate twiddle constants
 comm_intr();			          				//init DSK, codec, McBSP
 while(!RTDX_isOutputEnabled(&ochan))     //wait for PC to enable RTDX
   	puts("\n\n Waiting . . . ");        //while waiting
 while(1)	      								//infinite loop
  {
   . . .
   for (i = 0 ; i < PTS ; i++)    			//swap buffers
    {
      samples[i].real=h[i]*iobuffer[i];  	//multiply by Hamming window coeffs
      iobuffer[i] = x1[i];                //process frame to iobuffer
    }
   . . . use FFT magnitude squared 
   RTDX_write(&ochan, x1, sizeof(x1)/2);	//send 128 samples to PC
  }            									//end of infinite loop
} 														//end of main
interrupt void c_int11()	    				//ISR
{. . . as in FFT256c.c }



