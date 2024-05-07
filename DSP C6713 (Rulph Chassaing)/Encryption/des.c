// des.c	DES (Data Encryption Standard) software emulation
#include "des.h"
#include "c6xdsk.h"
#include <stdio.h>

reg key;					//Private key stored for encryption / decryption
bitarry keyt[16];			//key transforms 48bits each round

bitarry left_sample[17];	//data array holding each of round of data samples
bitarry right_sample[17];
bitarry f_output;			//bit array for f_function() return data
bitarry initial;			//bit array for permutations in initial permutation
bitarry final;				//bit array for permutations in final permutation
reg sample_data;

int input_sample1 = 0;		//Input samples from codec
int input_sample2 = 0;
int output_sample1 = 0;		//Output samples from codec
int output_sample2 = 0;

short round = 0;		//flag to determine which sample to take
short start = 0;			//polling flag set when second sample is acquired
short encrypt = 0; 			//flag to determine if encrpting or decrypting
short decrypt = 0;			//flag to turn decrypting on/off from switch #2
unsigned int enable;		//enable flag to store dip switch #3 setting

//------------------- Function Prototypes -------------------------//
void initial_perm();
void final_perm();
void f_function(int);
void key_scheduler();
unsigned int get_ioport();

//--------------- Interrupt Service Routine #11 -------------------//
interrupt void c_int11()
{
  short i,j;

  if(round==0) {				//storing 32 bits
    sample_data.intreg.int1 = input_sample();	//input first data item
    output_sample(output_sample1);	//output second data item
  }

  if(round==1) {				//using the stored 32 bits to achieve 64bit sample
    output_sample(output_sample2);	//output first data item
    sample_data.intreg.int2 = input_sample();	//input second data item

    if(enable==1) {
      initial_perm();					//performing the initial permutation
      encrypt = 0;					//Decrypting
      for (i=0; i<=14; i++) {			//For 16 rounds of encryption
        left_sample[i+1]=right_sample[i];		//Next Left sample is old right_sample
        f_function(i);						//right samplef_output = right_sample[i];// is put through f_function
	    for (j=0; j<32; j++)					//output from f_function is xor-ed w/ old left_sample
          right_sample[i+1].b[j]=f_output.b[j] ^ left_sample[i].b[j];
      } 
      final_perm();					//Final permutation performed

      if(decrypt==1) {
        initial_perm();					//Initial permutation for decryption
        encrypt = 1;					//Decrypting
        for (i=0; i<=14; i++) {			//For 16 Rounds	same process is followed
          left_sample[i+1]=right_sample[i];	//Except the keys are reversed within the f_function
 	      f_function(i);
          for (j=0; j<32; j++)
            right_sample[i+1].b[j]=f_output.b[j]^left_sample[i].b[j];
        } 
	    final_perm();
	  }
    }

    output_sample1 = sample_data.intreg.int1;	//output samples
    output_sample2 = sample_data.intreg.int2;
  }

  round^=1;					//alternate buffering and encrypting
  return;
}

//---------------------Main Procedure--------------------
void main()
{
  key_scheduler();			//create key transforms for each round	
  comm_intr();              //init DSK, codec, McBSP
  while(1);                 //infinite loop
}

//-----------------key_scheduler() function------------------
// Subroutine takes the 64-bit binary value in key and
//   transforms it into 16 individual 46-bit keys in the
//   array, keyt[16]
void key_scheduler()
{
  short i, j;			//counter variables
  bitarry ckey, dkey, init_key;		//temporary bit array
  unsigned int io_port_values, user_dip_settings;

  io_port_values = get_ioport();			//read I/O ports
  user_dip_settings = io_port_values & 0x7000000;	//mask I/O port value to get settings from user switches


  if ((user_dip_settings & 0x04000000) == 0x04000000) {		//enable
	enable = 1;
	printf("DES Enabled\n");
  }
  else {													//disable
	enable = 0;
	printf("DES Disabled\n");
  }

if(enable==1) {
  if ((user_dip_settings & 0x02000000) == 0x02000000) {		//encrypt
	decrypt = 1;
	printf("Encrypting and Decrypting Data\n");
  }
  else {													//decrypt
	decrypt = 0;
	printf("Encrypting Data Only\n");
  }

  if ((user_dip_settings & 0x01000000) == 0x00000000) {		//key1
	key.intreg.int1 = key1l;
	key.intreg.int2 = key1h;
  	printf("Using Key1: 0x45E016B759DB4AF8\n");
  }

  else {													//key2
	key.intreg.int1 = key2l;
	key.intreg.int2 = key2h;
	printf("Using Key2: 0x9836BD176BE86AFA\n");
  }
}

init_key.b[0] = key.bitreg.b0;
init_key.b[1] = key.bitreg.b1;
init_key.b[2] = key.bitreg.b2;
init_key.b[3] = key.bitreg.b3;
init_key.b[4] = key.bitreg.b4;
init_key.b[5] = key.bitreg.b5;
init_key.b[6] = key.bitreg.b6;
init_key.b[7] = key.bitreg.b7;
init_key.b[8] = key.bitreg.b8;
init_key.b[9] = key.bitreg.b9;
init_key.b[10] = key.bitreg.b10;
init_key.b[11] = key.bitreg.b11;
init_key.b[12] = key.bitreg.b12;
init_key.b[13] = key.bitreg.b13;
init_key.b[14] = key.bitreg.b14;
init_key.b[15] = key.bitreg.b15;
init_key.b[16] = key.bitreg.b16;
init_key.b[17] = key.bitreg.b17;
init_key.b[18] = key.bitreg.b18;
init_key.b[19] = key.bitreg.b19;
init_key.b[20] = key.bitreg.b20;
init_key.b[21] = key.bitreg.b21;
init_key.b[22] = key.bitreg.b22;
init_key.b[23] = key.bitreg.b23;
init_key.b[24] = key.bitreg.b24;
init_key.b[25] = key.bitreg.b25;
init_key.b[26] = key.bitreg.b26;
init_key.b[27] = key.bitreg.b27;
init_key.b[28] = key.bitreg.b28;
init_key.b[29] = key.bitreg.b29;
init_key.b[30] = key.bitreg.b30;
init_key.b[31] = key.bitreg.b31;
init_key.b[32] = key.bitreg.b32;
init_key.b[33] = key.bitreg.b33;
init_key.b[34] = key.bitreg.b34;
init_key.b[35] = key.bitreg.b35;
init_key.b[36] = key.bitreg.b36;
init_key.b[37] = key.bitreg.b37;
init_key.b[38] = key.bitreg.b38;
init_key.b[39] = key.bitreg.b39;
init_key.b[40] = key.bitreg.b40;
init_key.b[41] = key.bitreg.b41;
init_key.b[42] = key.bitreg.b42;
init_key.b[43] = key.bitreg.b43;
init_key.b[44] = key.bitreg.b44;
init_key.b[45] = key.bitreg.b45;
init_key.b[46] = key.bitreg.b46;
init_key.b[47] = key.bitreg.b47;
init_key.b[48] = key.bitreg.b48;
init_key.b[49] = key.bitreg.b49;
init_key.b[50] = key.bitreg.b50;
init_key.b[51] = key.bitreg.b51;
init_key.b[52] = key.bitreg.b52;
init_key.b[53] = key.bitreg.b53;
init_key.b[54] = key.bitreg.b54;
init_key.b[55] = key.bitreg.b55;
init_key.b[56] = key.bitreg.b56;
init_key.b[57] = key.bitreg.b57;
init_key.b[58] = key.bitreg.b58;
init_key.b[59] = key.bitreg.b59;
init_key.b[60] = key.bitreg.b60;
init_key.b[61] = key.bitreg.b61;
init_key.b[62] = key.bitreg.b62;
init_key.b[63] = key.bitreg.b63;

// PC-1 = removes all parity bits and performs permutation
    for(j=0; j<=55; j++) {
      if(j<=27)
        dkey.b[j] = init_key.b[PC1[j]-1];
      else
        ckey.b[j-28] = init_key.b[PC1[j]-1];
    }

// begin finding 16 different key transformations
  for(i=0; i<=15; i++) {

    for(j=27; j>=0; j--) {		//left shift ckey[i] and dkey[i] once
      dkey.b[j+1] = dkey.b[j];
      ckey.b[j+1] = ckey.b[j];
    }
    dkey.b[0] = dkey.b[28];
    ckey.b[0] = ckey.b[28];

// double left shift for all iterations except 1, 2, 9, and 16
    if (i!=0 && i!=1 && i!=8 && i!=15) {

      for(j=27; j>=0; j--) {		//shift ckey[i] and dkey[i] once more
        dkey.b[j+1] = dkey.b[j];
        ckey.b[j+1] = ckey.b[j];
      }
      dkey.b[0] = dkey.b[28];
      ckey.b[0] = ckey.b[28];
    }

// perform PC-2 on ckey[i] and dkey[i]
    for(j=0; j<=47; j++) {
      if(PC2[j]<=27)
        keyt[i].b[j] = dkey.b[PC2[j]-1];
      else
        keyt[i].b[j] = ckey.b[PC2[j]-28];
    }						// end for loop (repeat for all 16 keys)
  }
return;
}


//-----------------get_ioport() function------------------
// Subroutine reads the state of ioport
unsigned int get_ioport(void)
{
  return(*(unsigned volatile int *)IO_PORT);
}


//-----------------initial_perm() function------------------
void initial_perm()
{
short i;


initial.b[0] = sample_data.bitreg.b0;
initial.b[1] = sample_data.bitreg.b1;
initial.b[2] = sample_data.bitreg.b2;
initial.b[3] = sample_data.bitreg.b3;
initial.b[4] = sample_data.bitreg.b4;
initial.b[5] = sample_data.bitreg.b5;
initial.b[6] = sample_data.bitreg.b6;
initial.b[7] = sample_data.bitreg.b7;
initial.b[8] = sample_data.bitreg.b8;
initial.b[9] = sample_data.bitreg.b9;
initial.b[10] = sample_data.bitreg.b10;
initial.b[11] = sample_data.bitreg.b11;
initial.b[12] = sample_data.bitreg.b12;
initial.b[13] = sample_data.bitreg.b13;
initial.b[14] = sample_data.bitreg.b14;
initial.b[15] = sample_data.bitreg.b15; 
initial.b[16] = sample_data.bitreg.b16;
initial.b[17] = sample_data.bitreg.b17; 
initial.b[18] = sample_data.bitreg.b18; 
initial.b[19] = sample_data.bitreg.b19;
initial.b[20] = sample_data.bitreg.b20;
initial.b[21] = sample_data.bitreg.b21;
initial.b[22] = sample_data.bitreg.b22;
initial.b[23] = sample_data.bitreg.b23; 
initial.b[24] = sample_data.bitreg.b24; 
initial.b[25] = sample_data.bitreg.b25; 
initial.b[26] = sample_data.bitreg.b26;
initial.b[27] = sample_data.bitreg.b27; 
initial.b[28] = sample_data.bitreg.b28; 
initial.b[29] = sample_data.bitreg.b29;
initial.b[30] = sample_data.bitreg.b30;
initial.b[31] = sample_data.bitreg.b31;
initial.b[32] = sample_data.bitreg.b32;
initial.b[33] = sample_data.bitreg.b33;
initial.b[34] = sample_data.bitreg.b34;
initial.b[35] = sample_data.bitreg.b35;
initial.b[36] = sample_data.bitreg.b36; 
initial.b[37] = sample_data.bitreg.b37;
initial.b[38] = sample_data.bitreg.b38;
initial.b[39] = sample_data.bitreg.b39;
initial.b[40] = sample_data.bitreg.b40; 
initial.b[41] = sample_data.bitreg.b41;
initial.b[42] = sample_data.bitreg.b42; 
initial.b[43] = sample_data.bitreg.b43;
initial.b[44] = sample_data.bitreg.b44; 
initial.b[45] = sample_data.bitreg.b45; 
initial.b[46] = sample_data.bitreg.b46; 
initial.b[47] = sample_data.bitreg.b47; 
initial.b[48] = sample_data.bitreg.b48; 
initial.b[49] = sample_data.bitreg.b49; 
initial.b[50] = sample_data.bitreg.b50;
initial.b[51] = sample_data.bitreg.b51; 
initial.b[52] = sample_data.bitreg.b52; 
initial.b[53] = sample_data.bitreg.b53;
initial.b[54] = sample_data.bitreg.b54;
initial.b[55] = sample_data.bitreg.b55; 
initial.b[56] = sample_data.bitreg.b56;
initial.b[57] = sample_data.bitreg.b57; 
initial.b[58] = sample_data.bitreg.b58;
initial.b[59] = sample_data.bitreg.b59;
initial.b[60] = sample_data.bitreg.b60;
initial.b[61] = sample_data.bitreg.b61;
initial.b[62] = sample_data.bitreg.b62;
initial.b[63] = sample_data.bitreg.b63;

for(i=0; i<64; i++) {
    if(i<32)
      left_sample[0].b[i] = initial.b[IP[i]-1];
    else
      right_sample[0].b[i-32] = initial.b[IP[i]-1];
  }
return;
}

//-------------------final_perm() function-------------------
void final_perm()
{
short i;

for(i=0; i<64; i++) {
  if(i<32)
    final.b[IP[i]-1] = right_sample[15].b[i];
  else
    final.b[IP[i]-1] = left_sample[15].b[i-32];
  }

sample_data.bitreg.b0 = final.b[0];
sample_data.bitreg.b1 = final.b[1]; 
sample_data.bitreg.b2 = final.b[2]; 
sample_data.bitreg.b3 = final.b[3];
sample_data.bitreg.b4 = final.b[4];
sample_data.bitreg.b5 = final.b[5];
sample_data.bitreg.b6 = final.b[6];
sample_data.bitreg.b7 = final.b[7];
sample_data.bitreg.b8 = final.b[8];
sample_data.bitreg.b9 = final.b[9];
sample_data.bitreg.b10 = final.b[10];
sample_data.bitreg.b11 = final.b[11];
sample_data.bitreg.b12 = final.b[12];
sample_data.bitreg.b13 = final.b[13];
sample_data.bitreg.b14 = final.b[14];
sample_data.bitreg.b15 = final.b[15];
sample_data.bitreg.b16 = final.b[16];
sample_data.bitreg.b17 = final.b[17];
sample_data.bitreg.b18 = final.b[18];
sample_data.bitreg.b19 = final.b[19];
sample_data.bitreg.b20 = final.b[20];
sample_data.bitreg.b21 = final.b[21];
sample_data.bitreg.b22 = final.b[22];
sample_data.bitreg.b23 = final.b[23];
sample_data.bitreg.b24 = final.b[24];
sample_data.bitreg.b25 = final.b[25];
sample_data.bitreg.b26 = final.b[26];
sample_data.bitreg.b27 = final.b[27];
sample_data.bitreg.b28 = final.b[28];
sample_data.bitreg.b29 = final.b[29];
sample_data.bitreg.b30 = final.b[30];
sample_data.bitreg.b31 = final.b[31];
sample_data.bitreg.b32 = final.b[32];
sample_data.bitreg.b33 = final.b[33];
sample_data.bitreg.b34 = final.b[34];
sample_data.bitreg.b35 = final.b[35];
sample_data.bitreg.b36 = final.b[36];
sample_data.bitreg.b37 = final.b[37];
sample_data.bitreg.b38 = final.b[38];
sample_data.bitreg.b39 = final.b[39];
sample_data.bitreg.b40 = final.b[40];
sample_data.bitreg.b41 = final.b[41];
sample_data.bitreg.b42 = final.b[42];
sample_data.bitreg.b43 = final.b[43];
sample_data.bitreg.b44 = final.b[44];
sample_data.bitreg.b45 = final.b[45];
sample_data.bitreg.b46 = final.b[46];
sample_data.bitreg.b47 = final.b[47];
sample_data.bitreg.b48 = final.b[48];
sample_data.bitreg.b49 = final.b[49];
sample_data.bitreg.b50 = final.b[50];
sample_data.bitreg.b51 = final.b[51];
sample_data.bitreg.b52 = final.b[52];
sample_data.bitreg.b53 = final.b[53];
sample_data.bitreg.b54 = final.b[54];
sample_data.bitreg.b55 = final.b[55];
sample_data.bitreg.b56 = final.b[56];
sample_data.bitreg.b57 = final.b[57];
sample_data.bitreg.b58 = final.b[58];
sample_data.bitreg.b59 = final.b[59];
sample_data.bitreg.b60 = final.b[60];
sample_data.bitreg.b61 = final.b[61];
sample_data.bitreg.b62 = final.b[62];
sample_data.bitreg.b63 = final.b[63]; 

return;
}

//-----------------f_function() function--------------------
void f_function(i)
{
  short j;
  short col = 0, row = 0;
  svalue value;
  bitarry temp;
  
  for(j=0; j<48; j++) {
  	temp.b[j] = right_sample[i].b[E[j]-1];			//expand previous right sa
  	if (encrypt == 0) 
  	  	temp.b[j] ^= keyt[i].b[j];				//xor with keyt[i]
	if (encrypt == 1)
    	temp.b[j] ^= keyt[14-i].b[j];				//xor with keyt[i]
  }
  
  for(j=0; j<8; j++) {				//perform permutations with S-blocks
    if(temp.b[0+6*j] == 1)
      row++;
    if(temp.b[5+6*j] == 1)
      row += 2;
    if(temp.b[1+6*j] == 1)
      col++;
    if(temp.b[2+6*j] == 1)
      col += 2;
    if(temp.b[3+6*j] == 1)
      col += 4;
    if(temp.b[4+6*j] == 1)
      col += 8;
	
    switch(j) {					//get decimal value from corresponding S-block
      case 0: value.intvalue = S1[row*16+col]; break;
      case 1: value.intvalue = S2[row*16+col]; break;
      case 2: value.intvalue = S3[row*16+col]; break;
      case 3: value.intvalue = S4[row*16+col]; break;
      case 4: value.intvalue = S5[row*16+col]; break;
      case 5: value.intvalue = S6[row*16+col]; break;
      case 6: value.intvalue = S7[row*16+col]; break;
      case 7: value.intvalue = S8[row*16+col];
    }
	
    temp.b[0+6*j] = value.bitvalue.b0;
    temp.b[1+6*j] = value.bitvalue.b1;
    temp.b[2+6*j] = value.bitvalue.b2;
    temp.b[3+6*j] = value.bitvalue.b3;
    
    row = 0;
    col = 0;
  }

  for(j=0; j<=31; j++)
  	f_output.b[j] = temp.b[Pbox[j]-1];
    
  return;
}

