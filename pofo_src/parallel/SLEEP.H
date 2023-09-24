#ifndef _SLEEP_H__
#define _SLEEP_H__


// The supplied DOS "delay" functions seem to freeze the Portfolio so I wrote a couple
// basic sleep functions

// Sleep until a target clock tick time
#define SLEEP_UNTIL(wakeTime) {                                     \
    while(clock() < wakeTime);                                      \
}

// Sleep for the specified number of ms
#define SIMPLE_SLEEP(ms) {                                          \
    clock_t wakeTime = clock() + ((CLOCKS_PER_SEC * ms) / 1000);    \
    SLEEP_UNTIL(wakeTime);                                          \
}


#endif      // _SLEEP_H__
