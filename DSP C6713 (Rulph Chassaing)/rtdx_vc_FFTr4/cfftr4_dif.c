/*===============================================================================
*
* From: https://www-a.ti.com/apps/c6000/xt_download.asp?sku=C67x_cfftr4
*
*	TEXAS INSTRUMENTS, INC.		
*
*	RADIX 4 FFT (DIF) with digit-reversed output
*
*	Revision Date:	04/09/98
*	
*	USAGE	This routine is C Callable and can be called as:
*		
*		void cfftr4_dif(float* x, float* w, short n)
*
*		x is pointer to an array holding the input and output floating
*		   point array which contains 'n' complex points
*		w is pointer to an array holding the coefficient floating point
*		   array which contains 3*n/4 complex numbers
*		n is the number of complex points in x
*
*		If the routine is not to be used as a C callable function,
*		then you need to initialize values for all of the parameters
*		passed to the function since these are assumed to be in
*		registers as defined by the calling convention of the
*		compiler, (refer to the TMS320C6x Optimizing C Compiler
*		User's Guide).
*
*	C CODE
*		This is the C equivalent for the assembly code.  Note that
*		the assembly code is hand optimized and restrictions may
*		apply.
*/

void cfftr4_dif(float* x, float* w, short n)
{
	short n1, n2, ie, ia1, ia2, ia3, i0, i1, i2, i3, j, k;
	float r1, r2, r3, r4, s1, s2, s3, s4, co1, co2, co3, si1, si2, si3;
	n2 = n;
	ie = 1;
	for(k=n; k>1; k>>=2)
	{
		n1 = n2;
		n2 >>= 2;
		ia1 = 0;
		for(j=0; j<n2; j++)
		{
			ia2 = ia1 + ia1;
			ia3 = ia1 + ia2;
			co1 = w[ia1*2];
			si1 = w[ia1*2+1];
			co2 = w[ia2*2];
			si2 = w[ia2*2+1];
			co3 = w[ia3*2];
			si3 = w[ia3*2+1];
			ia1 += ie;
			for(i0=j; i0<n; i0+=n1)
			{
				 i1 = i0 + n2;
				 i2 = i1 + n2;
				 i3 = i2 + n2;
				 r1 = x[i0*2]   + x[i2*2];
				 r3 = x[i0*2]   - x[i2*2];
				 s1 = x[i0*2+1] + x[i2*2+1];
				 s3 = x[i0*2+1] - x[i2*2+1];
				 r2 = x[i1*2]   + x[i3*2];
				 r4 = x[i1*2]   - x[i3*2];
				 s2 = x[i1*2+1] + x[i3*2+1];
				 s4 = x[i1*2+1] - x[i3*2+1];
				 x[i0*2]   = r1 + r2;
				 r2        = r1 - r2;
				 r1        = r3 - s4;
				 r3        = r3 + s4;
				 x[i0*2+1] = s1 + s2;
				 s2        = s1 - s2;
				 s1        = s3 + r4;
				 s3        = s3 - r4;
				 x[i1*2]   = co1*r3 + si1*s3;
				 x[i1*2+1] = co1*s3 - si1*r3;
				 x[i2*2]   = co2*r2 + si2*s2;
				 x[i2*2+1] = co2*s2 - si2*r2;
				 x[i3*2]   = co3*r1 + si3*s1;
				 x[i3*2+1] = co3*s1 - si3*r1;
			}
		}
		ie <<= 2;
	}
}

