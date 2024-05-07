//graphic_FFT.c Partial Program for display of spectrum through EMIF using LEDs

#include "output.h"                 //contains EMIF address
int *output = (int *)OUTPUT;        //EMIF address included in header file

 while (1)   								//infinite loop
 {
  .
  .                                 //same as in FFTr2.c
  .
  for(i = 0; i < N/2; i++)
  {
   	if (Xmag[i] > 20000.0)			//if mag FFT >20000
	{
	 out=out+1<<i;							//shifts one to apropriate bit location
	}
  }
  *output = out;							//output to EMIF bus
  out = 0;									//reset out variable for next iteration
 }



