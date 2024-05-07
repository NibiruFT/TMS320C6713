//Modulation Schemes (PAM, BPSK, QPSK)

#include "DSK6713_aic23.h"
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;

#include <math.h>
//Initialization:
int i_4PAM_M, i_8PAM_M, i_16PAM_M, i_BPSK_M, i_BPSK_D, i_QPSK_M, i_QPSK_D;
int j_4PAM_M, j_4PAM_D, j_8PAM_M, j_8PAM_D, j_16PAM_M, j_16PAM_D, j_BPSK_M, j_BPSK_D, j_QPSK_M, j_QPSK_D ;
int k;
int masked_value, demod_level, buffer, initial, output, assign, bit;
int temp[4];
int data_4PAM[4];													//data table for 4-PAM MOD & DEMOD
int data_8PAM[8];													//data table for 8-PAM MOD & DEMOD
int data_16PAM[16];													//data table for 16-PAM MOD & DEMOD
short mode=1;														//gel file variable

//Data table for BPSK MOD & DEMOD:
int data_BPSK[2][4]={0, 1000, 0, -1000,								//0 degree		
                  	 0, -1000, 0, 1000};							//180 degree		
//Data table for QPSK MOD & DEMOD:            
int data_QPSK[4][4]={0, 1000, 0, -1000, 							//phase=0 & symbol=00	      
                     1000, 0, -1000, 0,								//phase=90 & symbol=01
                    -1000, 0, 1000, 0,		 						//phase=270 & symbol=10
                     0, -1000, 0, 1000}; 							//phase=180 & symbol=11

interrupt void c_int11()         									//interrupt service routine
{
	int sample_data;
	switch(mode)
	{  
    	case 1:
    	{
	   		//4-PAM Modulator: Assigns sampled data coming from an input 
	   		//source to 4 predetermined voltage levels.
			if (i_4PAM_M==96)										//new input is taken once every 96 samples (8 shifts x 12 repeated outputs) 
      		{
      			sample_data = input_sample(); 						//inputs data
      			i_4PAM_M=0;
      			j_4PAM_M=0;
      		}
      		masked_value = sample_data & 0x0003;					//masks input sample as 2-bit segments
      		output = data_4PAM[masked_value];       				//gets corresponding level from table 
      		output_sample(output);									//outputs corresponding voltage level 12 times
      		j_4PAM_M++;												//repeated output counter 
      		
   			if (j_4PAM_M==12)										//checks if repetition is over
      		{
        	j_4PAM_M=0;						
        	sample_data = sample_data >> 2;							//shifts input to mask next segment 
      		}
       		i_4PAM_M++;
      		break;      
    	}
//---------------------------------------------------------------------------------------------------------------  
		case 2:
    	{
    		//4-PAM Demodulator
      		sample_data = input_sample(); 							//inputs data
      		buffer=0;												//initializing buffer 
   
      		for (j_4PAM_D=0; j_4PAM_D<8; j_4PAM_D++) 
      		{
        		masked_value = sample_data & 0x0003;				//masks input sample as 2-bit segments
        		output = data_4PAM[masked_value];					//retrieves the corresponding hex value from data table
        		sample_data = sample_data >> 2;						//shifts input so as to mask another part on the next cycle 
      
        		demod_level = (initial - output)/(0x5555);			//determines which level was transmitted by doing inverse operation to modulator
        		demod_level = demod_level << 14;					//aligns constructed value since least significant bits are first extracted 	
        		buffer = buffer + demod_level;						//adds bit pattern just recovered to previously formed output sample
        		buffer = buffer >> 2;								//creates vacancy for bits coming next
      		}
      		output_sample(buffer*16);								//adjusts output voltage level
      		break; 
    	} 
//---------------------------------------------------------------------------------------------------------------  
		case 3:
		{
			//8-PAM Modulator: Assigns sampled
			//data coming from an input source to 8 predetermined voltage levels.
			if (i_8PAM_M==60)										//new input is taken once every 60 samples (5 shifts x 12 repeated outputs)
      		{
      			sample_data = input_sample();						//inputs data
      			sample_data = sample_data >> 1;  					//least significant bit discarded
      			i_8PAM_M=0;
      			j_8PAM_M=0;
      		}
      		masked_value = sample_data & 0x0007;					//masks input sample as 3-bit segments
      		output = data_8PAM[masked_value];       				//gets corresponding level from table
      		output_sample(output);									//outputs corresponding voltage level 12 times
      		j_8PAM_M++;												//repeated output counter 
   
      		if (j_8PAM_M==12)										//checks if repetition is over						
      		{
        		j_8PAM_M=0;						
        		sample_data = sample_data >> 3;						//shifts input so as to mask another part 
      		}
      		i_8PAM_M++;
      		break;      
		}
//---------------------------------------------------------------------------------------------------------------  
		case 4:
		{
			//8-PAM Demodulator
      		sample_data = input_sample(); 							//inputs data
   	  		sample_data = sample_data >> 1;							//discards LSB so that remaining number of bits (15)
      		buffer=0;												//is a multiple of 3, which makes shifting easier
   
      		for (j_8PAM_D=0; j_8PAM_D<5; j_8PAM_D++) 
      		{
        		masked_value = sample_data & 0x0007;				//masks input sample as 3-bit segments
        		output = data_8PAM[masked_value];					//retrieves the corresponding hex value from data table
        		sample_data = sample_data >> 3;						//shifts input so as to mask another part on the next cycle  
      
        		demod_level = (initial - output)/(0x2492);			//determines which level was transmitted by doing inverse operation to modulator
        		demod_level = demod_level << 13;					//aligns constructed value since least significant bits are first extracted 
        		buffer = buffer + demod_level;						//adds bit pattern just recovered to previously formed output sample
        		buffer = buffer >> 3;								//creates vacancy for bits coming next
      		}
      		output_sample(buffer*16);								//adjusts output voltage level
      		break; 
		}
//---------------------------------------------------------------------------------------------------------------  
		case 5:
		{
			//16-PAM Modulator: Assigns sampled
       		//data coming from an input source to 16 predetermined voltage levels.

      		if (i_16PAM_M==48)										//new input is taken once every 48 samples (4 shifts x 12 repeated outputs)
      		{
      			sample_data = input_sample(); 						//inputs data
      			i_16PAM_M=0;
      			j_16PAM_M=0;
      		}
      		masked_value = sample_data & 0x000F;					//masks input sample as 4-bit segments
      		output = data_16PAM[masked_value];       				//gets corresponding level from table
      		output_sample(output);									//outputs corresponding voltage level 12 times
      		j_16PAM_M++;											//repeated output counter
   
      		if (j_16PAM_M==12)										//checks if 4-bit segment was output						
      		{	
        		j_16PAM_M=0;						
        		sample_data = sample_data >> 4;						//shifts input so as to mask another part 
      		}
       
      		i_16PAM_M++;
      		break;      
		}
//---------------------------------------------------------------------------------------------------------------  
		case 6:
		{
			//16-PAM Demodulator
			sample_data = input_sample(); 							//inputs data
      		buffer=0;												//initializing buffer
   
      		for (j_16PAM_D=0; j_16PAM_D<4; j_16PAM_D++) 
      		{
        		masked_value = sample_data & 0x000F;				//masks input sample as 4-bit segments
        		output = data_16PAM[masked_value];					//retrieves the corresponding hex value from data table
        		sample_data = sample_data >> 4;						//shifts input so as to mask another part on the next cycle 
      
        		demod_level = (initial - output)/(0x1111);			//determines which level was transmitted by doing inverse operation to modulator
        		demod_level = demod_level << 12;					//aligns constructed value since least significant bits are first extracted
        		buffer = buffer + demod_level;						//adds bit pattern just recovered to previously formed output sample	
        		buffer = buffer >> 4;								//creates vacancy for bits coming next
      		}
      		output_sample(buffer*16);								//adjusts output voltage level
      		break; 
		}
//---------------------------------------------------------------------------------------------------------------  
		case 7:
		{
			//BPSK Modulator
			if (i_BPSK_M==64)										//determines when to get new input
      		{
        		sample_data = input_sample(); 						//inputs data
        		i_BPSK_M=0;
        		j_BPSK_M=0;
      		}
      
      		masked_value = sample_data & 0x0001;					//masks input sample as 1-bit segments
      		output = data_BPSK[masked_value][j_BPSK_M];				//gets corresponding level from table     
      		output_sample(output*10);								//outputs corresponding sinusoid
      		j_BPSK_M++;												//repeated output counter
   
     		if (j_BPSK_M==4)										//checks if 1-bit segment was output						
     		{
       			j_BPSK_M=0;						
       			sample_data = sample_data >> 1;						//shifts input so as to mask another part 
     		} 
      		i_BPSK_M++;									
  	  		break;
		}
//---------------------------------------------------------------------------------------------------------------  
		case 8:
		{
			//BPSK Demodulator
			sample_data = input_sample(); 							//inputs data
   			buffer=0;
   			for (i_BPSK_D=0; i_BPSK_D<16; i_BPSK_D++)				//counting 16 segments
   			{
     			for (j_BPSK_D=0; j_BPSK_D<4; j_BPSK_D++) 			//counting 4 data_table bits
     			{
       				masked_value = sample_data & 0x0001;			//masks input sample as 1-bit segments
       				output = data_BPSK[masked_value][j_BPSK_D];		//retrieves the corresponding hex value from data table
       				temp[j_BPSK_D]=output;							//temp holds modulated value
     			} 
     			bit=0;												//initializes bit to its default value												
     																//demodulation starts
     			if (temp[1]==-1000)									//checks the 2nd value in temp array and							
     			{													//determines if corresponding bit is 1 or 0	
       				bit = 1;										
     			}
     			bit = bit << 15;									//aligns determined bit to right location
     			buffer = buffer + bit;
     		
     			if (i_BPSK_D!=15)									//prevents redundant shifting
     			buffer = buffer >> 1;								
     			
     			sample_data = sample_data >> 1	;					//shifts input so as to mask another part          
   			}
    		output_sample(buffer);
			break;
		}
//---------------------------------------------------------------------------------------------------------------  
		case 9:
		{
			//QPSK Modulator
			if (i_QPSK_M==32)										//determines when to get new input
   			{
     			sample_data = input_sample(); 						//inputs data
   
     			i_QPSK_M=0;
     			j_QPSK_M=0;
   			}
      
   			masked_value = sample_data & 0x0003;					//masks input sample as 2-bit segments
  			output = data_QPSK[masked_value][j_QPSK_M];       
   			output_sample(output*10);								//outputs corresponding sinusoid
   			j_QPSK_M++;
   
   			if (j_QPSK_M==4)										//checks if 2-bit segment was output						
   			{
     			j_QPSK_M=0;						
     			sample_data = sample_data >> 2;						//shifts input so as to mask another part 
   			} 
   			i_QPSK_M++;
  			break;
		}
//---------------------------------------------------------------------------------------------------------------  
		case 10:
		{
			//QPSK Demodulator
			sample_data = input_sample(); 							//inputs data
   			buffer=0;
   			for (i_QPSK_D=0; i_QPSK_D<8; i_QPSK_D++)				//counting 8 segments
   			{
     			for (j_QPSK_D=0; j_QPSK_D<4; j_QPSK_D++) 			//counting 4 data_table bits
     			{
       				masked_value = sample_data & 0x0003;			//masks input sample as 2-bit segments
       				output = data_QPSK[masked_value][j_QPSK_D];		//retrieves the corresponding hex value from data table
       				temp[j_QPSK_D]=output;							//temp holds modulated value
     			} 
     			bit=0;												//initializes bit to its default value

     			if (temp[0]==0)										//demodulation starts
     			{													
       				if (temp[1]==-1000)
       					bit=3;			
     			}													//checks first 2 values and
     			else												//determines corresponding dibit
     			{
       				if (temp[0]==1000)
         				bit=1;
       				else
         				bit=2;
     			}    
     			bit = bit << 14;									//aligns determined bit to right location
     			buffer = buffer + bit;
     		
     			if (i_QPSK_D!=7)									//prevents redundant shifting
     			buffer = buffer >> 2;
     
     			sample_data = sample_data >> 2	;					//shifts input so as to mask another part          
   			}    	
    		output_sample(buffer);	
  			break;
		}
	}
	return;
}

void main()
{
	initial=0x7FFF;
	//4PAM Modulator:						
	for(k=0; k<4; k++)												//forms look-up table for 4-level PAM and voice modulating scheme (PAM-R)      
	{  
		assign = initial-(k*0x5555);
 		data_4PAM[k] = assign;
	}
	i_4PAM_M=0;
	
	//8PAM Modulator:
	for(k=0; k<8; k++)												//forms look-up table for 8-level PAM and voice modulating scheme (PAM-R)      
	{  
		assign = initial-(k*0x2492);
 		data_8PAM[k] = assign;
	}
	i_8PAM_M=0;
	
	//16PAM Modulator:
	for(k=0; k<16; k++)												//forms look-up table for 16-level PAM and voice modulating scheme (PAM-R)      
	{  
		assign = initial-(k*0x1111);
 		data_16PAM[k] = assign;
	}
	i_16PAM_M=0;

	//BPSK Modulator:
	i_BPSK_M=64;
	j_BPSK_M=0;
   
	//QPSK Modulator:
	i_QPSK_M=32;
	j_QPSK_M=0;

	comm_intr();													//init DSK, codec, McBSP
	while(1);														//infinite loop
}
