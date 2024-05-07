//Speech Synthesis Using Linear Predictive Coding 

#include "dsk6713_aic23.h"	
Uint32 fs=DSK6713_AIC23_FREQ_8KHZ;
#include <stdio.h>
#include <math.h>
#include "input.h"   /* Holds the input speech samples */
#include "hamming.h"  /* Holds the scaled Hamming window coefficients */
#define Def_overlap 40     /* Default overlap length */
#define Def_framelength 80 /* Default frame length	*/ 
#define Def_total 7440     /* Total length of the speech samples */
#pragma DATA_SECTION (G_final_output,".output");  /* Store the output in External Memory */
#pragma DATA_SECTION (input,".input");           /* Store the input in External Memory */  
int G_index=Def_total+3400;				/* Index to the output array  */
int G_index2=0;
int *G_final_output=NULL;       /* To store the synthesised speech */
int G_out=0;

interrupt void c_int11()
{
 short dummy;
 
 G_out=G_final_output[G_index];
  if (G_index2 % 2 == 0)
 {
 G_out=0;
 G_index++;
 } 
  dummy=input_sample();
  output_sample(G_out);
 
 G_index2++;
 if (G_index==Def_total*2 )
 {
   G_index=Def_total+3400;
   G_index2=0;
 }
}
/**** The display routines can be used to view the contents of various arrays ****/
/**** input: arr:the array to be viewed size: Size of the array                                      ****/
/**** for displaying integer arrays                                           ****/ 
void display(int* arr,int size)
{
 int i;
 
 for( i=0;i<size;i++)
 printf("%d\n",*(arr+i));
}
// for displaying float arrays
void dispflt(float* arr,int size)
{
 int i;

 for( i=0;i<size;i++)
 printf("%f\n",*(arr+i));
}

/**** SEGMENTATION FUNCTION   ***/
/**** input : start:Index of input array defining the start of the segment *****/
/**** return: segment:pointer to the first element of the segment          *****/
/**** This function takes in the starting index and returns a              *****/ 
/**** segment whose length is Def_framelength                              *****/
int* segmentation(int start)
{
 int loop;
 int seg_loop=0;
 int* segment;

 segment=(int *)malloc(sizeof(int)*Def_framelength);
 for(loop=start;loop<start+Def_framelength;loop++ )
  {
   segment[seg_loop]=input[loop];
   seg_loop++;
   }
  return segment;
 }
/**** WINDOWING FUNCTION  **/
/**** input : sp_input:The array containing the segmented input    ****/
/**** return: out:the Hamming windowed segment                     ****/
/**** The Function takes in the segmented speech samples.The       ****/   
/**** hamming window coefficients are multipled with the segmented ****/ 
/**** speech.													   ****/
int* window(int* sp_input)
{
 int loop=0;
 int* out;

 out=(int *)malloc(sizeof(int)*Def_framelength);
 for(loop=0;loop<Def_framelength;loop++)
 out[loop]=sp_input[loop]*ham[loop];
 return (out);
}
/** AUTOCORRELATION FUNCTION  **/
/**** input : sp_input:The array containing the windowed segment     ****/
/****       :p : The number of autocorrelation coefficients required ****/ 
/**** return: corr:Array containing the autocorrelation coefficients ****/
/**** The function takes in the hamming windowed segment and         ****/ 
/**** calculates the autocorrelation coefficients(the number specified***/
/**** by input parameter p)for the segment                           ****/ 
int* xcorr(int* sp_input, short p)
{
 short i;
 int j;
 int* corr;

 corr=(int*)malloc((sizeof(int)* p));
 for(i=0;i<p;i++) 
 {
 corr[i]=0;
    // Loop till Def_framelength-i so that index does not spill over
	for (j=0;j<Def_framelength-i;j++)
	 corr[i]=corr[i]+ (sp_input[j]*sp_input[j+i] );
  }
  return (corr);
 } 
/**** LPC FUNCTION    ****/
/**** input : r:Autocorrelation coefficients   *****/
/****     order: The number of LPC coefficients required ****/   
/**** return: alpha : LPC coefficients         *****/
/**** The function calculates the LPC coefficients of a segment ****/
/**** of speech from the correlation coefficients using Levinson-Durbin Algorithm       ****/
float* levinson(int* r, short order)
{
 float *e;
 float *k;
 float *alpha;
 float *alpha_prev;
 int i=1,j=1 ;
 float sum=0.0;
 short loop;
 
 e=(float *)malloc((order+1)*sizeof(float));
 k=(float *)malloc((order+1)*sizeof(float));
 alpha=(float *)malloc((order+1)*sizeof(float));
 alpha_prev=(float *)malloc((order+1)*sizeof(float));
 // Initialising the Array variables
 for (loop=0;loop<order+1;loop++)
 {
  e[loop]=0;
  k[loop]=0;
  alpha[loop]=0;
  alpha_prev[loop]=0;
  }
  // End initalising
  e[0]=r[0];
  // Iterate with i=1 to order as iteration variable
  i=1;
  for(i=1;i<=order;i++)
  {
  // start step 1
  sum=0;
  for (j=1;j<=i-1;j++)
  sum+=alpha_prev[j]*r[i-j];
  k[i]=(r[i]-sum)/e[i-1];
  // End step 1
  //start step 2
  alpha[i]=k[i];
  for(j=1;j<=i-1;j++)
  alpha[j]=alpha_prev[j]- ( k[i]*alpha_prev[i-j] ) ;
  //end step 2
  //start step 3
    e[i]=e[i-1]*(1-(k[i]*k[i]));
  //end step 3
 for( loop=0;loop<order+1;loop++)
 {
  alpha_prev[loop]=alpha[loop];
  }
 }// End For levinson
 for( loop=0;loop<order+1;loop++)
 alpha[loop]=-1*alpha[loop];
 alpha[0]=1;
 free(alpha_prev);
 free(e);
 free(k);
 return (alpha);
}
/**** RESIDUAL FUNCTION  ****/
/**** input : Num_cof: Filter coefficients                          ****/
/****         Nr_size: Number of filter coefficients                ****/    
/****        input : speech segment                                 ****/   
/**** return: sum_numerator : Output of the filter                  ****/
/****  The function implements an FIR filter. Output is the residual****/
/**** signal when given the speech segment as the input and  the    ****/ 
/**** filter coefficients being the LPC parameters                 ****/ 
int* fir_filter(float *Num_cof,short Nr_size,int *fir_input )
{
 int loop_var=0;
 int i;
 int output_loop=0;
 int *sum_numerator;
 float lpc_copy[18];

 for(i=0;i<Nr_size;i++)
 lpc_copy[i]=Num_cof[i];
 sum_numerator=(int *)malloc((sizeof(int))*Def_framelength);
 // numerator
 for(output_loop=0;output_loop<Def_framelength;output_loop++)
 {
  sum_numerator[output_loop]=0;
   for(loop_var=0;(loop_var<=output_loop) && (loop_var<Nr_size) ;loop_var++)
   {
    sum_numerator[output_loop] += lpc_copy[loop_var]*fir_input[output_loop-loop_var];
   }
  }
 return (sum_numerator);
}
/**** SYNTHESIS FUNCTION ****/
/**** input : Den_coff: Filter coefficients                         ****/
/****         Nr_size: Number of filter coefficients                ****/    
/****        input : residual speech signal                         ****/   
/**** return: sum_denom : Filter output(synthesised speeech segment ****/
/**** The function implements an IIR filter. Output is the          ****/
/**** synthesised speeech segment when given the residual signal    ****/
/**** as the input and  the filter coefficients being the LPC       ****/ 
/**** parameters                                                    ****/ 
int* iir_filter(float* Den_coff,short Nr_size,int *iir_input)
{
 int loop_var=0;
 int output_loop=0;
 int *sum_denom;
 
 sum_denom=(int *)malloc((sizeof(int))*Def_framelength);
 for(output_loop=0;output_loop<Def_framelength;output_loop++)
 {
  for(loop_var=1;loop_var<output_loop ;loop_var++)
  {
   sum_denom[output_loop]+= Den_coff[loop_var]*sum_denom[output_loop-loop_var];
   }
  sum_denom[output_loop]=iir_input[output_loop]-sum_denom[output_loop];
 }
 return (sum_denom);
}
/*****  BUFFERING FUNCTION     ****/
/**** input : frame_in: the synthesised speech frame                ****/
/****         start : index of starting position                    ****/   
/**** return: void                                                  ****/
/**** The function puts all the synthesised speech frames together  ****/
/**** to form the final synthesised speech signal                    ****/
void accumulate(int* frame_in,int start)
{
 int loop=0;
 int j=0;
 for (loop=start;loop<Def_framelength;loop++)
 {
  G_final_output[loop]=frame_in[j];
  G_final_output[loop]=G_final_output[loop]>>15; // Scaling 
  j++;
  }
 }
/******************************MAIN FUNCTION********************************/
void main()
{
 int loopvar=0;
 int *present_seg;
 int *corr;
 float* lpc;
 int *residue;
 int *windowed;
 int *frame_out;
 
 comm_intr(); // Initializing the DSK and the interrupts
 printf("start here");
//Memory Allocation to store Output
 G_final_output=(int*)malloc(sizeof(int)*Def_total*2);
// Start of main segmentation
 for (loopvar=0;loopvar<=Def_total-Def_framelength;loopvar+=Def_overlap)
 { 
//Loop iterates for every speech segment.Every speech segment is windowed                                                 ***/
//calculate autocorrelation coefficients,LPC parameters,Residual signal from FIR,                                              ***/
//Synthesised frame from passing residual signal->IIR
//frames buffered->G_final_output array 

  present_seg=segmentation(loopvar); 
 
  windowed=window(present_seg); //multiply by contents of hamming
  free(present_seg);

  corr=xcorr(windowed,10);//calculate autocorrelation 

  lpc=levinson(corr,9);  //calculate LPC coefficients
  free(corr);

  residue=fir_filter(lpc,10,windowed);//calculate residual signal
  free(windowed);
 
  frame_out=iir_filter(lpc,10,residue);//generate synthesized segment
  free(residue);

  accumulate(frame_out,loopvar); //buffer synthesized segment
  free(frame_out);
 } // End Of Segmentation loop
 printf("\n done \n");
 while(1);
}// END OF MAIN PROGRAM

