//Beatdetector.c Partial program for beat detection project

const int chunks = 20; 								//number of frames in buffer
const int instant_length = 200; 					//length of 1 buffer
#define average_length 4000 						//length of buffer
const float c = 1.3; 								//confidence multiplier
double ae = 0, ie = 0;
short buffer[average_length]; 					//Buffer

void main()
{
 comm_poll();         								//init DSK, codec, McBSP
 while(average_counter<average_length){		//sample entire buffer
	buffer[average_counter]=input_sample();
	average_counter++;
 }
 while(1){												//infinite loop
  instant_counter = 0;
  while(instant_counter<instant_length){ 		//sample one frame and
  	buffer[chunk_counter*instant_length+instant_counter]=input_sample();
  	instant_counter++;                    		//move it to circular buffer
  }
  for(average_counter=0;average_counter<average_length;average_counter++){
	ae=ae+buffer[average_counter]*buffer[average_counter];//average energy
  }                                                  		//in entire buffer
  ae = ae / average_length;
  for(instant_counter=0;instant_counter<instant_length;instant_counter++){
	ie=ie+buffer[chunk_counter*instant_length+instant_counter]
   	*buffer[chunk_counter*instant_length+instant_counter];
  } 													//average energy in last few msec
  ie = ie / instant_length;
  if (ie > ae*c){//if energy in short time buffer>whole buffer,turn on LEDs
  .
  else {			  //if not,turn off LEDs
  .
  }
  chunk_counter++; //increment position in chunk counter
  if(chunk_counter>=chunks) chunk_counter=0; //index right point in buffer
 } //end of while(1) infinite loop
}  //end of main

 


