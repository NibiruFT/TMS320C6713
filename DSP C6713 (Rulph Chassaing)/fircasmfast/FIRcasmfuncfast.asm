;FIRCASMfuncfast.asm C-called faster function to implement FIR 
		.def	_fircasmfunc	
_fircasmfunc:			;ASM function called from C
		MV	A6,A1       ;setup loop count 
 		MPY   A6,2,A6	;since dly buffer data as byte
	 	ZERO 	A8          ;init A8 for accumulation	
		ADD   A6,B4,B4    ;since coeff buffer data as byte
		SUB   B4,1,B4     ;B4=bottom coeff array h[N-1] 
loop:					;start of FIR loop  
		LDH 	*A4++,A2    ;A2=x[n-(N-1)+i] i=0,1,...,N-1
   ||	      LDH 	*B4--,B2    ;B2=h[N-1-i] i=0,1,...,N-1
		SUB	A1,1,A1	;decrement loop count
   ||  	LDH 	*A4,A7      ;A7=x[(n-(N-1)+i+1]update delays
		NOP	4
		STH 	A7,*-A4[1]  ;-->x[(n-(N-1)+i] update sample
   [A1]	B   	loop  	;branch to loop if count # 0
		NOP	2
 		MPY 	A2,B2,A6    ;A6=x[n-(N-1)+i]*h[N-1-i] 
 		NOP
 		ADD 	A6,A8,A8    ;accumlate in A8 		
		
		B   	B3	      ;return addr to calling routine
		MV  	A8,A4       ;result returned in A4
		NOP	4
 	  		

				
