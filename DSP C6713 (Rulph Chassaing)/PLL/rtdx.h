#ifndef __RTDX_H
#define __RTDX_H
/***********************************************************************
* VERSION : $Revision: 1.5 $
* DATE    : $Date: 1998/10/23 00:14:37 $
* Copyright (c) 1997 Texas Instruments Incorporated
*
* - The RTDX User Library Layer
*
************************************************************************/

/* This controls configuration to be either polling or interrupt driven */
/* It must be defined - values are 0 or 1.                              */
#undef RTDX_POLLING_IMPEMENTATION
#define RTDX_POLLING_IMPLEMENTATION 0
#if RTDX_POLLING_IMPLEMENTATION
extern void RTDX_Poll( void );
#endif

/* return codes used by read functions */
#define RTDX_READ_ERROR	-1
#define RTDX_OK			1

/* ----------------------------------------------------------------------
*  Define Data Types
*
* Data channels are represented by statically declared structures.  A data
* channel may be used either for input or output (logging), but not both.
* The contents of an input or output structure is private to the
* implementation and may change at any time.
*
* -----------------------------------------------------------------------*/


/* --- Data Channels --- */
#define RTDX_CHAN_ENABLED    0x1010

typedef struct {
	/* private */					/* NOTE: enabled MUST BE THE FIRST FIELD */
	volatile unsigned int enabled;
} RTDX_output_channel;

#define RTDX_OUTPUT_INITIAL_STATE	{0}

typedef struct {
	/* private */					/* NOTE: enabled MUST BE THE FIRST FIELD */
	volatile unsigned int enabled;
	volatile unsigned int count;	/* HOST puts actual bytes received here	*/
	volatile int busy;				/* 1 if channel busy reading.			*/
} RTDX_input_channel;

#define RTDX_INPUT_INITIAL_STATE	{0,0,0}

/* For DSPBios Compatability	*/
#define RTDX_outputChannel	RTDX_output_channel
#define RTDX_inputChannel	RTDX_input_channel


/* Macros to hide structure initialization data	*/
#define	RTDX_CreateOutputChannel(c)	\
		RTDX_outputChannel c = RTDX_OUTPUT_INITIAL_STATE
#define	RTDX_CreateInputChannel(c)	\
		RTDX_inputChannel c = RTDX_INPUT_INITIAL_STATE


/* This variable is set to non-NULL if transport is busy transmitting data */
extern void * volatile RTDX_writing;	/* exported from buffer layer   */


/************************************************************************
 * Procedure:
 *      RTDX_enableOutput
 *      RTDX_disableOutput
 *      RTDX_enableInput
 *      RTDX_disableInput
 * Parameters:
 *      ochan   identifier for an output channel
 *      ichan   identifier for an input  channel
 *
 * All input/output channels are initialized to be disabled.  A call to
 * the input/output enable routine causes the specified channel to be
 * enabled for data transmision.  Normally - user code will not call
 * these functions.  Enabling/Disabling is typically done by the
 * host-resident debugger.
 ************************************************************************/

extern void RTDX_enableOutput	( RTDX_output_channel *ochan);
extern void RTDX_disableOutput	( RTDX_output_channel *ochan);
extern void RTDX_enableInput	( RTDX_input_channel  *ichan);
extern void RTDX_disableInput	( RTDX_input_channel  *ichan);

/* For DSPBios Compatability	*/
#define RTDX_Enable_Data_Output		RTDX_enableOutput
#define RTDX_Disable_Data_Output	RTDX_disableOutput
#define RTDX_Enable_Data_Input		RTDX_enableInput
#define RTDX_Disable_Data_Input		RTDX_disableInput

/* To convert function calls to macros where possible	*/
#define RTDX_enableOutput(c)		( (c)->enabled = RTDX_CHAN_ENABLED )

/* CDM 10/22 */
/* This macro did not initialize the busy state, use the underlying function instead */
/* #define RTDX_enableInput(c)			( (c)->enabled = RTDX_CHAN_ENABLED ) */
#define RTDX_disableOutput(c)		( (c)->enabled = 0 )
#define RTDX_disableInput(c)		( (c)->enabled = 0 )

#define RTDX_isInputEnabled(zzz) ((zzz)->enabled == RTDX_CHAN_ENABLED)
#define RTDX_isOutputEnabled(zzz) ((zzz)->enabled == RTDX_CHAN_ENABLED)


/************************************************************************
 * Procedure:
 *      RTDX_write
 * Parameters:
 *      ochan   identifier for an output channel
 *      buffer  a pointer to the buffer to contain the data
 *      bsize   the size of the buffer in address units
 * Returns:
 *      int     Success ( non-zero ) / Fail (0)
 *
 * RTDX_write causes the specified data to be written to the specified
 * output channel, provided the channel is enabled.  On return from
 * RTDX_write, the data has been copied out of the specified buffer.
 * If the channel is not enabled, RTDX_write is a no-op that returns success.
 ************************************************************************/

extern int RTDX_write ( RTDX_output_channel *ochan, void *buffer, int bsize);
#define RTDX_Data_Write RTDX_write

/***************************************************************************
 * Template:
 *      int RTDX_read ( RTDX_input_channel *ichan, char *buffer, int bsize);
 *
 * Parameters:
 * 	ichan	identifier for the input channel to use
 * 	buffer	a pointer to the buffer to contain the data
 * 	bsize	the size of the buffer in address units
 *
 * Effect:
 *	RTDX_read causes a read request to be posted to the specified input
 * channel.  If the channel is enabled, RTDX_read then busy waits until the
 * data has arrived. On return from RTDX_read, the data has been copied
 * into the specified buffer.
 *
 * Returns:
 *	RTDX_READ_ERROR if the channel is not enabled, or already busy.
 * Otherwise, RTDX_read returns the actual number of units read, in
 * sizeof() units.
 *
 ***************************************************************************/
extern int RTDX_read ( RTDX_input_channel *ichan, void *buffer, int bsize);
#define RTDX_Data_Read RTDX_read


/***************************************************************************
 * Template:
 *      int RTDX_readNB ( RTDX_input_channel *ichan, char *buffer, int bsize);
 *
 * Parameters:
 * 	ichan	identifier for the input channel to use
 * 	buffer	a pointer to the buffer to contain the data
 * 	bsize	the size of the buffer in address units
 *
 * Effect:
 *	RTDX_readNB causes a read request to be posted to the specified
 * input channel.  RTDX_readNB posts a read request to the host, and then
 * immediately returns.  The user must wait for the data to arrive; a call
 * to RTDX_channelBusy can be used to poll the channel for completion.
 *
 * Returns:
 *	RTDX_READ_ERROR if the channel is not enabled, or busy.
 *	0 if the request cannot be posted due to the buffers being full.
 *	RTDX_OK for success.
 *
 ***************************************************************************/
extern int RTDX_readNB ( RTDX_input_channel *ichan, void *buffer, int bsize);
#define RTDX_Data_Read_NB RTDX_readNB


/************************************************************************
 * Procedure:
 *      RTDX_channelBusy
 *
 * Parameters:
 *      ichan   identifier for an input  channel
 *
 * Returns:
 *      TRUE if the channel is currently busy awaiting input.  FALSE
 * if it is not busy, meaning any previous request has completed.
 *
 * Notes:
 *	Usually this function is an inline expanded macro.
 ************************************************************************/
extern int RTDX_channelBusy( RTDX_input_channel *ichan );
#define RTDX_channelBusy( pichan )     ( (pichan)->busy )
#define RTDX_Channel_Busy RTDX_channelBusy


/************************************************************************
 * Procedure:
 *      RTDX_sizeofInput
 *
 * Parameters:
 *      ichan   identifier for an input  channel
 *
 * Returns:
 *      The amount of data actually read by the last completed read
 * operation, in sizeof() units, is returned.  The result of
 * RTDX_sizeofInput is not valid if the channel is currently busy.
 *
 * Notes:
 *	Usually this function is an inline expanded macro.
 ************************************************************************/
extern int RTDX_sizeofInput( RTDX_input_channel *ichan );
#define RTDX_sizeofInput( pichan )       ( (pichan)->count )
#define RTDX_Bytes_Read RTDX_sizeofInput


#endif  // __RTDX_H
