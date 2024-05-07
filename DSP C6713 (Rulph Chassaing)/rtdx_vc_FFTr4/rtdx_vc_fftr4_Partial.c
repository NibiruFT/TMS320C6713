//rtdx_vc_FFTr4.c FFT-4 using RTDX with VC++ (MatLAB for plotting)
. . .     See Examples on radix-4 FFT in Chapter 6 (.out file on CD)
#include <rtdx.h>
short input_buffer[N] = {0};						//store input samples(same as x)
float output_buffer[N] = {0};						//store magnitude FFT
int count; 												//used in digit reversal
short buffer_count=0;
short nFlag=1;											//when to begin the FFT
short i, j;
RTDX_CreateOutputChannel(ochan); 				//output channel C6x-->PC transfer
interrupt void c_int11()							//ISR
{
 input_buffer[buffer_count] = input_sample();//input -->buffer
 output_sample(input_buffer[buffer_count++]);//loop
 if(buffer_count >= N)
  {														//if more than N points,begin FFT
   buffer_count = 0;									//reset buffer_count
   nFlag = 0;											//flag to signal completion
   for(i = 0; i < N; i++)
	{
    x[2*i]=(float)input_buffer[i]; 				//real component of input
	 x[2*i+1] = 0;										//imaginary component of input
	}
  }
}
void main(void)
{
  . . .
  comm_intr();  										//init DSK
  while(!RTDX_isOutputEnabled(&ochan));		//wait for PC to enable RTDX
  while(1)												//infinite loop
   {
    while(nFlag); 									//wait/intr to finish accum samples
    nFlag = 1;
	 cfftr4_dif(x, w, N);							//call radix-4 FFT function
    digit_reverse((double *)x,IIndex,JIndex,count);
    for(j = 0; j < N; j++)
       output_buffer[j]=(float)sqrt(x[2*j]*x[2*j]+x[2*j+1]*x[2*j+1]);
    RTDX_write(&ochan, output_buffer,sizeof(output_buffer)); //Send DSK-->PC
   };
}
