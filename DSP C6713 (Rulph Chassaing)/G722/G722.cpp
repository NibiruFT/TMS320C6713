/**********************************************************************
codec_ftns.c - Encoder and Decoder functions. 
***********************************************************************/
#include <math.h>
/********  Function Declarations  ********/
int encode(int,int);
void decode(int);
int filtez(int *bpl,int *dlt);
void upzero(int dlt,int *dlti,int *bli);
int filtep(int rlt1,int al1,int rlt2,int al2);
int quantl(int el,int detl);
int logscl(int il,int nbl);
int scalel(int nbl,int shift_constant);
int uppol2(int al1,int al2,int plt,int plt1,int plt2);
int uppol1(int al1,int apl2,int plt,int plt1);
int logsch(int ih,int nbh);
void reset();

/********  Transmit QMF Variable Declarations  ********/
int tqmf[24];			/* TQMF delay samples */
int h[24] = {           /* TQMF filter coeficients */
    12,   -44,   -44,   212,    48,  -624,   128,  1448,
  -840, -3220,  3804, 15504, 15504,  3804, -3220,  -840,
  1448,   128,  -624,    48,   212,   -44,   -44,    12
};

/********  Receive QMF Variable Declarations  ********/
int accumc[11],accumd[11];  /* Receive QMF Accumulators */
int xout1,xout2;			/* outputs of decode */
int slout, xlout;

/********  Lo and Hi Band Encoder Variable Declarations  ********/
/* Quantizer Lookup Tables */
int decis_levl[30] = {	/* Decision Levels: Pre-multiplied by 8 */
   280,   576,   880,  1200,  1520,  1864,  2208,  2584,
  2960,  3376,  3784,  4240,  4696,  5200,  5712,  6288,
  6864,  7520,  8184,  8968,  9752, 10712, 11664, 12896,
 14120, 15840, 17560, 20456, 23352, 32767
};
int quant26bt_pos[31] = {  /* 6-bit positive quantization values */
    61,    60,    59,    58,    57,    56,    55,    54,
    53,    52,    51,    50,    49,    48,    47,    46,
    45,    44,    43,    42,    41,    40,    39,    38,
    37,    36,    35,    34,    33,    32,    32
};
int quant26bt_neg[31] = {  /* 6-bit negative quantization values */
    63,    62,    31,    30,    29,    28,    27,    26,
    25,    24,    23,    22,    21,    20,    19,    18,
    17,    16,    15,    14,    13,    12,    11,    10,
     9,     8,     7,     6,     5,     4,     4
};

/* Inverse Quantizer Lookup Tables */
int qq4_code4_table[16] = {
     0,  -20456,  -12896,   -8968,   -6288,   -4240,   -2584,   -1200,
 20456,   12896,    8968,    6288,    4240,    2584,    1200,       0
};
int qq5_code5_table[32] = {
  -280,    -280,  -23352,  -17560,  -14120,  -11664,   -9752,   -8184,
 -6864,   -5712,   -4696,   -3784,   -2960,   -2208,   -1520,    -880,
 23352,   17560,   14120,   11664,    9752,    8184,    6864,    5712,
  4696,    3784,    2960,    2208,    1520,     880,     280,    -280
};
int qq6_code6_table[64] = {
  -136,    -136,    -136,    -136,  -24808,  -21904,  -19008,  -16704,
-14984,  -13512,  -12280,  -11192,  -10232,   -9360,   -8576,   -7856,
 -7192,   -6576,   -6000,   -5456,   -4944,   -4464,   -4008,   -3576,
 -3168,   -2776,   -2400,   -2032,   -1688,   -1360,   -1040,    -728,
 24808,   21904,   19008,   16704,   14984,   13512,   12280,   11192,
 10232,    9360,    8576,    7856,    7192,    6576,    6000,    5456,
  4944,    4464,    4008,    3576,    3168,    2776,    2400,    2032,
  1688,    1360,    1040,     728,     432,     136,    -432,    -136
};
int qq2_code2_table[4] = {
  -7408,   -1616,   7408,  1616
};

/* Quantizer Adaptation Values */
int wl_code_table[16] = {
   -60,  3042,  1198,   538,   334,   172,    58,   -30,
  3042,  1198,   538,   334,   172,    58,   -30,   -60
};
int wh_code_table[4] = {
   798,   -214,    798,   -214
};
int wl_table[8] = {
   -60,   -30,    58,   172,   334,   538,  1198,  3042
};
int ilb_table[32] = {
  2048,  2093,  2139,  2186,  2233,  2282,  2332,  2383,
  2435,  2489,  2543,  2599,  2656,  2714,  2774,  2834,
  2896,  2960,  3025,  3091,  3158,  3228,  3298,  3371,
  3444,  3520,  3597,  3676,  3756,  3838,  3922,  4008
};

int 	delay_bpl[6];
int		delay_dltx[6];
int     nbl;
int     al1,al2;
int     plt,plt1,plt2;
int     rs;
int     dlt;
int     rlt,rlt1,rlt2;
int     detl;

int     delay_dhx[6];
int     delay_bph[6];
int     nbh;
int     ah1,ah2;
int     ph1,ph2;
int     rh1,rh2;
int     deth;

/********  Lo and Hi Band Decoder Variable Declarations  ********/
int     dec_deth,dec_detl,dec_dlt;
int     dec_del_bpl[6];
int     dec_del_dltx[6];
int		dec_plt,dec_plt1,dec_plt2;
int		dec_szl,dec_spl,dec_sl;
int		dec_rlt1,dec_rlt2,dec_rlt;
int		dec_al1,dec_al2;
int		dec_nbl,dec_yh,dec_dh,dec_nbh;

/********  filtez Variable Declarations  ********/

int     dec_del_bph[6];
int     dec_del_dhx[6];
int     dec_szh;

/********  filtep Variable Declarations  ********/

int     dec_rh1,dec_rh2;
int     dec_ah1,dec_ah2;
int     dec_ph,dec_sph;
int		dec_sh,dec_rh;
int		dec_ph1,dec_ph2;

/***************************************************************
encode: input samples in xin1 = first value, xin2 = second value
performs the encoder processing.
returns il and ih stored together
***************************************************************/
int encode(int xin1,int xin2)
{
	int i;						/* tqmf index */
    int *h_ptr;					/* pointer to tqmf coeficients */
    int *tqmf_ptr,*tqmf_ptr1;	/* pointers to tqmf delay values */
    long int xa,xb;				/* odd and even values */
    int xl,xh;					/* low and high bands */
    int decis;
    int sh;         			/* comes from adaptive predictor */
    int eh;						/* hi band error */
    int dh;
    int il,ih;					/* encoder outputs */
    int szh,sph,ph,yh;			/* high band predictor outputs */
    int szl,spl,sl,el;			/* low band predictor outputs */

/*BEGIN: Transmit Quadrature Mirror Filter*/
    h_ptr = h;
    tqmf_ptr = tqmf;
    /* first multiply and accumulate */
    xa = (long)(*tqmf_ptr++) * (*h_ptr++);
    xb = (long)(*tqmf_ptr++) * (*h_ptr++);
	/* multiply and accumulate loop */
    for(i=0 ; i<10 ; i++) {
        xa += (long)(*tqmf_ptr++) * (*h_ptr++);
        xb += (long)(*tqmf_ptr++) * (*h_ptr++);
    }
	/* final multiply and accumulate */
    xa += (long)(*tqmf_ptr++) * (*h_ptr++);
    xb += (long)(*tqmf_ptr) * (*h_ptr++);
	/* update delay line tqmf with new samples */
    tqmf_ptr1 = tqmf_ptr - 2;
    for(i = 0 ; i < 22 ; i++) *tqmf_ptr-- = *tqmf_ptr1--;
    *tqmf_ptr-- = xin1;
    *tqmf_ptr = xin2;
    /* combine filter outputs to produce hi/lo sub-bands */
    xl = (xa + xb) >> 15;
    xh = (xa - xb) >> 15;
    xlout = xl;
/*END: Transmit Quadrature Mirror Filter*/

/*BEGIN: Lo Sub-Band Encoder*/
	/* compute the predictor output zero section */
	szl = filtez(delay_bpl, delay_dltx);
	/* compute the predictor output pole section */
    spl = filtep(rlt1,al1,rlt2,al2);
	/* compute the predictor output value */
	sl = szl + spl;
	slout = sl;
	/* compute the error signal el(n) */
	el = xl - sl;
	/* adaptive quantizer qmplementation */
	il = quantl(el,detl);
	/* inverse adaptive quantizer implementation */
	dlt = ((long)detl*qq4_code4_table[il >> 2]) >> 15;
	/* quantizer adaptation implementation */
	/* first compute the log scaling factor nbl */
	nbl = logscl(il, nbl);
	/* then compute the linear scaling factor detl */
	/* 8 is a scale factor so that scalel can be used */
	/* for the hi-band encoder */
	detl = scalel(nbl,8);
	/* adaptive prediction implementation */
	/* compute the partial reconstructed signal */
    plt = dlt + szl;
	/* update the zero section predictor coefficients */
    upzero(dlt,delay_dltx,delay_bpl);
	/* update second pole section predictor coefficient */
    al2 = uppol2(al1,al2,plt,plt1,plt2);
	/* update first pole section predictor coefficient */    
    al1 = uppol1(al1,al2,plt,plt1);
	/* compute the quantized recontructed signal for adaptive predic */
    rlt = sl + dlt;
	/* implement delays for next time */
    rlt2 = rlt1;
    rlt1 = rlt;
    plt2 = plt1;
    plt1 = plt;
/*END: Lo Sub-Band Encoder*/
           
/*BEGIN: Hi Sub-Band Encoder*/ 
	/* compute the predictor output zero section */
    szh = filtez(delay_bph,delay_dhx);
    /* compute the predictor output pole section */
    sph = filtep(rh1,ah1,rh2,ah2);
    /* compute the predictor output value */
    sh = sph + szh;
	/* compute the error signal eh(n) */
    eh = xh - sh;
	/* adaptive quantizer qmplementation */
    if(eh >= 0) {
        ih = 3;     /* 2,3 are pos codes */
    }
    else {
        ih = 1;     /* 0,1 are neg codes */
    }
    decis = (564L*(long)deth) >> 12L;
    if(abs(eh) > decis) ih--;     /* mih = 2 case */
	/* inverse adaptive quantizer implementation */
	dh = ((long)deth*qq2_code2_table[ih]) >> 15L ;
    /* quantizer adaptation implementation */
	/* first compute the log scaling factor nbh */
    nbh = logsch(ih,nbh);
    /* then compute the linear scaling factor deth */ 
    deth = scalel(nbh,10);
	/* adaptive prediction implementation */
	/* compute the partial reconstructed signal */
    ph = dh + szh;
     /* update the zero section predictor coefficients */
    upzero(dh,delay_dhx,delay_bph);
     /* update second pole section predictor coefficient */
    ah2 = uppol2(ah1,ah2,ph,ph1,ph2);
	/* update first pole section predictor coefficient */
    ah1 = uppol1(ah1,ah2,ph,ph1);
    /* compute the quantized recontructed signal for adaptive predic */
    yh = sh + dh;
	/* implement delays for next time */
    rh2 = rh1;
    rh1 = yh;
    ph2 = ph1;
    ph1 = ph;
/*END: Hi Sub-Band Encoder*/

/*Multiplex ih and il to get 8-bit signal*/
    return(il | (ih << 6));
}

/***************************************************************
decode: Input is the output of the encoder
performs the decoder processing.
result in xout1 and xout2
***************************************************************/
void decode(int input)
{
    int i;						/* rqmf filter index */
    int xa1,xa2;    			/* qmf accumulators */
    int *h_ptr;					/* qmf coeficient pointer */
    int *ac_ptr,*ac_ptr1;		/* delay buffer pointers */
    int *ad_ptr,*ad_ptr1;
    int ilr,ih;					/* encoded error */
    int xs,xd;
    int rl,rh;
    int dl;

/*Split Received Word into Lo-Band and Hi-Band */
    ilr = input & 0x3f;
    ih = input >> 6;

/*BEGIN: Lo Sub-Band Decoder*/
	/* compute predictor output for zero section */
    dec_szl = filtez(dec_del_bpl,dec_del_dltx);
	/* compute predictor output signal for pole section */
    dec_spl = filtep(dec_rlt1,dec_al1,dec_rlt2,dec_al2);
    /* compute the predictor output value */
    dec_sl = dec_spl + dec_szl;
	/* compute quantized difference signal for adaptive predic */
    dec_dlt = ((long)dec_detl*qq4_code4_table[ilr >> 2]) >> 15;
	/* compute quantized difference signal for decoder output */
    dl = ((long)dec_detl*qq6_code6_table[ilr]) >> 15;
    /* compute the quantized recontructed signal for adaptive predic */
    rl = dl + dec_sl;
	/* quantizer adaptation implementation */
	/* first compute the log scaling factor dec_nbl */
    dec_nbl = logscl(ilr,dec_nbl);
	/* then compute the linear scaling factor dec_detl */
    dec_detl = scalel(dec_nbl,8);
	/* adaptive prediction implementation */
	/* compute the partial reconstructed signal */
    dec_plt = dec_dlt + dec_szl;
    /* update the zero section predictor coefficients */
    upzero(dec_dlt,dec_del_dltx,dec_del_bpl);
    /* update second pole section predictor coefficient */
    dec_al2 = uppol2(dec_al1,dec_al2,dec_plt,dec_plt1,dec_plt2);
	/* update first pole section predictor coefficient */
    dec_al1 = uppol1(dec_al1,dec_al2,dec_plt,dec_plt1);
    /* compute the quantized recontructed signal for adaptive predic */
    dec_rlt = dec_sl + dec_dlt;
	/* implement delays for next time */
    dec_rlt2 = dec_rlt1;
    dec_rlt1 = dec_rlt;
    dec_plt2 = dec_plt1;
    dec_plt1 = dec_plt;
/*END: Lo Sub-Band Decoder*/

/*BEGIN: Hi Sub-Band Decoder*/
	/* compute predictor output for zero section */
    dec_szh = filtez(dec_del_bph,dec_del_dhx);
	/* compute predictor output signal for pole section */
    dec_sph = filtep(dec_rh1,dec_ah1,dec_rh2,dec_ah2);
    /* compute the predictor output value */
    dec_sh = dec_sph + dec_szh;
	/* compute quantized difference signal for adaptive predic */
    dec_dh = ((long)dec_deth*qq2_code2_table[ih]) >> 15L ;
	/* quantizer adaptation implementation */
	/* first compute the log scaling factor dec_nbh */
    dec_nbh = logsch(ih,dec_nbh);
	/* then compute the linear scaling factor dec_deth */
    dec_deth = scalel(dec_nbh,10);
    /* adaptive prediction implementation */
	/* compute the partial reconstructed signal */
    dec_ph = dec_dh + dec_szh;
    /* update the zero section predictor coefficients */
    upzero(dec_dh,dec_del_dhx,dec_del_bph);
    /* update second pole section predictor coefficient */
    dec_ah2 = uppol2(dec_ah1,dec_ah2,dec_ph,dec_ph1,dec_ph2);
 	/* update first pole section predictor coefficient */
    dec_ah1 = uppol1(dec_ah1,dec_ah2,dec_ph,dec_ph1);
    /* compute the quantized recontructed signal for adaptive predic */
    rh = dec_sh + dec_dh;
 	/* implement delays for next time */
    dec_rh2 = dec_rh1;
    dec_rh1 = rh;
    dec_ph2 = dec_ph1;
    dec_ph1 = dec_ph;
/*END: Hi Sub-Band Decoder*/

/*BEGIN: Receive Quadrature Mirror Filter*/
    xd = rl - rh;
    xs = rl + rh;
    h_ptr = h;
    ac_ptr = accumc;
    ad_ptr = accumd;
    /* first multiply and accumulate */
    xa1 = (long)xd * (*h_ptr++);
    xa2 = (long)xs * (*h_ptr++);
	/* main multiply accumulate */
    for(i = 0 ; i < 10 ; i++) {
        xa1 += (long)(*ac_ptr++) * (*h_ptr++);
        xa2 += (long)(*ad_ptr++) * (*h_ptr++);
    }
	/* final multiply and accumulate */
    xa1 += (long)(*ac_ptr) * (*h_ptr++);
    xa2 += (long)(*ad_ptr) * (*h_ptr++);
	/* scale by 2^14 */
    xout1 = xa1 >> 14;
    xout2 = xa2 >> 14;
	/* update delay lines */
    ac_ptr1 = ac_ptr - 1;
    ad_ptr1 = ad_ptr - 1;
    for(i = 0 ; i < 10 ; i++) {
        *ac_ptr-- = *ac_ptr1--;
        *ad_ptr-- = *ad_ptr1--;
    }
    *ac_ptr = xd;
    *ad_ptr = xs;
/*END: Receive Quadrature Mirror Filter*/
}

/***************************************************************
reset: clears all storage locations
***************************************************************/
void reset()
{
    int i;

    detl = dec_detl = 32;   /* reset to min scale factor */
    deth = dec_deth = 8;
    nbl = al1 = al2 = plt1 = plt2 = rlt1 = rlt2 = 0;
    nbh = ah1 = ah2 = ph1 = ph2 = rh1 = rh2 = 0;
    dec_nbl = dec_al1 = dec_al2 = dec_plt1 = dec_plt2 = dec_rlt1 = dec_rlt2 = 0;
    dec_nbh = dec_ah1 = dec_ah2 = dec_ph1 = dec_ph2 = dec_rh1 = dec_rh2 = 0;
    for(i = 0 ; i < 6 ; i++) {
        delay_dltx[i] = 0;
        delay_dhx[i] = 0;
        dec_del_dltx[i] = 0;
        dec_del_dhx[i] = 0;
    }
    for(i = 0 ; i < 6 ; i++) {
        delay_bpl[i] = 0;
        delay_bph[i] = 0;
        dec_del_bpl[i] = 0;
        dec_del_bph[i] = 0;
    }
    for(i = 0 ; i < 24 ; i++) tqmf[i] = 0;
    for(i = 0 ; i < 11 ; i++) {
        accumc[i] = 0;
        accumd[i] = 0;
    }
}

/***************************************************************
filtez: compute predictor output signal (zero section)
input: bpl1-6 and dlt1-6, output: szl
***************************************************************/
int filtez(int *bpl,int *dlt)
{
    int i;
    long int zl;
    zl = (long)(*bpl++) * (*dlt++);
    for(i = 1 ; i < 6 ; i++)
		zl += (long)(*bpl++) * (*dlt++);

    return((int)(zl >> 14));
}

/***************************************************************
filtep: compute predictor output signal (pole section)
input rlt1-2 and al1-2, output spl
***************************************************************/
int filtep(int rlt1,int al1,int rlt2,int al2)
{
    long int pl;

    pl = (long)al1*rlt1;
    pl += (long)al2*rlt2;

    return((int)(pl >> 14));   /* x2 here */
}

/***************************************************************
quantl: quantize the difference signal in the lower sub-band
***************************************************************/
int quantl(int el,int detl)
{
    int ril,mil;
    long int wd,decis;

	/* abs of difference signal */
    wd = abs(el);
	/* determine mil based on decision levels and detl gain */
    for(mil = 0 ; mil < 30 ; mil++) {
        decis = (decis_levl[mil]*(long)detl) >> 15L;
        if(wd < decis) break;
    }
	/* if mil=30 then wd is less than all decision levels */
    if(el >= 0) ril = quant26bt_pos[mil];
    else ril = quant26bt_neg[mil];
    return(ril);
}

/***************************************************************
logscl: update the logarithmic quantizer scale factor in lower
sub-band note that nbl is passed and returned
***************************************************************/
int logscl(int il,int nbl)
{
    long int wd;

    wd = ((long)nbl * 127L) >> 7L;   /* leak factor 127/128 */
    nbl = (int)wd + wl_code_table[il >> 2];
    if(nbl < 0) nbl = 0;
    if(nbl > 18432) nbl = 18432;

    return(nbl);
}

/***************************************************************
scalel: compute the quantizer scale factor in the lower or upper
sub-band
***************************************************************/
int scalel(int nbl,int shift_constant)
{
    int wd1,wd2,wd3;

    wd1 = (nbl >> 6) & 31;
    wd2 = nbl >> 11;
    wd3 = ilb_table[wd1] >> (shift_constant + 1 - wd2);

    return(wd3 << 3);
}

/***************************************************************
upzero: inputs: dlt, dlti[0-5], bli[0-5], outputs: updated bli[0-5]
also implements delay of bli and update of dlti from dlt
***************************************************************/
void upzero(int dlt,int *dlti,int *bli)
{
    int i,wd2,wd3;
	/*if dlt is zero, then no sum into bli */
    if(dlt == 0) {
        for(i = 0 ; i < 6 ; i++) {
            bli[i] = (int)((255L*bli[i]) >> 8L); /* leak factor of 255/256 */
        }
    }
    else {
        for(i = 0 ; i < 6 ; i++) {
            if((long)dlt*dlti[i] >= 0) wd2 = 128; else wd2 = -128;
            wd3 = (int)((255L*bli[i]) >> 8L);    /* leak factor of 255/256 */
            bli[i] = wd2 + wd3;
        }
    }
	/* implement delay line for dlt */
    dlti[5] = dlti[4];
    dlti[4] = dlti[3];
    dlti[3] = dlti[2];
    dlti[2] = dlti[1];
    dlti[1] = dlti[0];
    dlti[0] = dlt;
}

/***************************************************************
uppol2: update second predictor coefficient (pole section)
inputs: al1, al2, plt, plt1, plt2. outputs: apl2
***************************************************************/
int uppol2(int al1,int al2,int plt,int plt1,int plt2)
{
    long int wd2;
    int apl2;

    wd2 = 4L*(long)al1;
    if((long)plt*plt1 >= 0L) wd2 = -wd2;    /* check same sign */
    wd2 = wd2 >> 7;                  /* gain of 1/128 */
    if((long)plt*plt2 >= 0L) {
        wd2 = wd2 + 128;             /* same sign case */
    }
    else {
        wd2 = wd2 - 128;
    }
    apl2 = wd2 + (127L*(long)al2 >> 7L);  /* leak factor of 127/128 */
	/* apl2 is limited to +-.75 */
    if(apl2 > 12288) apl2 = 12288;
    if(apl2 < -12288) apl2 = -12288;

    return(apl2);
}

/***************************************************************
uppol1: update first predictor coefficient (pole section)
inputs: al1, apl2, plt, plt1. outputs: apl1
***************************************************************/
int uppol1(int al1,int apl2,int plt,int plt1)
{
    long int wd2;
    int wd3,apl1;

    wd2 = ((long)al1*255L) >> 8L;   /* leak factor of 255/256 */
    if((long)plt*plt1 >= 0L) {
        apl1 = (int)wd2 + 192;      /* same sign case */
    }
    else {
        apl1 = (int)wd2 - 192;
    }
	/* note: wd3= .9375-.75 is always positive */
    wd3 = 15360 - apl2;             /* limit value */
    if(apl1 > wd3) apl1 = wd3;
    if(apl1 < -wd3) apl1 = -wd3;

    return(apl1);
}

/***************************************************************
logsch: update the logarithmic quantizer scale factor in higher
sub-band note that nbh is passed and returned
***************************************************************/
int logsch(int ih,int nbh)
{
    int wd;

    wd = ((long)nbh * 127L) >> 7L;       /* leak factor 127/128 */
    nbh = wd + wh_code_table[ih];
    if(nbh < 0) nbh = 0;
    if(nbh > 22528) nbh = 22528;

    return(nbh);
}
