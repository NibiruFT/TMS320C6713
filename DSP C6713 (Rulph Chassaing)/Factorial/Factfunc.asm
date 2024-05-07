;Factfunc.asm  Assembly function called from C to find factorial
    
         	.def	_factfunc		;asm function called from C
_factfunc:	MV	A4,A1     		;setup loop count in A1		
     		SUB	A1,1,A1  		;decrement loop count
LOOP:   	MPY   A4,A1,A4 		;accumulate in A4
           	NOP            		;for 1 delay slot with MPY
           	SUB   A1,1,A1   		;decrement for next multiply
    [A1]   	B     LOOP      		;branch to LOOP if A1 # 0
	      NOP	5        	 	;five NOPs for delay slots
            B   	B3	    		;return to calling routine 
            NOP	5			;five NOPs for delay slots
            .end
