//Receiver.c Demodulates received BPSK voice signal 
#include "dsk6713_aic23.h"			//codec-DSK support file
#include <math.h>

Uint32 fs=DSK6713_AIC23_FREQ_96KHZ;		//set sampling rate
#define 	NUMSAMP			8			// # Samples per Symbol
#define		NUM_BITS		100
#define PI					3.1415926
#define SYNC_INTERVAL		100			// Interval between sync sequence
short carrier_sample;			// input sample
short data_sample;

short carrier_symbol[NUMSAMP];	// buffer to received one period of carrier
short data_symbol[NUMSAMP*2];	// buffer to received one period of data

short csIndex=0;				// symbol buffer index
short dsIndex=NUMSAMP;				// symbol buffer index
short phIndex=0;

#define NPHI 100
float phiBuf[NPHI]={0};			// buffer to view phi estimates from PLL
short phiBind=0;				// phi buffer index
short phiLoop = 0;				// Used to determine if Phi looped beyond 0<phi<2PI

float sin_table[96*NUMSAMP];	// a table of 3 periods of a sin wave
short pow_table[16];			// a table of powers of two

float y1, y2;					// correlation vectors
float phi=PI;					// phase estimate
float dphi = 0;					// change in phi
float damp=1;					// damping factor

short decSeqNum = 0;			// Number of encoded bits
short decodeSeq[9]={0};			// Encoder Symbol Index

short syncSequence[8] = {1,1,1,0,1,0,0,1};	// Syncronization Sequence
short syncTrigger = 0;			// Trigger to determine if Sync Seq is found
short syncInterval = 0;			// Looks for the interval between Sync Seqs
short rBit[2];					// Received bits
short bits[8]={0};				// Received bits in one frame
short bitIndex=0;				// Index into frame
short nrBits = 0;				// Determines how many received bits are in buffer

short out_sample = 0;			// current received sample
short prev_sample = 0;			// previously received sample
short prev_sample2 = 0;			// received two sample before current sample
short outInc = 0;				// Increments through the interpolation polynomial

// DEBUG VARIABLES

short TrigBuffer[200] = {0};	// Used to look at incoming trigger values
short tbIndex = 0;				// Index into TrigBuffer
short carrierBuffer[1000];		// Buffer for the carrier wave
short dataBuffer[1000];			// Buffer for the BPSK Data wave
short bufIndex = 0;				// Index into carrierBuffer & dataBuffer

short sampleBuffer[2000];		// Buffer of output Samples
short sBuffer = 0;				// Index into sampleBuffer

int X=0;						// Used in correlation receiver

interrupt void c_int11()            //interrupt service routine
{

	int i,j;
	int max=1;
	
  	carrier_sample = (short) input_right_sample(); //receive carrier
	data_sample = (short) input_left_sample(); 	// receive data		
		
	carrier_symbol[csIndex++] = carrier_sample;	// put carrier in symbol buffer
	data_symbol[dsIndex++] = data_sample;		// put data in symbol buffer
	
	carrierBuffer[bufIndex] = carrier_sample;	// debug carrier values
	dataBuffer[bufIndex] = data_sample;			// debug data values
	
	if(++bufIndex >= 1000)
		bufIndex = 0;

   	if(csIndex >= NUMSAMP)				// after one period is received
	{									// then perform phi estimate
		y1 = 0;							
		y2 = 0;
		csIndex = 0;					// Reset Index
		dsIndex = NUMSAMP;
		
		for(i=0; i<NUMSAMP; i++)		// correlate received carrier symbol
		{
			y1 += carrier_symbol[i]*sin_table[32*(NUMSAMP + i + phIndex) - 1];
			y2 += carrier_symbol[i]*sin_table[32*(NUMSAMP + i + phIndex) + 1];
			if(carrier_symbol[i] > max)
				max = carrier_symbol[i];
		}		
		if(max != 0)
		{
			y1 = y1 / max;					// normalize correlation coefs
			y2 = y2 / max;
		}
		else
		{
			max = 1;
		}
		dphi = 0.2*(y2 - y1);			// Calculate derivative to find the maximum
										// of the matched filter
		if(dphi > PI)					// help keep phi within 0<phi<2*PI
			dphi = PI;
		phi = phi + dphi;				// determine new estimate for phi
		phiLoop = 0;
		if(phi < 0)						// normalize phi to 0<phi<2*PI
		{
			phi = phi + 2*PI;	
			phiLoop = -1;
		}
	    if(phi > (2*PI))
	    {
	    	phi = phi - 2*PI;
	    	phiLoop = 1;
	    }	
		phiBuf[phiBind++]=phi;			// put phi in buffer for viewing
		if(phiBind >= NPHI)				// resent buffer index
			phiBind = 0;
		
		phIndex = (int) (NUMSAMP-1)*phi/(2*PI); // update phi's index into correlators

		if(phiLoop == 0)				// No wrapping 
		{								// Receive one bit
			X = 0;
			for(i=0; i<NUMSAMP; i++)	// Correlate Signal with Basis Function
			{
				X += data_symbol[(NUMSAMP-1)-phIndex+i]*sin_table[32*i];		
				data_symbol[i] = data_symbol[NUMSAMP + i];
			}	
    		rBit[0] = (X >= 0) ? 1: 0;  	// Make Detection
    		nrBits = 1;
    	}
		if(phiLoop == 1)					// phi wrapped to 0
		{									// Received two bits
			phiLoop = 0;
			X = 0;
			for(i=0; i<NUMSAMP; i++)		// Correlate Signal with Basis Function
			{
				X += data_symbol[i]*sin_table[32*i];			
			}
    		rBit[0] = (X >= 0) ? 1: 0;  	// Make Detection;
    		
    		X = 0;
			for(i=0; i<NUMSAMP; i++)		// Correlate Signal with Basis Function
			{
				X += data_symbol[(NUMSAMP-1)-phIndex+i]*sin_table[32*i];		
				data_symbol[i] = data_symbol[NUMSAMP + i];
			}	
    		rBit[1] = (X >= 0) ? 1: 0;  	// Make Detection			
    		nrBits = 2;
		}
		if(phiLoop == -1)					// Phi wrapped to 2*PI
		{									// don't receive any bits
			phiLoop = 0;
			for(i=0; i<NUMSAMP; i++)
			{								// update symbol values
				data_symbol[i] = data_symbol[NUMSAMP + i];
			}
			nrBits = 0;
		}

		for(i=0; i<nrBits; i++)				// Loop over the received bits
		{
			bits[bitIndex++] = rBit[i];

	    	decodeSeq[7] = rBit[i];			// put received bit in decSeq Buffer
	    	syncTrigger = 0;
			for(j=0; j<8; j++)				// Search the last 8 bits for
			{								// frame syncronization sequence
				if(j < 8)					// mask received seq with sync seq
				{
					syncTrigger += (decodeSeq[j] == syncSequence[j]);
				}
				decodeSeq[j] = decodeSeq[j+1];
			}
			syncInterval++;
			if(syncTrigger == 8)	// If sync interval is found set frame index
			{
				if(syncInterval == 8*(SYNC_INTERVAL+1))	// Look for symbols separated
				{										//  by the sync interval
					bitIndex = 0;						// Set the frame sync point
					prev_sample2 = prev_sample;			// Use same values
					prev_sample = out_sample;			// for output if sync bit is present						
				}
				syncInterval = 0;				
			}
			TrigBuffer[tbIndex++] = syncTrigger;	// Debug buffer to look at trig
			if(tbIndex >= 200)						// Reset trig buffer index
				tbIndex = 0;
				
				
			if(bitIndex >= 8)		// If a byte is received decode it
			{	
				prev_sample2 = prev_sample;		// Update 3rd polynomial vertex
				prev_sample = out_sample;		// Update 2nd polynomial vertex
				bitIndex = 0;					// reset index into bit buffer
				out_sample=0;					// set output sample to zero
				outInc = 0;						// Reset polynomial interp index
				if(bits[7]==1)		// If negative value take twos comliment
				{
					for(j=0;j<7;j++)  // Mask bits twos compliment
					{
						bits[j] = (bits[j] == 0);
					}
				}
    			for(j=8; j<15; j++)  // Decode the bits
    			{
            		out_sample=out_sample+pow_table[j]*bits[j-8];
    			}
    			if (bits[7]==1)		// If negative change the sign and add 1*2^8
    			{
            		out_sample=-(out_sample+256);
    			}  
    			sampleBuffer[sBuffer++] = out_sample;
					if(sBuffer >= 2000)
						sBuffer = 0;   			
			}	
		}
		
    	if(decSeqNum >= NUM_BITS)	// Reset index for decoded sequence
    	{
    		decSeqNum = 0;
    	}
	}

	// Use 3 point polynomial interpolation for output values
   	output_sample((out_sample-2*prev_sample + prev_sample2)*(outInc*outInc/64-outInc)/128 
   					+ (prev_sample-prev_sample2)*outInc/64 + prev_sample2);
   	outInc++;
}


void init_sin_table()  	// Tables used to speed computation
{
	int i;
	
	for(i=0; i<96*NUMSAMP; i++)		// Create table of sin waves
	{
		sin_table[i] = sin(2*PI*i/(32*NUMSAMP));
	}
	for(i=0; i<16; i++)				// Create Table of powers of two
	{
		pow_table[i] = pow(2, i);
	}
}

void main()
{
  init_sin_table();
  comm_intr();                  	//init DSK, codec, McBSP
  while(1);                     	//infinite loop
}


 
