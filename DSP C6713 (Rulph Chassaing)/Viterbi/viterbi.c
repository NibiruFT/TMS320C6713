//***********************************************************
// Convolutional Encoder 
// Submitted By: Dalys Sebastian, Aditya Nadkarni
//***********************************************************

#include "dsk6713_aic23.h"		//codec-dsk file support
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;	//set sampling rate
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.141592654   
#define MAX_INT 32767
#include "cos666.h"
#include "cos1500.h"
#include "cos2200.h"

void initialize();

//*****encoder delcarations****************

//encoder variables
short input[128];
short no_of_samples = 128;
short a = 0;
short b = 0;
float enc_output[6150];
int buffercount = 0;

//encoder functions
void encode(short);
void process_input();

//*****decoder declarations****************

//decoder variables
short no_of_dec_inputs = 6150;
short out_bit;
float acc_error_metric[4] = {0,0,0,0};
short output_table[4][2] = {{0,7},{3,4},{1,6},{2,5}};
short dec_output[128];
short output_bit_count = 0;
short output_count = 0;
short path_history[4] = {0,0,0,0};
short history_count = 0;
short dec_input_count = 0;

//decoder functions
float find_dist(float a1, float a2, float a3, short b);
void decode();
short min_metric_state();
float modulate(short out_bit);
float gngauss(float mean, float sigma);
float addnoise(float mod_out_bit);
void AddOutput(short);

//***variables for manipulating gel files*******
short inputstate = 1;
short mode = 1;
short noiselevel = 1;
float noise_array[6] = {0.0,0.0,0.3,0.4,2.0,3.0};
short execute = 1;
short oldmode = 1;
short oldnoise = 1;
short oldinputstate = 1;
short y;


//interrupt 11 implemented for handling gel files
interrupt void c_int11()
{
	//input_sample and output_sample are dummy functions which 
	//do not do anything, but helps to handle the proper 
	//handling of the interrupt
	y = input_sample(); 
	output_sample(y);	    
	
	//check for change in slider positions for all gel files
    if((oldmode != mode) ||  
       (oldnoise != noiselevel)||
       (oldinputstate != inputstate))
    	execute = 1;

	//if any of the slider position was changed
	if(execute == 1) 
	{
		int j;
		int noOfErrors = 0;
		initialize();
			
		if (inputstate == 1)
		{
			//intialize with cosine signal of 666Hz
		  	for(j = 0; j < 128; j++)
			   	input[j] = input1[j];
		}
		if (inputstate == 2)
		{
			//intialize with input=cos_666+cos_1500
		  	for(j = 0; j < 128; j++)
			   	input[j] = input1[j] + input2[j];
		}
		if (inputstate == 3)
		{
			//initialize with input=cos_666+cos_2200
		  	for(j = 0; j < 128; j++)
			   	input[j] = input1[j] + input3[j];
		}   

		//reset the output arrays
		for(j = 0; j < 128; j++)
			dec_output[j] = 0;	
		for(j = 0; j < no_of_dec_inputs; j++)
			enc_output[j] = 0;			
		
		//do convolutional encoding
		process_input();

		//do viterbi decoding
		decode();
		
		//estimate the no of errors in the output of decoder
		for(j=0; j < 128; j++)
		{
			if(dec_output[j] != input[j])
				noOfErrors++;
		}
		//set the variables to proper settings for gel operation
		execute = 0;
		oldmode = mode;
		oldnoise = noiselevel;
		oldinputstate = inputstate;
	}
	return;
}


//Main function of the application
void main()
{
	//listen for interrupt 11
	comm_intr();
	while(1);
}


//This function resets the global variables
void initialize()
{
  buffercount = 0;
  output_bit_count = 0;
  output_count = 0;
  history_count = 0;
  dec_input_count = 0;
}

//This function performs convolutional encoding
void encode(short in_bit)
{
	short v1, v2, v3;
	//find outputs of convolutional encoder
	v1 = in_bit;
	v2 = in_bit^b;
	v3 = v2^a;
	//set the contents of shift register
	b = a;
	a = in_bit;
	
	//if basic Viterbi decoding
	if(mode == 2)
	{
		enc_output[buffercount] = v1;
		buffercount++;
		enc_output[buffercount] = v2;
		buffercount++;
		enc_output[buffercount] = v3;
		buffercount++;
	}
	else
	{
		//soft decision
		enc_output[buffercount] = modulate(v1);
		buffercount++;
		enc_output[buffercount] = modulate(v2);
		buffercount++;	
		enc_output[buffercount] = modulate(v3);
		buffercount++;
	}
}

//This function performs BPSK modulation, and adds 
//Gaussian noise according to the noiselevel specified
float modulate(short out_bit)
{
	float mod_output1, mod_output2;
	
	mod_output1 = (2.0*(out_bit))-1;
	mod_output2 = addnoise(mod_output1);
	return mod_output2;
}


// Adding Gaussian White Noise
float addnoise(float mod_out_bit)
{
	float mean,sigma,gaussnoise; 
	float noise_mod_output;
	
	mean=0;
    sigma=noise_array[noiselevel];

	gaussnoise = gngauss(mean,sigma);
    noise_mod_output = mod_out_bit + gaussnoise; 
	return noise_mod_output;
}

//Generates Gaussian random number
float gngauss(float mean, float sigma)
{
	double u,r;     
	
	//create an uniform random variable
	u=(double)rand()/RAND_MAX;
	if (u==1.0)
		u=0.999999999;
	
	//generate Rayleigh random variable
    r=sigma*sqrt(2.0*log(1.0/(1.0-u)));
	
	//creates another uniform random variable
	u=(double)rand()/RAND_MAX;
    if (u==1.0)
		u=0.999999999;

    //generate Gaussian random variable
	return((float)(mean+r*cos(2*PI*u)));
}


//This function takes each input sample, splits it into 16 bits 
//and feeds each bit into the encoder one at a time, starting 
//with the leftmost bit
void process_input()
{
	short i,j;

	for(i = 0; i < no_of_samples; i++)
	{
		short temp  = input[i];
		short in_bit;
		short mask;
		for(j = 0; j < 16; j++)
		{
			mask = 0x0001;
			mask = mask << (15-j);
			in_bit = temp & mask;
			if(in_bit != 0)
				in_bit = 1;
			else
				in_bit = 0;
			encode(in_bit);
		}
	}
	//at the end of input, two zero bits have to be input
	//to the encoder, so as to make the shift register states to zero
	encode(0);
	encode(0);
}


//This function performs Viterbi decoding
void decode()
{
	//these variables represent intermediate values for error metric
	float metric1, metric2, metric3, metric4, metric5, metric6, metric7, metric8;
	
	//these variables represent intermediate values for path history
	short path1, path2, path3, path4;
	
	short sel_state = MAX_INT;

	while(dec_input_count < no_of_dec_inputs)
	{
		if(dec_input_count == 0)
		{
			// only 2 paths exist, i.e. only those starting from state 0
			// i.e., path going from state 0 to state 2, and path going from 
			// state 0 to itself
			acc_error_metric[0] = find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[0][0]);
			acc_error_metric[1] = MAX_INT;
			acc_error_metric[2] = find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[0][1]);
			acc_error_metric[3] = MAX_INT;
			// update path history
			path_history[0] = 0 << 15;
			path_history[2] = 1 << 15;
		}
		else if(dec_input_count == 3)
		{
			// here only 4 paths exist
			metric1 = acc_error_metric[0] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[0][0]);
			metric2 = acc_error_metric[2] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[2][0]);
			metric3 = acc_error_metric[0] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[0][1]);
			metric4 = acc_error_metric[2] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[2][1]);
			acc_error_metric[0] = metric1;
			acc_error_metric[1] = metric2;
			acc_error_metric[2] = metric3;
			acc_error_metric[3] = metric4;
			//update path history
			path1 = path_history[0] | (0 << 14);
			path2 = path_history[2] | (0 << 14);
			path3 = path_history[0] | (1 << 14);
			path4 = path_history[2] | (1 << 14);
			path_history[0] = path1;
			path_history[1] = path2;
			path_history[2] = path3;
			path_history[3] = path4;
		}
		else if(dec_input_count == (no_of_dec_inputs-6))
		{
			// here only 0 is got as input. Therefore only 4 paths exist
			metric1 = acc_error_metric[0] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[0][0]);
			metric5 = acc_error_metric[1] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[1][0]);
			metric2 = acc_error_metric[2] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[2][0]);
			metric6 = acc_error_metric[3] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[3][0]);
			if(metric1 <= metric5)
			{
				acc_error_metric[0] = metric1;
				path1 = path_history[0];
			}
			else
			{
				acc_error_metric[0] = metric5;
				path1 = path_history[1];
			}
			
			if(metric2 <= metric6)
			{
				acc_error_metric[1] = metric2;
				path2 = path_history[2];
			}
			else
			{
				acc_error_metric[1] = metric6;
				path2 = path_history[3];
			}
			acc_error_metric[2] = MAX_INT;
			acc_error_metric[3] = MAX_INT;
			path_history[0] = path1;
			path_history[1] = path2;
		}
		else if(dec_input_count == (no_of_dec_inputs-3))
		{
			//here only 2 paths exist
			metric1 = acc_error_metric[0] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[0][0]);
			metric5 = acc_error_metric[1] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[1][0]);
			if(metric1 <= metric5)
			{
				acc_error_metric[0] = metric1;
				path1 = path_history[0];
			}
			else
			{
				acc_error_metric[0] = metric5;
				path1 = path_history[1];
			}
			acc_error_metric[1] = MAX_INT;
			acc_error_metric[2] = MAX_INT;
			acc_error_metric[3] = MAX_INT;
			path_history[0] = path1;
		}
		else
		{
			// 8 possible paths exist, the steady state condition
			
			//history_count indicates by how many bits we have to shift-left
			if(dec_input_count < 45)
				history_count = 15-((dec_input_count/3)%15);
			else
				history_count = 0;
				
			metric1 = acc_error_metric[0] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[0][0]);
			metric5 = acc_error_metric[1] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[1][0]);
			metric2 = acc_error_metric[2] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[2][0]);
			metric6 = acc_error_metric[3] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[3][0]);
			metric3 = acc_error_metric[0] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[0][1]);
			metric7 = acc_error_metric[1] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[1][1]);
			metric4 = acc_error_metric[2] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[2][1]);
			metric8 = acc_error_metric[3] + find_dist(enc_output[dec_input_count],enc_output[dec_input_count+1],enc_output[dec_input_count+2],output_table[3][1]);
			
			if(metric1 <= metric5)
			{
				acc_error_metric[0] = metric1;
				path1 = path_history[0] | (0 << history_count);
			}
			else
			{
				acc_error_metric[0] = metric5;
				path1 = path_history[1] | (0 << history_count);
			}
			
			if(metric2 <= metric6)
			{
				acc_error_metric[1] = metric2;
				path2 = path_history[2] | (0 << history_count);
			}
			else
			{
				acc_error_metric[1] = metric6;
				path2 = path_history[3] | (0 << history_count);
			}
			
			if(metric3 <= metric7)
			{
				acc_error_metric[2] = metric3;
				path3 = path_history[0] | (1 << history_count);
			}
			else
			{
				acc_error_metric[2] = metric7;
				path3 = path_history[1] | (1 << history_count);
			}
			
			if(metric4 <= metric8)
			{
				acc_error_metric[3] = metric4;
				path4 = path_history[2] | (1 << history_count);
			}
			else
			{
				acc_error_metric[3] = metric8;
				path4 = path_history[3] | (1 << history_count);
			}
			path_history[0] = path1;
			path_history[1] = path2;
			path_history[2] = path3;
			path_history[3] = path4;
		}  // if else ends
		
		if((dec_input_count >= 45) && (dec_input_count < (no_of_dec_inputs-3)))
		{
			//if the path_history variable is full
			short i;

			sel_state = min_metric_state();
			out_bit = path_history[sel_state] & 0x8000;
			AddOutput(out_bit);
			for(i = 0; i <= 3; i++)
				path_history[i] = path_history[i] << 1;
		}

		if(dec_input_count == (no_of_dec_inputs-3))
		{
			//empty out the whole path_history, it is having 16 bits
			//the last 2 bits can be ignored as they were input to the encoder
			// to reset the shift registers to zero
			short i,mask;
	
			for(i = 0; i < 14; i++)
			{
				mask = 1 << (15-i);
				out_bit = path_history[0] & mask;
				out_bit = out_bit << i;
				AddOutput(out_bit);
			}
		}

		dec_input_count = dec_input_count+3;
	}//while ends
}

//This function finds out the state with the minimum metric value
short min_metric_state()
{
	float min = MAX_INT;
	short sel_state = MAX_INT;
	short i;
	for(i = 0; i <=3; i++)
	{
		if(min > acc_error_metric[i])
		{
			min = acc_error_metric[i];
			sel_state = i;
		}
	}
	return sel_state;
}

//This function creates the decoder output, by adding
//one bit at a time
void AddOutput(short i)
{
	output_bit_count++;
	if((i & 0x8000) != 0)
		i = 1;
	else
		i = 0;
	dec_output[output_count] = dec_output[output_count] | (i << (16-output_bit_count));
	if(output_bit_count == 16)
	{
		//one sample is created, go for the next sample
		output_bit_count = 0;
		output_count++;
	}
}

//This function finds the metric value
float find_dist(float a1, float a2, float a3, short b)
{
  float b1,b2,b3;
  float dist;
  
  //finds the bits of the given symbol b
  b1 = (b & 0x04) >> 2;
  b2 = (b & 0x02) >> 1;
  b3 = b & 0x01;

  //if soft decision, do BPSK modulation 
  // before finding the metric
  if(mode == 1)
  {
  	if(b1 == 0)
    	b1 = -1;
  	if(b2 == 0)
    	b2 = -1;
  	if(b3 == 0)
    	b3 = -1;
  }
 
  //method to find the metric,
  //can be generalized for both basic Viterbi and soft decision 
  dist = abs(b1-a1)+abs(b2-a2)+abs(b3-a3);
  return dist;
}




