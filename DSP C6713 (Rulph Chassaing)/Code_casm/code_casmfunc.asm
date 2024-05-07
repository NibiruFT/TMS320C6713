;Code_casmfunc.asm ASM function determines if code matches slider values

		.def	_codefunc	;ASM function called from C
_codefunc:	MV	A8, A2	;correct code
		MV   	B8, B2
		MV	A10, A7
		MV   	B10, B7

		CMPEQ	A2,A4,A1	;compare first digit(A1=1 if A2=A4) 
		CMPEQ	A1,0,A1	;otherwise A1=0
	[A1]	B	DONE		;done if A1=0 since no match
		NOP	5
		
		MV    B2,A2
		CMPEQ A2,B4,A1	;compare second digit
		CMPEQ	A1,0,A1 
	[A1]	B	DONE
            NOP	5
				   
		MV    A7,A2
		CMPEQ	A2,A6,A1	;compare third digit
		CMPEQ	A1,0,A1 
	[A1]	B	DONE
            NOP   5
                   
            MV    B7,A2
		CMPEQ	A2,B6,A1	;compare fourth digit
   		CMPEQ	A1,0,A1 
DONE:		MV	A1,A4		;return 1 if complete match
		B	B3		;return to C program
	      NOP	5
		.end

