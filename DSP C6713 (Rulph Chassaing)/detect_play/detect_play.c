//detect_play.c

//include appropriate support files

#include "slevel.h"

#define buffer_length 80000
#pragma DATA_SECTION(buffer,".far")
short buffer[buffer_length];
#pragma DATA_SECTION(playback_buffer,".far")
short playback_buffer[buffer_length];

int buffer_pos = 0;     /* input buffer position */
int playback_pos = 0;   /* playback buffer position */
int gain = 1;           /* output gain */
int duration = 0;       /* signal duration (playback on when duration > 0)*/

short buffer_data(short);           /* function declarations */
void start_playback(int *);
short playback(int *);

interrupt void c_int11()            /* interrupt service routine */
{
	short sample_data;
	int temp;

	sample_data = input_sample();	          /* input data */
	sample_data = buffer_data(sample_data); /* buffer input */
	temp = signal_level(sample_data);	      /* analyze the signal level */
  if (temp > 0 && duration == 0) {   /* if signal detected and playback off */
		duration = temp;
		start_playback(&duration);            /* start playback */
	}
	if (duration > 0)                       /* if playback is on */
		sample_data = playback(&duration);    /* play stored data backwards */
	else
		sample_data = 0;                      /* output zero signal */
	output_sample(sample_data);             /* output data */
	return;
}

void main()
{
  comm_intr();   /* init DSK, codec, McBSP */
  while(1);      /* infinite loop */
}

/* store the input sample in a circular buffer */
short buffer_data(short sample)
{
	buffer[buffer_pos] = sample;	/* store sample */
	buffer_pos++;           	    /* increment buffer position */
	if (buffer_pos > buffer_length)
		buffer_pos = 0;             /* buffer wrap-around */
	return sample;
}

/* set up data structures for playback */
void start_playback(int *duration)
{
	int i;

	if (*duration > buffer_length)
		*duration = buffer_length;	/* adjust duration to <= buffer length */
	playback_pos = buffer_pos;    /* copy buffer pointer */
	for (i=0;i<buffer_length;i++) /* copy buffer */
		playback_buffer[i] = buffer[i];
}

/* play back stored samples in reverse order */
short playback(int *duration)
{
	short output;

	output = playback_buffer[playback_pos]; /* outputting samples in reverse */
	output = gain * output;         /* add gain to output */
	playback_pos--;  /* reducing the count to access the next sample */
	(*duration)--;   /* decrement duration (playback stops when duration == 0)*/
	if (playback_pos < 0)
		playback_pos += buffer_length; /* buffer wrap-around */
	return output;
}
