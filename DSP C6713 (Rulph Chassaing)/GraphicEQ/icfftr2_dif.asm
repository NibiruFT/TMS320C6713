;File was assembled with assembler version 4.0 (included with CCS V1.2) and
;NOT with assembler version 4.1 (included with CCS V2) due to "BUG" SDSsq21569.
;icfftr2_dif.asm file used as source in project in lieu of icfftr2_dif.sa. 
;RC,August 2001.


;******************************************************************************
;* TMS320C6x ANSI C Codegen                                      Version 4.00 *
;* Date/Time created: Mon Aug 20 20:26:58 2001                                *
;******************************************************************************

;******************************************************************************
;* GLOBAL FILE PARAMETERS                                                     *
;*                                                                            *
;*   Architecture      : TMS320C671x                                          *
;*   Optimization      : Enabled at level 1                                   *
;*   Optimizing for    : Compile time 1st, speed 2nd                          *
;*                       Based on options: -o1, no -ms                        *
;*   Endian            : Little                                               *
;*   Interrupt Thrshld : Disabled                                             *
;*   Memory Model      : Small                                                *
;*   Calls to RTS      : Near                                                 *
;*   Pipelining        : Disabled                                             *
;*   Memory Aliases    : Presume are aliases (pessimistic)                    *
;*   Debug Info        : Debug                                                *
;*                                                                            *
;******************************************************************************

FP	.set	A15
DP	.set	B14
SP	.set	B15
	.global	$bss

	.file	"icfftr2_dif.sa"
*=============================================================================== 
*
* From: https://www-a.ti.com/apps/c6000/xt_download.asp?sku=C67x_icfftr2
* 
*     TEXAS INSTRUMENTS, INC. 
* 
*     INVERSE COMPLEX RADIX-2 DECIMATION-IN-FREQUENCY FFT 
* 
*     Revision Date: 07/06/98
* 
*     USAGE 
*           This routine is C-callable and can be called as: 
* 
*           void icfftr2_dif(float* x, float* w, short n) 
* 
*		x[] --- input and output sequences (dim-n)      (input/output)
*			x has n complex numbers (2*n SP values).  
*			The real and imaginary values are interleaved
*			in memory: re0:im0, re1:im1, .....
*               w[]  --- FFT coefficients (dim-n/2)             (input)
*			w has n/2 complex numbers (n SP values). 
*			FFT coeficients must be in bt-reversed order
*			The real and imaginary values are interleaved
*			in memory: re0:im0, re1:im1, .....
*               n    --- FFT size                               (input)       
* 
*           If the routine is not to be used as a C-callable function, 
*           then all instructions relating to the stack should be removed. 
*           See comments of individual instructions to determine if they are 
*           related to the stack. You also need to initialize all passed 
*           parameters since these are assumed to be in registers as defined by 
*           the calling convention of the compiler, (See the C compiler 
*           reference guide.) 
*
*     C CODE 
*           This is the C equivalent of the assembly code without restrictions: 
*           Note that the assembly code is hand optimized and restrictions may 
*           apply. 
* 
*		void icfftr2_dif(float* x, float* w, short n)
*		{
*		   short n2, ie, ia, i, j, k, m;
*		   float rtemp, itemp, c, s;
*		
*		   n2 = 1;
*		   ie = n;
*		   for(k=n; k > 1; k >>= 1)
*		   {
*		      ie >>= 1;
*		      ia = 0;
*		      for(j=0; j < ie; j++)
*		      {
*		         c = w[2*j];
*		         s = w[2*j+1];
*		         for(i=0; i < n2; i++)
*		         {
*		            m = ia + n2;
*		            rtemp     = x[2*ia]   - x[2*m];
*		            x[2*ia]   = x[2*ia]   + x[2*m];
*		            itemp     = x[2*ia+1] - x[2*m+1];
*		            x[2*ia+1] = x[2*ia+1] + x[2*m+1];
*		            x[2*m]    = c*rtemp   - s*itemp;
*		            x[2*m+1]  = c*itemp   + s*rtemp;
*			    ia++;
*		         }
*		         ia += n2;
*		      }
*		      n2 <<= 1;
*		   }
*		}
* 
*       DESCRIPTION
*		This routine is used to compute the Inverse, Complex, Radix-2, 
*		Decimation-in-Frequency Fast Fourier Transform of a single
*		precision complex sequence of size n, and a power of 2.
* 		The routine requires bit-reversed input and bit-reversed 
*		coefficents (twiddle factors) and produces results that are
* 		in normal order.  Final scaling is not done in this function.
*
*       TECHNIQUES
*            1. Loading input x as well as coefficient w in double word.
*            2. Both loops j and i shown in the C code are placed in the
*               INNERLOOP of the assembly code.
*            3. mpy was used to perform a mv.  EX. mpy x, 1, y <=> mv x, y
*	     4. Because the data loads are 1 itteration ahead of the 
*		coefficent loads, counter i was copied to counter m so that
*	 	the actual count could live longer for the coefficent loads.
*            5. Two output pointers/counters are maintained to remove the
*		dependency between the X'a and Y's - the Y's have a much longer
*		latency path than the X's.
*	     6.	Inner loop prolog and epilog are done in parallel with the 
*		outer loop.
*
*       ASSUMPTIONS
*		n >= 8
*
*               Both input x and coefficient w should be aligned on double word
*               (8 byte) boundary.
* 	
*		The follwoing C code is used to generate the coefficient table 
*		(non-bit reversed).
*
*		#include <math.h>
*		/* generate real and imaginary twiddle 
*			table of size n/2 complex numbers */
*
*		gen_w_r2(float* w, int n)
*		{
*	   	int i;
*	   	float pi = 4.0*atan(1.0);
*	   	float e = pi*2.0/n;
*
*	   		for(i=0; i < ( n>>1 ); i++)
*	   		{
*	     			w[2*i]   = cos(i*e);
*	     			w[2*i+1] = sin(i*e);
*	   		}
*		}
*
*
* 		The follwoing C code is used to bit-reverse the coefficents.
*
*		bit_rev(float* x, int n)
*		{
*		  int i, j, k;
*		  float rtemp, itemp;
*
*		  j = 0;
*		  for(i=1; i < (n-1); i++)
*		  {
*		     k = n >> 1;
*		     while(k <= j)
*		     {
*		        j -= k;
*		        k >>= 1;
*		     }
*		     j += k;
*		     if(i < j)
*		     {
*		        rtemp    = x[j*2];
*		        x[j*2]   = x[i*2];
*		        x[i*2]   = rtemp;
*		        itemp    = x[j*2+1];
*		        x[j*2+1] = x[i*2+1];
*		        x[i*2+1] = itemp;
*		     }
*		  }
*		}
*
* 		The follwoing C code is used to perform the final scaling 
*		of the IFFT.
*
*		/* divide each element of x by n */
*		divide(float* x, int n)
*		{
*		   int i;
*		   float inv = 1.0 / n;
*
*		   for(i=0; i < n; i++)
*		   {
*		     x[2*i]   = inv * x[2*i];
*		     x[2*i+1] = inv * x[2*i+1];
*		   }
*		}
*
*
* 	MEMORY NOTE
*
*		Data (x)		8*N    bytes
*		Coefficients (w)	8*N/2  bytes
*		Stack			4*10   bytes
*		Program			800    bytes
*	
*		Note 1:  Data and Coefficents must reside in different memory
*		blocks to avoid memory conflicts.
*
*		Note 2:	 Data and Coefficents must be aligned to an 8 byte 
*		boundary.
*
*       CYCLES 
*
*	   # of cycles = 21 + 4 + M*((N/2-2)*4 + 24)
*                        /    \         \         \
*    C preservation  ___/      \         \         \___ Loop L Prolog/Epilog
*                               \         \                     +
*                                \         \                  Loop K
*                                 \         \
*                                  \         \___ Loop L
*                                   \
*                                    \___ Loop K Prolog
*
*		where:  N is the number of point in the IFFT
*			M = log(base 2)N, is the number of stages in the IFFT
*
*	Example:  1024 Point FFT Performance
*		N = 1024, M = 10, assume a 167MHz CPU clock
*
*		# of cycles = 21 + 4 + 10*((1024/2-2)*4 + 24) = 20665
*
*		time = # of cycles * CPU clock = 20665/167*10^6 = 124.0 usec
*
*
* 	EXAMPLE USAGE
*
* void main(void)
* {
*    gen_w_r2(w, N);         // Generate coefficient table
*    bit_rev(w, N>>1);       // Bit-reverse coefficient table
*    cfftr2_dit(x, w, N);    // This is the radix 2 FFT benchmark available 
*			     // from TI
*                            // input in normal order, output in bit-reversed 
*			     // order
*                            // coefficient table in bit-reversed order
*    icfftr2_dif(x, w, N);   // Inverse radix 2 FFT
*                            // input in bit-reversed order, output in normal 
*			     // order
*                            // coefficient table in bit-reversed order
*    divide(x, N);           // scale inverse FFT output
*                            // result is the same as original input to FFT
* }
*
*	Since the twiddle table is in bit-reversed order, it turns out that
*	the same twiddle table will also work for smaller IFFTs.  This
*	means that if you need to do both 512 and 1024 point IFFTs in the
*	same application, you only need to have the 1024 point twiddle
*	table.  The 512 point FFT will use the first half of the 1024
*	point twiddle table.
*
*
* 	IMPLEMENTATION
*
*	The above C implemetation of the IFFT has been modified to better fit
*	the 'C67xx architecture thus allowing the translation from C to hand
*	coded assembly easier.  The modified function is listed below and is 
*	functionally equivelent to the above function.  Note, the C statements
*	in this function are used as comment for the equivelent assembly
*	statements (see the optimized assembly listeing).
*
* void icfftr2_dif(float* x, float* w, short n)
* {
*	short n2, i, k, l, p, m, j, n2A;
*	float rtemp, itemp, s, c, xr, xi, yr, yi, Xr, Xi, Yr, Yi;
*	float *wptrB, *xinptrA, *xoutptrB, *xoutptrA, p1r, p2r, p1i, p2i;
*	
*	n2 = 1;
*	wptrB = w;
*	xinptrA = x; 
*	xoutptrB = x; 	
*	c=*wptrB++; 
*	s=*wptrB++;
*	xoutptrA = xoutptrB + 2*n2;
*	xoutptrB = 1 + xoutptrB;
*	i = n2;
*	j = n2;
*	p = n2;
*	
*	for(k=n; k > 1; k >>= 1)
*	{
*		for (l=0; l<n/2; l++)
*		{
*			yr = xinptrA[2*n2];
*			yi = xinptrA[2*n2 + 1];
*			xr = *xinptrA++; 
*			xi = *xinptrA++;
*			j = j - 1;
*			i = i - 1;	
*			p = p -1;  
*			itemp = xi - yi;
*			rtemp = xr - yr;
*			Xi = xi + yi;
*			Xr = xr + yr;
*			if (i==0) xinptrA = xinptrA + 2*n2; 
*			p1i = c*itemp;
*			p2i = s*rtemp;
* 			p1r = c*rtemp;
* 			p2r = s*itemp;
*			*xoutptrB = Xi;
*			xoutptrB = xoutptrB + 2;
*			*(xoutptrB - 3) = Xr;		
*			Yi = p1i + p2i;
*			Yr = p1r - p2r;
*			if (p==0) xoutptrB = xoutptrB + 2*n2;
*			m = i; 	
*			if (i==0) i = n2;
*			*xoutptrA++ = Yr;
*			*xoutptrA++ = Yi;
*			if (j==0) xoutptrA = xoutptrA + 2*n2;
*			if (m==0) {
*				c=*wptrB++; 
*				s=*wptrB++;
*			}
*			if (p==0) p = n2;	
* 			if (j==0) j = n2;
*		}
*	
*		n2 = n2 << 1;
*		xinptrA = x;
*		xoutptrB = x;
*		wptrB = w;  	
*		c=*wptrB++; 
*		s=*wptrB++;
*		i = n2;
*		j = n2;
*		p = n2;
*		xoutptrA = xoutptrB + 2*n2;
*		xoutptrB = xoutptrB + 1;	
*	}
* }
*
*
*       NOTATIONS
*
*               f = Function Prolog or Epilog
*               o = Outer Loop
*               p = Inner Loop Prolog
*		e = Inner Loop Epilog
*
*===============================================================================
	;void icfftr2_dif(float* x, float* w, short n)
	; {

	.def _icfftr2_dif

_icfftr2_dif:	; .cproc	x, w, n

   	; short n2, i, k, l, nd2, n2A;
   	; float rtemp, itemp, s, c, xr, xi, yr, yi, Xr, Xi, Yr, Yi;
   	; float *wptrB, *xinptrA, *xoutptrB, *xoutptrA, p1r, p2r, p1i;
   	; float p2i;
   	; short n2p1;

p1r		.set	A0
p		.set	A1
k		.set	A2
l		.set	A2
p2r		.set	A3
c		.set 	A4
s		.set	A5
yr		.set 	A6
yi		.set	A7
rtemp		.set	A8
itemp		.set	A9
Yr		.set	A10	
Yi		.set 	A11
wptrB		.set	A12
xoutptrB	.set	A13
p1i		.set	A14
p2i		.set	A15

j		.set	B0
i		.set	B1
m		.set	B2
n2		.set	B3
xr		.set	B4
xi		.set	B5
x		.set	B6
w		.set	B7
n		.set	B8
Xr		.set	B9
Xi		.set	B10
xinptrA		.set 	B11
xoutptrA	.set	B12
n2As		.set	B13
tmpk		.set	B14

	; ----------------- function prolog  --------------------

 ; preserve "save-on-call" registers

	sub		B15, 4, A0

	stw	.D2 	A10, *B15--[2]		; f
 ||	stw	.D1 	B10, *A0--[2]		; f

	stw	.D2 	A11, *B15--[2]		; f
 ||	stw	.D1 	B11, *A0--[2]		; f

	stw	.D2 	A12, *B15--[2]		; f
 ||	stw	.D1 	B12, *A0--[2]		; f

	stw	.D2 	A13, *B15--[2]		; f
 ||	stw	.D1 	B13, *A0--[2]		; f
 ||     mvc     .S2     CSR,B13			; f 

	stw	.D2 	A14, *B15--[2]		; f
 ||	stw	.D1 	B14, *A0--[2]		; f
 ||     and     .L2     -2,B13,B13		; f

	stw	.D2 	A15, *B15--[2]		; f
 ||	stw	.D1	B3, *A0--[2]		; f
 ||     mvc     .S2     B13,CSR			; f disable global interrupts

	mv	.L2x	A4, xinptrA		; f move arg1 to x
 ||	mv	.D2	B4, w			; f move arg2 to w
 ||	mvk	.S2 	1, n2			; o n2 = 1;

	; ----------------- prolog for loopl --------------------   			
	
   	mv	.L1x	w, wptrB		; o wptrB = w;
 ||  	mv	.L2	xinptrA, x		; o xinptrA = x;	
 ||	lddw	.D2	*+xinptrA[n2], A7:A6	; p yr = *xinptr++; 
						;   xi = *xinptr++; 
 ||	mpy	.M2	n2, 1, i		; o i = n2;
 ||	mv	.S2x	A6, n			; f move arg3 to n

  	mv	.S1X	x, xoutptrB		; o xoutptrB = x; 
 ||	lddw	.D2	*xinptrA++, B5:B4	; p xr = *xinptrA++
						;   xi = *xinptrA++
 ||	shr	.S2	n, 2, tmpk		; o tmpk = n/2
	
	shr	.S1x	n, 1, l			; o l = n/2;
 || 	shl	.S2	n2, 3, n2As		; o n2As = n2<<3;
 ||[i]	sub	.L2	i, 1, i			; p i = i - 1; 

  	add	.L2X	xoutptrB, n2As, xoutptrA; o xoutptrA = xoutptrB + n2;
 ||	add	.D1	xoutptrB, 4, xoutptrB	; o xoutptrB = 4 + xoutptrB
 ||	mv	.D2	n2, j			; o j = n2;
 ||	mv	.S1X	n2, p			; o p = n2
 ||	sub	.L1	l, 2, l			; o l = l - 2
 ||[!i]	add	.S2	xinptrA, n2As, xinptrA	; p if(i==0)xinptrA=xinptrA+n2A 

						; for(k=n; k > 1; k >>= 1)
   						; {

 	;------------------ outer loop - loopk -------------------------						; for (l=0; l<n/2; l++)			  
loopk:
c0:
	lddw	.D2	*+xinptrA[n2], A7:A6	; @ yr = xinptrA[2*n2]; 
						;   yi = xinptrA[2*n2 + 1]] 
||	mpy	.M2	i, 1, m			; m = i; 	

c1:
	lddw	.D2	*xinptrA++, B5:B4	; @ xr = *xinptrA++; 
						;   xi = *xinptrA++; 
||	lddw	.D1	*wptrB++, A5:A4		; c = *wptrB++; s = *wptrB++; 
||[!i]	mv	.S2	n2, i			; @ if (i==0) i = n2; 

c2:
	subsp	.L1x	xr, yr, rtemp		; rtemp = xr - yr; 
||[i]	sub	.S2	i, 1, i			; @ i = i - 1; 	

c3:
        subsp	.L1x	xi, yi, itemp		; itemp = xi - yi; 
||      addsp	.L2x	xi, yi, Xi		; Xi = xi + yi; 
||[!i]	add	.S2	xinptrA, n2As, xinptrA	; @ if (i==0) 

c4:
	lddw	.D2	*+xinptrA[n2], A7:A6	; @@ yr = xinptrA[2*n2];
						;    yi = xinptrA[2*n2 + 1]; 
||      addsp	.L2x	xr, yr, Xr		; Xr = xr + yr; 
||	mpy	.M2	i, 1, m			; m = i; 	

c5:
	lddw	.D2	*xinptrA++, B5:B4	; @@ xr = *xinptrA++; 
						;    xi = *xinptrA++; 
||[!m]	lddw	.D1	*wptrB++, A5:A4		; @ if (m==0) {c=*wptrB++;
                                                ;              s=*wptrB++;} 
||[!i]	mv	.S2	n2, i			; @@ if (i==0) i = n2; 

c6:
	subsp	.L1x	xr, yr, rtemp		; @ rtemp = xr - yr; 
||	mpysp	.M1	c, rtemp, p1r		; p1r = c*rtemp; 
||[i]	sub	.S2	i, 1, i			; @@ i = i - 1; 	

c7:
        subsp	.L1x	xi, yi, itemp		; @ itemp = xi - yi; 
||      addsp	.L2x	xi, yi, Xi		; @ Xi = xi + yi; 
||	mpysp	.M1	s, itemp, p2r		; p2r = s*itemp; 
||[!i]	add	.S2	xinptrA, n2As, xinptrA	; @@ if (i==0) 

c8:
	lddw	.D2	*+xinptrA[n2], A7:A6	; @@@ yr = xinptrA[2*n2]
						;     yi = xinptr[2*n2 + 1]; 
||      addsp	.L2x	xr, yr, Xr		; @ Xr = xr + yr; 
||	mpysp	.M1	s, rtemp, p2i		; p2i = s*rtemp; 
||	mpy	.M2	i, 1, m			; @ m = i; 	


c9:
	lddw	.D2	*xinptrA++, B5:B4	; @@@ xr = *xinptrA++; 
						;     xi = *xinptrA++; 
||	mpysp	.M1	c, itemp, p1i		; p1i = c*itemp; 
||[!m]	lddw	.D1	*wptrB++, A5:A4		; @@ if (m==0) {c=*wptrB++; 
						;		s=*wptrB++;} 
||[!i]	mv	.S2	n2, i			; @@@ if (i==0) i = n2; 

c10:
	subsp	.L1x	xr, yr, rtemp		; @@ rtemp = xr - yr; 
||	mpysp	.M1	c, rtemp, p1r		; @ p1r = c*rtemp; 
||	stw	.D1	Xi, *xoutptrB++[2]	; *xoutptrB = Xi;
						;  xoutptrB=xoutptrB+2; 
||[i]	sub	.S2	i, 1, i			; @@@ i = i - 1; 	

c11:
        subsp	.L1x	xi, yi, itemp		; @@ itemp = xi - yi; 
||      addsp	.L2x	xi, yi, Xi		; @@ Xi = xi + yi; 
||	mpysp	.M1	s, itemp, p2r		; @ p2r = s*itemp; 
||	stw	.D1	Xr, *-xoutptrB[3]	; *(xoutptrB-3) = Xr; 
||[!i]	add	.S2	xinptrA, n2As, xinptrA	; @@@ if (i==0)
						;       xinptrA = xinptrA + n2A; 
||[p]	sub	.S1	p, 1, p			; p = p - 1;  

c12:
	lddw	.D2	*+xinptrA[n2], A7:A6	; @@@@ yr = xinptrA[2*n2];
						;      xyi = xinptrA[2*n2 + 1]; 
||      addsp	.L2x	xr, yr, Xr		; @@ Xr = xr + yr; 
||	mpysp	.M1	s, rtemp, p2i		; @ p2i = s*rtemp; 
||      subsp	.L1	p1r, p2r, Yr		; Yr = p1r - p2r; 
||[!p]	add	.S1x	xoutptrB, n2As, xoutptrB; if (p==0) 
						;    xoutptrB = xoutptrB + n2; 
||	mpy	.M2	i, 1, m			; @@ m = i; 	
||[l]	sub	.D1	l, 1, l			; if (l!=0) l = l -1; 

c13:
	lddw	.D2	*xinptrA++, B5:B4	; @@@@ xr = *xinptrA++; 
						;      xi = *xinptrA++; 
||	mpysp	.M1	c, itemp, p1i		; @ p1i = c*itemp; 
||      addsp	.L1	p1i, p2i, Yi		; Yi = p1i + p2i; 
||[!m]	lddw	.D1	*wptrB++, A5:A4		; @@@ if (m==0) {c=*wptrB++; 
						;		 s=*wptrB++;} 
||[!i]	mv	.S2	n2, i			; @@@@ if (i==0), i = n2; 
||[!p]	mv	.S1x	n2, p			; if (p==0), p = n2; 	

c14:
	subsp	.L1x	xr, yr, rtemp		; @@@ rtemp = xr - yr; 
||	mpysp	.M1	c, rtemp, p1r		; @@ p1r = c*rtemp; 
||	stw	.D1	Xi, *xoutptrB++[2]	; @ *xoutptrB=Xi;
						;    xoutptrB=xoutptrB+2; 
||[i]	sub	.S2	i, 1, i			; @@@@ i = i - 1; 	
||[l]	b	.S1	loopl			; if (l!=0) branch to loopl

c15:
        subsp	.L1x	xi, yi, itemp		; @@@ itemp = xi - yi; 
||      addsp	.L2x	xi, yi, Xi		; @@@ Xi = xi + yi; 
||	mpysp	.M1	s, itemp, p2r		; @@ p2r = s*itemp; 
||	stw	.D1	Xr, *-xoutptrB[3]	; *(xoutptrB-3) = Xr; 
||[!i]	add	.S2	xinptrA, n2As, xinptrA	; @@@@ if (i==0)
						;      xinptrA = xinptrA + n2A; 
||[p]	sub	.S1	p, 1, p 		; @ p = p - 1; 

	; ----------------- end prolog for inner loop - loopl ------------------   		

 	;------------------ inner loop - loopl loop code -----------------------
loopl:	

c16:
	lddw	.D2	*+xinptrA[n2], A7:A6	; @@@@@ yr = xinptrA[2*n2]; 
						;       yi = xinptrA[2*n2 + 1]; 
||      addsp	.L2x	xr, yr, Xr		; @@@ Xr = xr + yr; 
||	mpysp	.M1	s, rtemp, p2i		; @@ p2i = s*rtemp; 
||      subsp	.L1	p1r, p2r, Yr		; @ Yr = p1r - p2r; 
||[!p]	add	.S1x	xoutptrB, n2As, xoutptrB; @ if (j==0) 
						;    xoutptrB = xoutptrB + n2; 
||[l]	sub	.D1	l, 1, l			; @ if (l!=0) l = l -1; 
||	mpy	.M2	i, 1, m			; @@@ m = i; 	
||[!j]	add	.S2	xoutptrA, n2As, xoutptrA; if (j==0) 

c17:
	lddw	.D2	*xinptrA++, B5:B4	; @@@@@ xr = *xinptrA++; 
						;       xi = *xinptrA++; 
||	mpysp	.M1	c, itemp, p1i		; @@ p1i = c*itemp; 
||      addsp	.L1	p1i, p2i, Yi		; @ Yi = p1i + p2i; 
||[!m]	lddw	.D1	*wptrB++, A5:A4		; @@@@ if (i==0) {c=*wptrB++; 
						;		  s=*wptrB++;}
||[!i]	mv	.S2	n2, i			; @@@@@ if (i==0) i = n2; 
||[!p]	mv	.S1x	n2, p			; @ if (p==0) p = n2; 	
||[j]	sub	.L2	j, 1, j			; j = j - 1;

c18:
	subsp	.L1x	xr, yr, rtemp		; @@@@ rtemp = xr - yr; 
||	mpysp	.M1	c, rtemp, p1r		; @@@ p1r = c*rtemp; 
||	stw	.D2	Yr, *xoutptrA++		; *xoutptrA++ = Yr; 
||	stw	.D1	Xi, *xoutptrB++[2]	; @ *xoutptrB=Xi; 
						;   xoutptrB=xoutptrB+2; 
||[i]	sub	.S2	i, 1, i			; @@@@@ i = i - 1; 	
||[l]	b	.S1	loopl			; @

c19:
        subsp	.L1x	xi, yi, itemp		; @@@@ itemp = xi - yi; 
||      addsp	.L2x	xi, yi, Xi		; @@@@ Xi = xi + yi; 
||	mpysp	.M1	s, itemp, p2r		; @@@ p2r = s*itemp; 
||	stw	.D2	Yi, *xoutptrA++		; *xoutptrA++ = Yi; 
||	stw	.D1	Xr, *-xoutptrB[3]	; @ *(xoutptrB-3) = Xr; 
||[!i]	add	.S2	xinptrA, n2As, xinptrA	; @@@@@ if (i==0) 
						;     xinptrA = xinptrA + n2A; 
||[p]	sub	.S1	p, 1, p			; @@ p = p - 1;  
||[!j]	mpy	.M2	n2, 1, j		; if (j==0) j = n2; 

loopl_end:

 	;------------------ end of inner loop - loopl loop code ----------------
	
	; ----------------- epilog for inner loop - loopl ----------------------   	
 	
c20:
      	subsp	.L1	p1r, p2r, Yr		; e Yr = p1r - p2r;
 ||[!j]	add	.D2	xoutptrA, n2As, xoutptrA; e if (j==0) 
						;      xoutptrA = xoutptrB + n2;
 ||	mv	.S1x	tmpk, k			; o k = tmpk
 ||	mv	.L2	x, xinptrA		; o xinptrA = x;
 ||	shl	.S2	n2, 1, n2		; o n2 = n2 << 1;

c21:
      	addsp	.L1	p1i, p2i, Yi		; e Yi = p1i + p2i;
 ||[j]	sub	.L2	j, 1, j			; e j = j - 1;
 ||	mv	.S1x	x, xoutptrB		; o xoutptrB = x;

c22:
	stw	.D2	Yr, *xoutptrA++		; e *xoutptrA++ = Yr;
 ||[k]	b	.S1	loopk			; o 

c23:
	stw	.D2	Yi, *xoutptrA++		; e *xoutptrA++ = Yi;
 ||[!j]	mpy	.M2	n2, 1, j		; e if (j==0) j = n2;
 ||	mv	.S1x	w, wptrB		; o wptrB = w;  	

c24:
   [!j]	add	.L2	xoutptrA, n2As, xoutptrA; e if (j==0) 
						;      xoutptrA = xoutptrA + n2;
 ||	shr	.S1x	n, 1, l			; o l = n/2
 ||	shl	.S2	n2, 3, n2As		; o n2As = n2<<3;	 
 ||	lddw	.D2	*+xinptrA[n2], A7:A6	; p yr = xinptrA[2*n2]; 
						;   yi = xinptrA[2*n2 + 1];
c25:
   [k]	shr	.S2	tmpk, 1, tmpk		; o tmpk = tmpk >> 1;
 ||	sub		l, 2, l			; o l = l - 2;
 ||  	mv	.L2	n2, i			; o i = n2;
 ||	lddw	.D2	*xinptrA++, B5:B4	; p xr = *xinptrA++; 
						;   xi = *xinptrA++;
c26:
 	stw	.D2	Yr, *xoutptrA++		; e *xoutptrA++ = Yr;
 ||  	mv	.L2	n2, j			; o j = n2;
 ||	mv		n2, p			; o p = n2
 ||[i]	sub	.S2	i, 1, i			; p i = i - 1;

c27: 
	stw	.D2	Yi, *xoutptrA		; e *xoutptrA++ = Yi;
 ||  	add	.S2X	xoutptrB, n2As, xoutptrA; o xoutptrA = xoutptrB + n2;
 ||	add		xoutptrB, 4, xoutptrB	; o xoutptrB = xoutptrB + 4;	 
 ||[!i]	add	.L2	xinptrA, n2As, xinptrA	; p if (i==0) 
						;      xinptrA = xinptrA + n2A; 
loopk_end:

	; ----------------- end of epilog for inner loop - loopl ---------------   	

	; ----------------- end of outer loop - loopk --------------------------

	; ---------------------------- function epilog  ------------------------

	mvc	.S2	CSR, B13

	; restore preserved by call registers

	sub		B15, 4, A0

	ldw	.D1 	*++A0[2], B3		; f
 ||	ldw	.D2 	*++B15[2], A15		; f
 ||	mvc	.S2	CSR, B13		; f

	ldw	.D1 	*++A0[2], B14		; f
 ||	ldw	.D2 	*++B15[2], A14		; f
 ||	or	.L2	B13, 1, B13		; f

	ldw	.D1 	*++A0[2], B13		; f
 ||	ldw	.D2 	*++B15[2], A13		; f
 ||	mvc     .S2     B13,CSR			; f enable global interrupts

	ldw	.D1 	*++A0[2], B12		; f
 ||	ldw	.D2 	*++B15[2], A12		; f

	ldw	.D1 	*++A0[2], B11		; f
 ||	ldw	.D2 	*++B15[2], A11		; f

	ldw	.D2 	*++B15[2], A10		; f
 ||	ldw	.D1 	*++A0[2], B10		; f
 ||	b	.S2	B3			; f return();
	nop		5			; f
