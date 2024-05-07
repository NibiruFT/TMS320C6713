//DTMF_Bios_FFT.c Partial prog uses LED to detect DTMF signals using radix-4 FFT
. . .                            //see radix-4 example in Chapter 6
short input_buffer[N] = {0};		//to store input samples...same as x
float output_buffer[7] = {0};		//to store magnitude of FFT
short buffer_count, i, J;
short nFlag;							//indicator to begin FFT
short nRow, nColumn;
double delta;
float tempvalue;

interrupt void c_int11()
{
 input_buffer[buffer_count] = input_sample();
 output_sample((short)input_buffer[buffer_count++]);
 if(buffer_count >= N)           //if accum more than N points--> begin FFT
  {
   buffer_count = 0;					// reset buffer_count
	nFlag = 0;							// flag to signal completion
   for(i = 0; i < N; i++)
	 {
	  x[2*i] = (float)input_buffer[i]; // real part of input
	  x[2*i+1] = 0;					// imaginary part of input
	 }
  }
}

void main(void)
{
 nFlag = 1;
 buffer_count = 0;
 //generate twiddle constants, then index for digit reversal
 comm_intr();
 while(1)                        //infinite loop
 {
  	while(nFlag); 						//wait for ISR to finish buffer accumm samples
  	nFlag = 1;
   //call radix-4 FFT, then digit reverse function
   output_buffer[0]=(float) sqrt(x[2*22]*x[2*22] + x[2*22+1]*x[2*22+1]);
   . . . //for weigths 25,28,31,39,43
   output_buffer[6] = (float) sqrt(x[2*47]*x[2*47] + x[2*47+1]*x[2*47+1]);
   tempvalue = 0;            					//choose largest row frequency
	nRow = 0;
	for(j = 0; j < 4; j++)
	 {
	  if(tempvalue < output_buffer[j])
	  	{
	    if(output_buffer[j] > 0.5e4)
		  {
		  	nRow = j + 1;
		  	tempvalue = output_buffer[j];
		  }
		}
	 }	//end of for loop
   tempvalue = 0;                       //choose largest column frequency
	nColumn = 0;
	for(j = 4; j < 7; j++)
	 . . . //as for the rows
	  	  	nColumn = j - 3;
	 . . .//as for the rows
    } //end of for loop
   if((nRow != 0) && (nColumn != 0))
   {
	 if((nRow==1)&&(nColumn==1))        //button 0001 ("1")
	 {DSK6713_LED_off(0);DSK6713_LED_off(1);DSK6713_LED_off(2);DSK6713_LED_on(3);}
	 if((nRow==1)&&(nColumn==2))     	//button 0010
	 {DSK6713_LED_off(0);DSK6713_LED_off(1);DSK6713_LED_on(2);DSK6713_LED_off(3);}
	 //for button "3", "4", ..., "#"
	}
	else
	{DSK6713_LED_off(0);DSK6713_LED_off(1);DSK6713_LED_off(2);DSK6713_LED_off(3);}
 };   //end of while (1) infinite loop
}	   //end of main
