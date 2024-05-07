//Code_casm.c  Calls ASM function. Determines if code chosen match with slider values

#include <stdio.h>
short digit1=1, digit2=1, digit3=1, digit4=1; 		//initialize slider values

main()
{
  short code1=1, code2=2, code3=2, code4=4;										//initialize code
  short result;	
  DSK6713_init();											//init BSL
  DSK6713_DIP_init();  									//init dip switches on DSK

 while(DSK6713_DIP_get(3) == 1)						//continue until SW #3 is pressed
 {
  if(DSK6713_DIP_get(0) == 0)							//true if DIP SW #0 is pressed
  {                                             //call ASM function
  result=codefunc(digit1,digit2,digit3,digit4,code1,code2,code3,code4);
  if(result==0) printf("correct match\n");		//result from ASM function
  else printf("no match\n");
  }
 }
}

