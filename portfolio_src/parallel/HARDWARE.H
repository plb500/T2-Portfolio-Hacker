#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <time.h>

// Initialize the various Parallel Interface (8255) Ports
void init_ports();

// Reset the connected hardware
void reset_hacker_box();

// Close down the connected hardware
void shutdown_hacker_box();

// Trigger the connected hardware to write the digit at the supplied digit index
void request_pin_digit(char digit);

// Read the digit returned by the connected hardware
char read_pin_digit();

// Returns whether the connected hardware is ready (has a PIN) or not
int hacker_box_ready();


#endif      // __HARDWARE_H__
