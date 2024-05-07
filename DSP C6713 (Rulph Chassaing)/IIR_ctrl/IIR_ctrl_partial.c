//IIR_ctrl.c Core program. Select IIR filter with srambling option using DIP SW
.
short DIP_Mask = 20;									//any DIP SW value except 0-15
short BUFFER_SIZE = 512;							//size of buffer
short buffer[512];									//buffer for voice scrambler
short index=0,input_index=0,output_index=0;	//index for sample #, buffer

interrupt void c_int11()
{
 short i, input;
 int un, yn;

 input = (short)input_sample();	            //external input
 if (DIP_Mask == 0) {								//output=input (no filtering)
     .													//like a loop program
     yn=input;  			
   }
 } else if (DIP_Mask == 1) {						//2-kHz LP filter if DIP = 1
   for (i = 0; i < stages; i++) {un=input-...yn=...update delays - See IIR.c}
	.
   }else if (DIP_Mask == 2) {...					//...for other filters }
    else if (DIP_Mask == 5){						//voice scrambler if DIP SW=5
    if((index % 2) == 0) {							//for every other sample
     buffer[input_index++] = input;				//input sample->buffer
     if(input_index==BUFFER_SIZE) {input_index=0;} //reset buffer when full
    }
   if (index >= BUFFER_SIZE) {					//if buffer is at least half full
     yn = buffer[output_index++];				//output next value
     if(output_index==BUFFER_SIZE) {output_index=0;} //reset buffer if at end
   }
   index++;												//increase overall sample index
   if(index>=(BUFFER_SIZE*2)) {index=0; }  	//overall sample index if at end
   } else { yn = 0;	}								//no output if other DIP value
 output_sample((short)(yn)); 			      	// output
 return;
}

void main()
{
  comm_intr();
  while(1) {
    short newMask = 0;
    newMask += DSK6713_DIP_get(3) * 1;
    newMask += DSK6713_DIP_get(2) * 2;
    newMask += DSK6713_DIP_get(1) * 4;
    newMask += DSK6713_DIP_get(0) * 8;			//hex value of DIP switch
    if (DIP_Mask != newMask) {					//wait for change
 	  DIP_Mask = newMask;							//load DIP switch value
    if (DIP_Mask == 5) {
        DSK6713_LED_on(3);
        DSK6713_LED_off(2);
        DSK6713_LED_on(1);
        DSK6713_LED_off(0);
    } else if (DIP_Mask == 4) { ...
      }
    }
  }         //end of while(1)
}  			//end of main
