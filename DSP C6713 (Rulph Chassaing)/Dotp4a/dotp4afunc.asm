;Dotp4afunc.asm Multiply two arrays. Called from dotp4a_init.asm
;A4=x address,B4=y address,A6=count(size of array),B3=return address
	
	 	.def		dotp4afunc  ;dot product function
       	.text		     		;text section
dotp4afunc	MV		A6,A1	      ;move loop count -->A1
	 	ZERO  	A7	      ;init A7 for accumulation
	
loop	 	LDH 		*A4++,A2    ;A2=(x. A4 as address pointer
 	 	LDH 		*B4++,B2    ;B2=(y). B4 as address pointer
	      NOP		4	      ;4 delay slots for LDH
	      MPY	.M1x	B2,A2,A3    ;A3 = x * y
	      NOP		     		;1 delay slot for MPY	
	      ADD		A3,A7,A7    ;sum of products in A7
            SUB		A1,1,A1     ;decrement loop counter
    [A1]    B		loop	      ;branch back to loop till A1=0
	      NOP		5	      ;5 delay slots for branch
	      
		MV		A7,A4	      ;A4=result A4=return register
	      B		B3          ;return from func to addr in B3
	      NOP 		5	      ;5 delay slots for branch
