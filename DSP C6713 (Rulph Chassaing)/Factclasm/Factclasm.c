//Factclasm.c Factorial of number. Calls linear ASM function 

#include <stdio.h>		     //for print statement
	
void main()
{
 short number = 7;		     //set value
 short result;    	           //result of factorial

 result = factclasmfunc(number);   //call ASM function factlasmfunc
 printf("factorial = %d", result); //print from linear ASM function
}
            
