;Noisegen_casmfunc.asm Noise generation C-called function

		.def	_noisefunc  ;ASM function called from C
_noisefunc	ZERO	A2		;init A2 for seed manipulation
		MV	A4,A1		;seed in A1	
		SHR	A1,17,A1	;shift right 17->bit 17 to LSB 
		ADD	A1,A2,A2	;add A1 to A2 => A2
		SHR	A1,11,A1	;shift right 11->bit 28 to LSB 
		ADD	A1,A2,A2	;add again
		SHR	A1,2,A1	;shift right 2->bit 30 to LSB 
		ADD	A1,A2,A2	;
		SHR	A1,1,A1	;shift right 1->bit 31 to LSB 
		ADD	A1,A2,A2	;
		AND	A2,1,A2	;Mask LSB of A2
		SHL	A4,1,A4	;shift seed left 1 
		OR	A2,A4,A4	;Put A2 into LSB of A4
		B	B3		;return to calling function
		NOP	5		;5 delays for branch
