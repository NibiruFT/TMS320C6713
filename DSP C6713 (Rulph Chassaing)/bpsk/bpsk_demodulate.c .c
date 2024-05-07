//BPSK_demodulate.c Partial program for BPSK demodulation
.
.
double mixer_out, pd;

interrupt void c_int11()
{		
	input_signal=((short)input_sample()/10);
	mixer_out = input_signal*input_signal;
   dly[0] = mixer_out;
   .
	.
   filter_output= (yn >> 15);   		//output of 16 kHz BP filter
   x = 0;                       		//init downsampled value
 	if (flag == 0)               		//discard input sample value
    	flag = 1;                		//don't discard at next sampling
 	else {                         
   	x = filter_output;       		//downsampled value is input value
   	flag = 0;
 	}
   pd = x * input_signal;           //product detector
  	dly2[0] = ((short)pd);         	//for 4 kHz LP filter
   .
	.
   m = (yn2 >> 15);             		//output of LP filter
	output_sample(m);
   return;
}

void main() {
	comm_intr();
	while(1);
}
