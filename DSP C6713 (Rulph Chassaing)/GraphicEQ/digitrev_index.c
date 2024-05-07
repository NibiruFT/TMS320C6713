//******************************************************************************
//	TEXAS INSTRUMENTS, INC.
//
//	Copyright © Texas Instruments Incorporated 1998
//
//	TI retains all right, title and interest in this code and authorizes its
//	use solely and exclusively with digital signal processing devices
//	manufactured by or for TI.  This code is intended to provide an
//	understanding of the benefits of using TI digital signal processing devices.
//	It is provided "AS IS".  TI disclaims all warranties and representations,
//	including but not limited to, any warranty of merchantability or fitness
//	for a particular purpose.  This code may contain irregularities not found
//	in commercial software and is not intended to be used in production
//	applications.  You agree that prior to using or incorporating this code
//	into any commercial product you will thoroughly test that product and the
//	functionality of the code in that product and will be solely responsible
//	for any problems or failures.  
//
//	TI retains all rights not granted herein.
//******************************************************************************


// From TI's docs on bitrev_cplx (both docs and code):
// This routine calculates the index for digitrev of length n (length of index is
// 2^(radix*ceil(k/radix)) where n = 2^k.
// in other words
//   Either:sqrt(n) when n=2^even# Or: sqrt(2)*sqrt(n) when n=2^odd# [radix 2]
//          sqrt(n) when n=4^even# Or: sqrt(4)*sqrt(n) when n=4^odd# [radix 4]
// Note: the variable "radix" is 2 for radix-2 and 4 for radix-4
// ******************************************************************************
void digitrev_index(short *index, int n, int radix){

	int		i,j,k;
	short	nbits, nbot, ntop, ndiff, n2, raddiv2; 

	nbits = 0;
	i = n;	
	while (i > 1){
		i = i >> 1;
		nbits++;
	}
	
	raddiv2	= radix >> 1;
	nbot	= nbits >> raddiv2;
	nbot	= nbot << raddiv2 - 1;
	ndiff	= nbits & raddiv2;
	ntop	= nbot + ndiff;
	n2	= 1 << ntop;
	index[0] = 0;
	for ( i = 1, j = n2/radix + 1; i < n2 - 1; i++){
		index[i] = j - 1;
		for (k = n2/radix; k*(radix-1) < j; k /= radix)
				j -= k*(radix-1);
		j += k;
	}
	index[n2 - 1] = n2 - 1;
}
