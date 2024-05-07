;FIRCASMfunc.asm ASM function called from C to implement FIR 
;A4 = Samples address, B4 = coeff address, A6 = filter order 
;Delays organized as:x(n-(N-1))...x(n);coeff as h[0]...h[N-1]     

		.def	_fircasmfunc	
_fircasmfunc:			;ASM function called from C
		MV	A6,A1       ;setup loop count 
 		MPY   A6,2,A6	;since dly buffer data as byte
	 	ZERO 	A8          ;init A8 for accumulation	
		ADD   A6,B4,B4    ;since coeff buffer data as byte
		SUB   B4,1,B4     ;B4=bottom coeff array h[N-1] 
loop:					;start of FIR loop  
		LDH 	*A4++,A2    ;A2=x[n-(N-1)+i] i=0,1,...,N-1
		LDH 	*B4--,B2    ;B2=h[N-1-i] i=0,1,...,N-1
		NOP	4
 		MPY 	A2,B2,A6    ;A6=x[n-(N-1)+i]*h[N-1-i] 
 		NOP
 		ADD 	A6,A8,A8    ;accumlate in A8 		
  		LDH 	*A4,A7      ;A7=x[(n-(N-1)+i+1]update delays
  		NOP	4	      ;using data move "up"
 		STH 	A7,*-A4[1]  ;-->x[(n-(N-1)+i] update sample
  		SUB 	A1,1,A1	;decrement loop count
    [A1]	B   	loop  	;branch to loop if count # 0
	      NOP	5

		MV  	A8,A4       ;result returned in A4
		B   	B3	      ;return addr to calling routine
		NOP	4
