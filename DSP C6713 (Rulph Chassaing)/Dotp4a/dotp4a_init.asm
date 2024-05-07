;Dotp4a_init.asm ASM program to init variables. Calls dotp4afunc.asm

	      .def	init       ;starting address
	      .ref	dotp4afunc ;called ASM function
	      .text			      ;section for code
x_addr    .short	 1,2,3,4    ;numbers in x array
y_addr    .short 	 0,2,4,6    ;numbers in y array
result_addr .short 	 0	      ;initialize sum of products
					
init      MVK	result_addr,A4    ;A4=result addr
	      MVK	0,A3 		      ;A3=0
	      STH	A3,*A4	      ;init result to 0
	      MVK 	x_addr,A4		  ;A4=address of x
	      MVK	y_addr,B4	      ;B4=address of y 
	      MVK	4,A6		      ;A6=size of array
	      B	dotp4afunc	      ;branch to function dotp4afunc 
	      MVK	ret_addr,b3	      ;B3=return addr from dotp4a
	      NOP	3                 ;3 more delay slots(branch)

ret_addr  MVK	result_addr,A0    ;A0=result_addr
          STH	A4,*A0	      ;store result 
wait	  B		wait		      ;wait here
	      NOP	5		      ;delay slots for branch
