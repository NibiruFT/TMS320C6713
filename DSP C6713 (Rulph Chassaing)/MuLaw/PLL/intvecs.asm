;**********************************************************************
;*		 (C) COPYRIGHT TEXAS INSTRUMENTS, INC. 1996 				  *
;*																	  *
;*		Interrupt Vector Assignments for 'C6x						  *
;**********************************************************************
	.title "Interrupt Vectors w/ RTDX for C6X"
	.tab	4

SP	.set	B15					; Redefine for convenience

;**********************************************************************
;*	   Interrupt Service Table (IST)
;* 
;* This is a table of Interrupt Service Fetch Packets (ISFP) which
;* contain code for servicing the interrupts.
;* This serves as a multi-purpose example for the C6x family.
;* Consult the TI DSP Reference manual to determine which interrupts 
;*	 apply to a given target and/or application.
;**********************************************************************
	.sect	".intvecs"
	.align	32*8*4				; must be aligned on 256 word boundary
RESET_V 			; Reset ISFP
	.ref	_c_int00			; program reset address
	MVKL 	_c_int00, B3
	MVKH	_c_int00, B3
	B		B3
	MVC 	PCE1, B0			; address of interrupt vectors
	MVC 	B0, ISTP			; set table to point here
	NOP 	3

	.align	32
NMI_V				; Non-maskable interrupt Vector
	B		NRP
	NOP 	5

	.align	32
AINT_V				; Analysis Interrupt Vector (reserved)
	B		$
	NOP 	5

	.align	32
MSGINT_V			; Message Interrupt Vector (reserved)
	.ref	RTEMU_msg
	STW 	B0, *SP--[2]
||	MVKL 	RTEMU_msg, B0
	MVKH	RTEMU_msg, B0
	B		B0
	LDW 	*++SP[2], B0
	NOP 	4

	.align	32
INT4_V				; Maskable Interrupt #4
	B		$
	NOP 	5

	.align	32
INT5_V				; Maskable Interrupt #5
	B		$
	NOP 5

	.align	32
INT6_V				; Maskable Interrupt #6
	B		$
	NOP 	5

	.align	32
INT7_V				; Maskable Interrupt #7
	B		$
	NOP 	5

	.align	32
INT8_V				; Maskable Interrupt #8
	B		$
	NOP 	5

	.align	32
INT9_V				; Maskable Interrupt #9
	B		$
	NOP 	5

	.align	32
INT10_V 			; Maskable Interrupt #10
	B		$
	NOP 	5

	.align	32
INT11_V 			; Maskable Interrupt #11
	B		$
	NOP 	5

	.align	32
INT12_V 			; Maskable Interrupt #12
	B		$
	NOP 	5

	.align	32
INT13_V 			; Maskable Interrupt #13
	B		$
	NOP 	5

	.align	32
INT14_V 			; Maskable Interrupt #14
	B		$
	NOP 	5

	.align	32
INT15_V 			; Maskable Interrupt #15
	B		$
	NOP 	5

; the remainder of the vector table is reserved

	.align	32*8*4				; reserve full 256 words

	.end
