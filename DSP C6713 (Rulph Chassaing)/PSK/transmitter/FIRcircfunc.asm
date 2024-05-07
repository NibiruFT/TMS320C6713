;FIRcircfunc.asm ASM function called from C using circular addressing
;A4=newest sample, B4=coefficient address, A6=filter order 
;Delay samples organized: x[n-(N-1)]...x[n]; coeff as h(0)...h[N-1]

            .def  _fircircfunc
            .def   last_addr 
            .def   delays  
	      .sect "circdata"   ;circular data section
            .align 512         ;align delay buffer 512-byte boundary
delays      .space 512         ;init 512-byte buffer with 0's
last_addr   .int   last_addr-1 ;point to bottom of delays buffer
            .text		       ;code section
_fircircfunc:			 ;FIR function using circ addr                    
            MV    A6,A1    	 ;setup loop count 
 	      MPY   A6,2,A6      ;since dly buffer data as byte
	      ZERO  A8   	       ;init A8 for accumulation				   		
	      ADD   A6,B4,B4	 ;since coeff buffer data as bytes
	      SUB   B4,1,B4      ;B4=bottom coeff array h[N-1] 
    
            MVKL 0x00080040,B6 ;select A7 as pointer and BK0
            MVKH 0x00080040,B6 ;BK0 for 512 bytes (256 shorts) 
       
            MVC   B6,AMR       ;set address mode register AMR		

	      MVKL  last_addr,A9  ;A9=last circ addr(lower 16 bits)
	      MVKH last_addr,A9  ;last circ addr (higher 16 bits)		
            LDW   *A9,A7       ;A7=last circ addr
            NOP   4 
            STH   A4,*A7++	 ;newest sample-->last address

loop:					 ;begin FIR loop
            LDH   *A7++,A2     ;A2=x[n-(N-1)+i] i=0,1,...,N-1  
    ||      LDH   *B4--,B2     ;B2=h[N-1-i] i=0,1,...,N-1
            SUB   A1,1,A1	 ;decrement count
    [A1]    B     loop    	 ;branch to loop if count # 0
            NOP   2
 	      MPY   A2,B2,A6     ;A6=x[n-(N-1)+i]*h[N-1+i]
            NOP   
            ADD   A6,A8,A8     ;accumulate in A8 			
          
            STW   A7,*A9	 ;store last circ addr to last_addr
            B     B3           ;return addr to calling routine
            MV    A8,A4	       ;result returned in A4
            NOP   4