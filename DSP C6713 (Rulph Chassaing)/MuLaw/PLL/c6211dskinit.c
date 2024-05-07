/*C6211dskinit.c Includes functions provided by TI,init DSK,AD535,McBSP*/
#include <c6x.h>
#include "c6211dsk.h"
#include "c6211dskinit.h"
#include "c6211interrupts.h"

/*TI routine from codec_poll.c example included with c6211 DSK*/
void mcbsp0_init()    /* set up McBSP0   */
{
    *(unsigned volatile int *)McBSP0_SPCR = 0;			/*  reset serial port                         */
    *(unsigned volatile int *)McBSP0_PCR = 0;           /*  set pin control reg.;                     */
    *(unsigned volatile int *)McBSP0_RCR = 0x10040;     /*  set rx control reg. one 16 bit data/frame */
    *(unsigned volatile int *)McBSP0_XCR = 0x10040;     /*  set tx control reg. one 16 bit data/frame */
    *(unsigned volatile int *)McBSP0_DXR = 0;   
	*(unsigned volatile int *)McBSP0_SPCR = 0x12001;    /*  setup SP control reg.;                    */
}

/*TI routine from codec_poll.c example included with c6211 DSK*/
void mcbsp0_write(int out_data)
{
  int temp;
  temp = *(unsigned volatile int *)McBSP0_SPCR & 0x20000;
  while ( temp == 0)
  {
    temp = *(unsigned volatile int *)McBSP0_SPCR & 0x20000;
  }        
  *(unsigned volatile int *)McBSP0_DXR = out_data;
}

/*  TI routine from codec_poll.c example included with c6211 DSK*/
int mcbsp0_read()
{
  int temp;
  temp = *(unsigned volatile int *)McBSP0_SPCR & 0x2;
  while ( temp == 0)
  {
    temp = *(unsigned volatile int *)McBSP0_SPCR & 0x2;
  }
  temp = *(unsigned volatile int *)McBSP0_DRR;
  return temp;
}
 
/*  TI routine from McBSP_AD535_INIT.c example */
void TLC320AD535_Init()   /*  TLC320AD535 Initialization Routine  */
{
  /*  Setting up AD535 Register 3  */
  mcbsp0_read();
  mcbsp0_write(0); 
  mcbsp0_read();
  mcbsp0_write(0);
  mcbsp0_read();
  mcbsp0_write(0);
  mcbsp0_read();
  mcbsp0_write(1);       /* Send Bit to denote Secondary Communications */
  mcbsp0_read();
  mcbsp0_write(0x0386);  /* Voice Channel Reset, Pre-Amps Selected */
  mcbsp0_read();
  mcbsp0_write(0);       /* Clear Secondary Communications */
  mcbsp0_read();
  mcbsp0_write(0);
  mcbsp0_read();
  mcbsp0_write(0);
  mcbsp0_read();
  mcbsp0_write(1);       /* Send Bit to denote Secondary Communications */
  mcbsp0_read();
  mcbsp0_write(0x0306);  /* Voice Channel Clear Reset, Pre-Amps Selected */
  mcbsp0_read();
  mcbsp0_write(0);       /* Clear Secondary Communications */
  mcbsp0_read();    
  
  /*    Setting up AD535 Register 4 */
  mcbsp0_write(0);
  mcbsp0_read();
  mcbsp0_write(0);
  mcbsp0_read();
  mcbsp0_write(1);
  mcbsp0_read();
  mcbsp0_write(0x0400);  /* Set Microphone Pre-Amp Gain to 20 dB */
  mcbsp0_read();
  mcbsp0_write(0);
  mcbsp0_read();

  /*    Setting up AD535 Register 5 */
  mcbsp0_write(0);
  mcbsp0_read();
  mcbsp0_write(0);
  mcbsp0_read();
  mcbsp0_write(1);
  mcbsp0_read();
  mcbsp0_write(0x0502);  /* DAC PGA = 0 dB */
  mcbsp0_read();
  mcbsp0_write(0);
  mcbsp0_read();
}   

void c6211_dsk_init()       /* dsp and periphiral initialization  */
{
  CSR=0x100;    	       /* disable all interrupts             */
  IER=1;                   /* disable all interrupts except NMI  */
  ICR=0xffff;              /* clear all pending interrupts       */
  *(unsigned volatile int *)EMIF_GCR = 0x3300; 	      /* EMIF global control          */
  *(unsigned volatile int *)EMIF_CE0 = 0x30;          /* EMIF CE0control              */
  *(unsigned volatile int *)EMIF_CE1 = 0xffffff03;	  /* EMIF CE1 control, 8bit async */
  *(unsigned volatile int *)EMIF_SDCTRL = 0x07117000; /* EMIF SDRAM control           */
  *(unsigned volatile int *)EMIF_SDRP = 0x61a;        /* EMIF SDRM refresh period     */
  *(unsigned volatile int *)EMIF_SDEXT = 0x54519;     /* EMIF SDRAM extension         */
  mcbsp0_init();
  TLC320AD535_Init();
}   

void comm_poll()		/*added for communication/initialization using polling*/
{
    c6211_dsk_init();
}  

void comm_intr()		/*added for communication/initialization using interrupt*/
{ 
  c6211_dsk_init();
  config_Interrupt_Selector(11, XINT1); /*using transmit INT11*/
  enableSpecificINT(11);                /*for specific interrupt*/
  enableNMI();              			/*Enable NMI*/
  enableGlobalINT(); 					/*Enable GIE for global interrupt*/
  mcbsp0_write(0);   					/*write to SP0*/
}    

void output_sample(int out_data)        /*added for output*/
{
  mcbsp0_write(out_data & 0xfffe);      /*mask out LSB*/
}


int input_sample()                      /*added for input*/
{
   return mcbsp0_read();
}
