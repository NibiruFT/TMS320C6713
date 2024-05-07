;Sinegencasmfunc.asm ASM func to generate sine using DE
;A4 = address of y array, B4 = A

		.def	_sinegencasmfunc			;ASM function called from C
_sinegencasmfunc:					
		LDH		*+A4[0], A5			;y[n-2]-->A5
		LDH		*+A4[1], A2			;y[n-1]-->A2
		LDH		*+A4[2], A3			;y[n]-->A3
		NOP		3				;NOP due to LDH
		MPY		B4, A2, A8			;A*y[n-1]
		NOP		1				;NOP due to MPY
		SHR		A8, 14, A8			;shift right by 14
		SUB		A8, A5, A8			;A*y[n-1]-y[n-2]
		STH		A8, *+A4[2]			;y[n]=A*y[n-1]-y[n-2]		
		STH		A2, *+A4[0]			;y[n-2]=y[n-1]
		STH 	A8, *+A4[1]				;y[n-1] = y[n]
		B   	B3	      			;return addr to calling routine
		NOP		5				;delays to to branching
		.end

