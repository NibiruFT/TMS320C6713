//Dotp4clasm.c Multiplies two arrays using C calling linear ASM func

short dotp4clasmfunc(short *a,short *b,short ncount);  //prototype
#include <stdio.h>		              //for printing statement
#include "dotp4.h"                          //arrays of data values
#define  count 4                            //number of data values
short x[count] = {x_array};			  //declare 1st array
short y[count] = {y_array};   		  //declare 2nd array
volatile int result = 0;    		        //result

main()
{
  result = dotp4clasmfunc(x,y,count);	  //call linear ASM func
  printf("result = %d decimal \n", result); //print result
}
            
