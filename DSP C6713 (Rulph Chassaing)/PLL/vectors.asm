
*VECTORS.ASM provided by TI to set up interrupt vectors
*If ISR is used, put branch to it in appropriate vector location
*  .ref    ISR if interrupt-driven

   .ref		_c_int00
   .sect    	"vectors"

RESET_RST:
		mvkl .S2 _c_int00, B0
		mvkh .S2 _c_int00, B0
		B    .S2 B0
		NOP  	5
	
NMI_RST:	NOP	8

RESV1:		NOP     8

RESV2:		NOP     8
	
INT4:   	NOP	8
	
INT5: 		NOP 	8
	                  
INT6:		NOP	8
	
INT7:		NOP	8
	
INT8:		NOP	8
	
INT9: 		NOP	8
	
INT10:		NOP	8
	
INT11:  	NOP	8
*for interrupt-driven,use b ISR with 7 NOPs

INT12: 		NOP	8
	
INT13:		NOP	8
	
INT14: 		NOP	8
	
INT15: 		NOP	8
	