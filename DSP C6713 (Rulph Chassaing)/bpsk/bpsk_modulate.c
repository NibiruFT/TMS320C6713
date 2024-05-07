//BPSK_modulate.c Core program for BPSK modulation
.
.
short cos_table[4] = {1000,0,-1000,0};

interrupt void c_int11()
{
	input_data  = ((short)input_sample());
  	if(input_data>0) bpsk_signal = cos_table[i++];
	else bpsk_signal = -1*cos_table[i++];
   output_sample(bpsk_signal);
	if(i > 3) i=0;
}
void main() {
	comm_intr();
	while(1);
}
