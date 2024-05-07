//Sum.c Finds n+(n-1)+...+1.Calls assembly function sumfunc.asm

#include <stdio.h>

main()
{
 short n=6;			     //set value
 short result;    	     //result from asm function 

 result = sumfunc(n);        //call assembly function sumfunc
 printf("sum = %d", result); //print result from asm function
}
            
