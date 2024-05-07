//Soundboard_core.c  Sound effects
union {Uint32 uint; short channel[2];} AIC23_data;
union {Uint32 uint; short channel[2];} AIC23_input;
short EchoLengthB = 8000;									//echo delay
short EchoBuffer[8000];			     						//create buffer
short echo_type = 1;											//to select echo or delay
short Direction = 1;											//1->longer echo,-1->shorter
short EchoMin=0,EchoMax=0;			 						//shortest/longest echo time
short DistMag=0,DistortionVar=0,VolSlider=100,PreAmp=100,DistAmp=10;
short HarmBuffer[3001];				   					//buffer
short HarmLength=3000;										//delay of harmonics
float output2;
short DrumOn=0,iDrum=0,sDrum=0;	//turn on drum sound when = 1,and counter
int 	DrumDelay=0,tempo=40000;							//delay counter/drum tempo
short ampDrum=40;											  	//volume of drum sound
.                                                  //addtl casting
interrupt void c_int11()	           				  	//ISR
{
AIC23_input.uint = input_sample();	           	  	//newest input data
input = (short)(AIC23_input.channel[RIGHT] + AIC23_input.channel[LEFT])/2;
input = input*.0001*PreAmp*PreAmp;
output=input;
output2=input;       										//distortion section
if (output2>0) output2=0.0035*DistMag*DistMag*DistMag*((12.35975*(float)input)
                 			-(0.359375*(float)input*(float)input));
else  output2 =0.0035*DistMag*DistMag*DistMag*(12.35975*(float)input
                +0.359375*(float)input*(float)input);
output2/=(DistMag+1)*(DistMag+1)*(DistMag+1);
if (output2 > 32000.0)  output2 = 32000.0 ;
else if (output2 < -32000.0 )  output2 = -32000.0;
output= (output*(1/(DistMag+1))+output2);  			//overall volume slider
input = output;                                    //echo/reverb section
iEcho++;                                    			//increment buffer count
if (iEcho >= EchoLengthB) iEcho = 0;          		//if end of buffer reinit
output=input + 0.025*EchoAmplitude*EchoBuffer[iEcho];	//newest+oldest sample
if(echo_type==1) EchoBuffer[iEcho] = output;			//for decaying echo
else EchoBuffer[iEcho]=input;								//for single echo (delay)
EchoLengthB += Direction;									//alter the echo length
if(EchoLengthB<EchoMin+100){Direction=1;}//if echo delay is shortest->make it
if(EchoLengthB>EchoMax){Direction=-1;}	  //longer and if longest_>shorter
input=output;													//output echo->harmonics gen
if(HarmBool==1) { 											//everyother sample...
  HarmBool=0;													//switch the count
  HarmBuffer[iHarm]=input;							  		//store sample in buffer
  if(HarmBool2==1){ 											//everyother sample...
	HarmBool2=0;												//switch the count
	HarmBuffer[uHarm] += SecHarmAmp*.025*input;		//store sample in buffer
  }
  else{HarmBool2=1; uHarm++; 								//or just switch the count,
 	if(uHarm>HarmLength) uHarm=0; 						//and increment the pointer
  }
}
else{HarmBool=1; iHarm++; 									//or just switch the count
if(iHarm>HarmLength) iHarm=0;}							//and increment the pointer
output=input+HarmAmp*0.0125*HarmBuffer[jHarm];		//add harmonics to output
jHarm++;															//and increment the pointer
if(jHarm>HarmLength) jHarm=0;								//reinit when maxed out
DrumDelay--;													//decrement delay counter
if(DrumDelay<1) {                            		//drum section
 	DrumDelay=50000-Tempo;									//if time for drumbeat
 	DrumOn=1;													//turn it on
}
if(0){															//if drum is on
 	output=output+(kick[iDrum])*.05*(ampDrum);		//play next sample
 	if((sDrum%2)==1) {iDrum++;}							//but play at Fs/2
 	sDrum++;														//incr sample number
 	if(iDrum>2500){iDrum=0; DrumOn=0;}					//drum off if last sample
}
output = output*.0001*VolSlider*VolSlider;
AIC23_data.channel[LEFT]=output;
AIC23_data.channel[RIGHT]=AIC23_data.channel[LEFT];
output_sample(AIC23_data.uint);   						//output to both channels
}
main()                   	//init DSK,codec,McBSP and while(1) infinite loop

