/* pll.c Software Linear PLL 6211 Program						*/
/* Upendra Ummethala and Mike Mellor                           	*/
/* December 19, 2000 EE539 Real Time DSP Final Project       	*/

#define N 8000	
#include "sin8000_table.h"/* Sine wave digitized to 8000 points */

/*  The variable names used below are from Chapter 5 in  		*/
/*  the book: Phase-Locked Loops by Roland Best          		*/
/*  Fourth edition McGraw-Hill                           		*/

int u1_n;						/* Input signal at time n 		*/
int u2_n=0;                     /* DCO output signal at time n  */                            	
int u2_nplus1;                  /* DCO output signal at n+1		*/
int phi2_n=0;					/* DCO output phase at time n 	*/
int phi2_nplus1;        	    /* DCO output phase at n+1		*/
int ud_n_1=0;                   /* Phase detector output at n-1	*/
int ud_n;                       /* Phase detector output at n	*/
int uf_n_1=0;                  	/* Loop filter output at n-1	*/
int uf_n;                       /* Loop filter output at n 		*/	                                  	
int inp_freq=100;  				/* Input sine freq = 100Hz		*/        
int table_index=0; 				/* Index into the sine table	*/
int w0= 628;      				/* 100Hz = 628 rad/s target freq*/
int K0= 1;        				/* VCO Gain scaled up by ~2^6	*/
int uf_n_tmp;     				/* Tmp to scale	uf_n down by 2^6*/
int cycle_count=0;              /* Number of sine cycles		*/
int pi_scaled=25133; 			/* PI scaled by 8000 Hz rate	*/
int A1 = -4002;		/* Loop filter coefficient scaled up by 2^12*/
int B0 = 4348;		/* Loop filter coefficient scaled up by 2^12*/
int B1 = -3784;		/* Loop filter coefficient scaled up by 2^12*/
 
void main()
{

	comm_poll();
		while(1)               	/* Runs forever					*/
		{ 
		 
		 								/* Create scaled input	*/
		 u1_n=31*sin8000_table[table_index];/* from sine table  */
		 
		 /* The following is for generating the input*/
		 table_index=table_index+inp_freq;
		 if(table_index >= N)	 
        	{	                /* Unwrap phase if 2*pi range  	*/
        	table_index -= N;   /* is exceeded, i.e. modulo N 	*/
        	cycle_count++; 		/* Count cycles at phase wrap 	*/
        	}
        
        if(cycle_count >100)	/* After 100 cycles 			*/
			{	                /* at this phase give 			*/
         	table_index += (N >> 2); /* a 180 degree phase step */
                                     
         	cycle_count = 0;	/* Reset cycle count. 			*/
        	}                        
        	
          	u1_n=input_sample();	/* obtain input from ADC*/
                  		
		  	ud_n=u1_n*u2_n;      	/* Phase detector output 	*/
		  	uf_n=-A1*uf_n_1+B0*ud_n+B1*ud_n_1; /* Filter output	*/
		  	uf_n=uf_n>>12;     		/* Scaled down filter output*/
		  	uf_n_tmp=uf_n>>6;  		/* Undo VCO Gain(K0) scaling*/
		  
		                            /* Calculate next DCO output*/
		                            /* phase for time n+1		*/ 		  
		  phi2_nplus1=phi2_n+(w0+K0*uf_n_tmp);
		  
		                            /* Maintain -pi to +pi range*/
		                            /* for next DCO phase		*/    
		  if (phi2_nplus1>pi_scaled)	/* If next DCO phase>pi	*/
		  phi2_nplus1=phi2_nplus1-2*pi_scaled; /* unwrap by 2*pi*/  
		  
		  if (phi2_nplus1>=0)  	/* If next DCO phase is positive*/		  
		  u2_nplus1=1; 			/* DCO output=+1(Walsh function)*/
		  else                  /* If next DCO phase is negative*/
		  u2_nplus1=-1; 		/* DCO output=-1(Walsh function)*/ 
		  
		  output_sample(u2_nplus1*31000); /* Output to scope	*/
		  
					/* Update values for next pass through loop	*/  
		  ud_n_1=ud_n;     		/* Phase detector: ud(n-1)=ud(n)*/
		  uf_n_1=uf_n;          /* Filter ouput: uf(n-1)=uf(n)	*/
		  phi2_n=phi2_nplus1;   /* DCO phase: phi2(n)=phi2(n-1)	*/
		  u2_n=u2_nplus1;       /* DCO output: u2(n)=u2(n+1)	*/
	}   
		
}