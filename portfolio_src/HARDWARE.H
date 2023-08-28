#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <time.h>

#define SIMPLE_SLEEP(ms) {                                          \
    clock_t wakeTime = clock() + ((CLOCKS_PER_SEC * ms) / 1000);    \
    while(clock() < wakeTime);                                      \
}

#define SLEEP_UNTIL(wakeTime) {                                     \
    while(clock() < wakeTime);                                      \
}

void init_ports();
void reset_hacker_box();
void shutdown_hacker_box();
void request_pin_digit(char digit);
char read_pin_digit();

int hacker_box_ready();


#endif      // __HARDWARE_H__
