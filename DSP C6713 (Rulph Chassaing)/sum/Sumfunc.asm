;SUMFUNC.ASM Assembly function to find n+(n-1)+...+1

	    .def	  _sumfunc ;function called from C
_sumfunc: MV   .L1  A4,A1    ;setup n as loop counter
	    SUB  .S1  A1,1,A1  ;decrement n

LOOP:     ADD  .L1  A4,A1,A4 ;accumulate in A4
          SUB  .S1  A1,1,A1  ;decrement loop counter
    [A1]  B	   .S2  LOOP     ;branch to LOOP if A1#0
          NOP	  5        ;five NOPs for delay slots
          B    .S2  B3	     ;return to calling routine
          NOP	  5        ;five NOPs for delay slots
          .end