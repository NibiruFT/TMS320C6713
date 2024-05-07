// Signal level analysis routine

#include "block_dc.h"
#include "detect_envelope.h"
/*
 * Analyze the characteristics of the input
 * returns 0 when no command is generated
 * returns a non-zero number of samples when detecting a signal of
 *   large enough signal level, long enough duration and
 *   the signal has been low for a specified period of time.
 *   Returned integer indicates the duration of the detected signal.
 */
#define threshold_low   200 	/* signal loss threshold */
#define threshold_high  400 	/* signal detection threshold */
#define threshold_start 500   /* delay before turning the signal on */
#define threshold_stop  3000  /* delay before turning the signal off */
#define extra_samples   600 	/* extra duration so signal is not chopped */
short signal_on = 0;          /* "signal present" flag */
short signal_found = 0;       /* "approved signal present" flag */
int duration1 = 0;            /* approved signal duration */
int duration_lost = 0;        /* signal loss duration */
int signal_level(short sample)
{
  int signal;
  int ret = 0;

  signal = detect_envelope(block_dc(sample)); /* approx. signal envelope */
  if (signal_on) {                /* an approved signal is in progress */
    duration1++;
    if (signal < threshold_low) { /* if the signal is low */
      duration_lost++;            /* accumulate signal loss duration */
      if (duration_lost > threshold_stop) { /* signal lost: output duration */
        ret = duration1 + extra_samples;    /* return signal duration */
        signal_on = 0;                      /* indicate the signal is lost */
        signal_found = 0;
        duration1 = 0;
      }
    }
    else {
      duration_lost = 0;          /* reset signal loss duration */
    }
  }
  else if (signal_found) { /* a large enough signal was recently detected  */
    if (signal < threshold_low) { /* signal lost: reset duration */
      signal_found = 0;
      duration1 = 0;
    }
    else {
      duration1++;
      if (duration1 > threshold_start) { /* signal is approved (not noise) */
        signal_on = 1;
        duration_lost = 0;
      }
    }
  }
  else if (signal > threshold_high) { /* a large enough signal is observed */
    signal_found = 1;        /* start signal tracking */
    duration1 = 1;
  }
  return ret;
}
