//Envelope detection routine
/* Approximate the envelope of the signal by rectifying and filtering
 *  sample = input sample (16-bit)
 *  returns signal envelope (16-bit)
 */
#define env_coeff 4000 						/* / 32768  envelope filter parameter */
int envelope = 0;      				/*current sample of signal envelope (32-bit) */
short detect_envelope(short sample)
{ 
  short word1,word2;

  if (sample < 0)
  	sample = -sample; /* rectify the signal */

  word1 = envelope >> 15;         				/* high-order word */
  word2 = envelope & 0x00007fff;  				/* low-order word */
  envelope = (word1 * (32768 - env_coeff)) +
  	   	     ((word2 * (32768 - env_coeff)) >> 15) + 
  	   	     sample * env_coeff; 				/* envelope =
                                      		envelope*(1-coeff) + sample*coeff */
  return envelope >> 15;
}
