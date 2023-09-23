#ifndef _PR_DEFS_H_
#define _PR_DEFS_H_


#define MAX_PIN_LENGTH              (4)
#define INVALID_PIN_VALUE           ('X')

#define INT_TO_CHAR(X)              (X + '0')
#define CHAR_TO_INT(X)              (X - '0')


/*
 * The supplied DOS "delay" functions seem to freeze the Portfolio so I wrote a couple
 * basic sleep functions
 */


/* Sleep until a target clock tick time */
#define SLEEP_UNTIL(wakeTime) {                                     \
    while(clock() < wakeTime);                                      \
}

/* Sleep for the specified number of ms */
#define SIMPLE_SLEEP(ms) {                                          \
    clock_t wakeTime = clock() + ((CLK_TCK * ms) / 1000);           \
    SLEEP_UNTIL(wakeTime);                                          \
}


#endif      // _PR_DEFS_H_
