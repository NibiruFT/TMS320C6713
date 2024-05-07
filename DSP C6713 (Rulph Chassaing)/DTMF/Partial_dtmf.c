//DTMF.c (Partial)Decodes DTMF signals and turn on corresponding LEDs
#define N	100
short i; short buffer[N]; short sin697[N],cos697[N],sin770[N],cos770[N];...
long weight697,weight697_sin,weight697_cos; long ...weight1477_cos;
long weight1,weight2,choice1,choice2;
interrupt void c_int11()
{
 for (i = N-1; i > 0; i--)
	buffer[i]=buffer[i-1];              	// initialize buffer
 buffer[0]=input_sample();    				//input sample into buffer
 output_sample(buffer[0]*10); 				//output
 weight697_sin=0;  weight697_cos=0;   		//sine/cosine weight @ each freq
 ...
 weight1477_sin=0;  weight1477_cos=0;
 for (i = 0; i < N; i++)
 {
  weight697_sin=weight697_sin+buffer[i]*sin697[i];
  weight697_cos=weight697_cos+buffer[i]*cos697[i];
  ...
  weight1477_cos=weight1477_cos+buffer[i]*cos1477[i];
 }
 //for each freq compare sine weight and cosine weight and choose largest
 if(abs(weight697_sin)>abs(weight697_cos)) weight697=abs(weight697_sin);
 	else weight697=abs(weight697_cos);
 ...
 if(abs(weight1477_sin)>abs(weight1477_cos)) weight1477=abs(weight1477_sin);
 	else weight1477=abs(weight1477_cos);

 weight1=weight697; choice1=1;//among weight697,... weight941 find largest
 if(weight770>weight1) {weight1=weight770; choice1=2;} //...
 if(weight941>weight1) {weight1=weight941; choice1=4;}
 weight2=weight1209; choice2=1;//among weight1209,...weight1477 find largest
 if(weight1336>weight2) {weight2=weight1336;  choice2=2;}
 if(weight1477>weight2) {weight2=weight1477;  choice2=3;}
 if((weight1>40000)&&(weight2>40000)) //set a threshhold
 {  // depending on choices1 and 2 turn on corresponding LEDs
  if((choice1==1)&&(choice2==1)) //button "1" -> 0001
  {DSK6713_LED_off(0);DSK6713_LED_off(1);DSK6713_LED_off(2);DSK6713_LED_on(3);}
  ...
  if((choice1==4)&&(choice2==3)) //button "#" -> 1100
  {DSK6713_LED_on(0);DSK6713_LED_on(1);DSK6713_LED_off(2);DSK6713_LED_off(3);}
 }else { //weights below threshold, turn LEDs off
  DSK6713_LED_off(0);DSK6713_LED_off(1);DSK6713_LED_off(2);DSK6713_LED_off(3);
  }
 return;
}
void main()
{
  DSK6713_LED_init();
  DSK6713_LED_off(0);DSK6713_LED_off(1);DSK6713_LED_off(2);DSK6713_LED_off(3);
  for (i = 0; i < N; i++)   //define sine/cosine for all 7 frequencies
  {
   buffer[i]=0;
   sin697[i]=1000*sin(2*3.14159*i/8000.*697);
   cos697[i]=1000*cos(2*3.14159*i/8000.*697);
   ...
   cos1477[i]=1000*cos(2*3.14159*i/8000.*1477);
  }
  comm_intr();   while(1); 		//init DSK, codec, McBSP;  infinite loop
}

