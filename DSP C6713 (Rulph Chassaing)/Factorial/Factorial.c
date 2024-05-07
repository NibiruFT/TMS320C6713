//Factorial.c Finds factorial of n. Calls function factfunc.asm

#include <stdio.h>		     //for print statement

void main()
{
 short n=7;				     //set value
 short result;    	           //result from asm function 

 result = factfunc(n);             //call assembly function factfunc
 printf("factorial = %d", result); //print result from asm function
}
            
