/*Mulaw.c  Implementing Mu law for speech companding 
*************************************************************************************************
About Mu Law --
Mu-law  encoding is a non uniform quantizing logarithmic compression for audio signals including
speech used in the United States.It is widely used in the telecommunications field because it improves the signal-to-noise ratio
without increasing the amount of data. The dynamic range increases while the no of bits for 
quantization remains constant. Typically, mu-law compressed speech is carried in 8-bit samples. 
It is a companding technqiue.That means that carries more information about the smaller signals than
about larger signals. Implement here Mu law compression and Expansion with table lookup.
It's based on the observation that many signals are statistically more likely to be near a
low signal level than a high signal level.  Therefore, it makes more sense to have more quantization
points near a low level than a high level.  
**************************************************************************************************
Mu Law Encoding or compression
unsigned char linear2ulaw(sample)

For the table of 256 values. We have the quantisation levels from 0 to 7
As we go more towards higher level of signal. More of the higher level signals are
represented by 7 as seen from the table lookup. 
Thus 3 exponent bits are used to represent the levels from 0 to 7
and 4 mantissa bits are used to represent the next 4 significant bits. 
One bit is used to represent the sign bit
***************************************************************************************************
Mu Law decoding or decompression
int ulaw2linear(ulawbyte)

From the char obtained by encoding the highest bit is the sign bit
The next 3 bits are the exponent bits and the next 4 bits are the mantissa bits.
The 3 expoment bits are used to select the quantisation level. The 4 mantissa bits
are used to add the less significant parts and one bit for sign.
***************************************************************************************************
*/
#define BIAS 0x84   /* define the add-in bias for 16 bit samples */
#define CLIP 32635
short sample_data = 0;
unsigned char lmu;
int sign, exponent, mantissa;
int mulin;
unsigned char linear2ulaw(short sample)
 {
 static int exp_lut[256] = {0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
                           4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
                           5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
                           5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
                           6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                           6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                           6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                           6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
                           7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                           7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                           7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                           7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                           7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                           7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                           7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
                           7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};

  unsigned char ulawbyte;
  /* Get the sample into sign-magnitude. */
  sign = (sample >> 8) & 0x80;		/* set aside the sign */
  if (sign != 0) sample = -sample;		/* get magnitude */
  if (sample > CLIP) sample = CLIP;		/* clip the magnitude */
  /* Convert from 16 bit linear to ulaw. */
  sample = sample + BIAS;
  exponent = exp_lut[(sample >> 7) & 0xFF];
  mantissa = (sample >> (exponent + 3)) & 0x0F;
  ulawbyte = ~(sign | (exponent << 4) | mantissa);
  return(ulawbyte);
}

main()
{
   comm_intr();					  		//init DSK, codec, McBSP
   while(1);              				//infinite loop; waiting for interrupts
}

interrupt void c_int11()				//ISR to write
{
sample_data = input_sample(); 	    	//newest input sample data
 lmu= linear2ulaw(sample_data);           //Conversion from linear to Mu law
 mulin=ulaw2linear(lmu);
  mulin=toascii(lmu);
  output_sample(mulin);  	            //output sample converted from mu law sample back to linear o/p
}

/*
** This routine converts from ulaw to 16 bit linear.
** Input: 8 bit ulaw sample
** Output: signed 16 bit linear sample
*/

int ulaw2linear(ulawbyte)
unsigned char ulawbyte;
{
  static int exp_lut[8] = {0,132,396,924,1980,4092,8316,16764};
  int sign, exponent, mantissa, sample;
  ulawbyte = ~ulawbyte;
  sign = (ulawbyte & 0x80);
  exponent = (ulawbyte >> 4) & 0x07;
  mantissa = ulawbyte & 0x0F;
  sample = exp_lut[exponent] + (mantissa << (exponent + 3));
  if (sign != 0) sample = -sample;
  return(sample);
}

