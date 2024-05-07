//RTDX_matlab_sim.c MATLAB-DSK interface using RTDX between PC & DSK

#include <rtdx.h>            								//RTDX support file
#include "target.h"                                //for init interrupt

RTDX_CreateInputChannel(ichan);   //create input channel-data transfer PC-->DSK
RTDX_CreateOutputChannel(ochan);  //create output channel-data transfer DSK-->PC
short buffer[10] = {0};				//init data from PC

void main(void)
{
 int i;                       	

 TARGET_INITIALIZE();                              //init for interrupt
 while(!RTDX_isInputEnabled(&ichan)) 					//for MATLAB to enable RTDX
   puts("\n\n Waiting to read ");					//while waiting
 RTDX_read(&ichan,buffer,sizeof(buffer));		   	//read data from PC to DSK
 puts("\n\n Read Completed");						
 
 for (i = 0; i<10; i++)
 	buffer[i]++;                                    //data from PC

 while(!RTDX_isOutputEnabled(&ochan)) 					//for MATLAB to enable RTDX
 	puts("\n\n Waiting to write ");			      //while waiting

 RTDX_write(&ochan,buffer,sizeof(buffer));			//send data from DSK to PC
 puts("\n\n Write Completed");

 while(1)	{}													// infinite loop
}

