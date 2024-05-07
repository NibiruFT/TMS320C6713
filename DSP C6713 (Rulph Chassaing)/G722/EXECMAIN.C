/**********************************************************************
execmain.c - Main executive routine of the G.722 audio codec.
***********************************************************************/
int encoder_on = 0;
int start_buffer = 0;
#define buffer_size  100
int Encoder_buff[buffer_size];
int slout_buff[buffer_size];
int xlout_buff[buffer_size];
int Input_buff[buffer_size];
int Output_buff[buffer_size];
int m=0;
int n=0;
int o=0;
int p=0;

    extern int encode(int,int);
    extern void decode(int);
    extern void reset();
    extern int xout1,xout2,slout,xlout;
    int prev_samp = 0;
    int flag=0;
    int t2, output;

interrupt void c_int11()          /*interrupt service routine*/
{
	int t1;
	int I;
	t2 = input_sample();  /*input data*/
	if(encoder_on==1)
	{
		t1 = 0.5*(t2 + prev_samp);
		prev_samp = t2;
		I=encode(t1,t2);
   	    decode(I);
	    output = 0.5*(xout1+xout2);
	    output_sample(output);

		if((m<buffer_size-1)&&(start_buffer == 1))
		{
		      Input_buff[m++]=t1;
		      Input_buff[m++]=t2;
		      Encoder_buff[n++]=I;
		      Output_buff[o++]=xout1;
	      	  Output_buff[o++]=xout2;
	      	  slout_buff[p]=slout;
	      	  xlout_buff[p++]=xlout;
		}
		else if((m!=0)&&(start_buffer == 0))
		{
			for(m=0 ; m<buffer_size; m++)
			{
				Input_buff[m] = 0;
				Encoder_buff[m] = 0;
				Output_buff[m] = 0;
				slout_buff[m] = 0;
				xlout_buff[m] = 0;
			}
			m = 0;
			n = 0;
			o = 0;
			p = 0;
		}
	}
	else
	{
		output = t2 & 0xFFFFFF00;
		output_sample(output);
	}
	return;
}

void main()
{
	comm_intr();                    /*init DSK, codec, McBSP*/
    reset();
	while(1);                		 /*infinite loop*/
}

