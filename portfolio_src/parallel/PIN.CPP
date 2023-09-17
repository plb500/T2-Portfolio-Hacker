#include <stdio.h>
#include <stdlib.h>

#include "display.h"
#include "hardware.h"
#include "sleep.h"


extern char *PIN_STRINGS[];


// Scroll the "random" digits until the hardware has returned the final PIN
void wait_for_hacker_box_pin(char *finalPin) {
    clock_t displayTimePeriod = ((CLOCKS_PER_SEC * 20) / 1000);
    clock_t pinReadTimePeriod = ((CLOCKS_PER_SEC * 60) / 1000);
    
    clock_t nextDisplayTime;
    clock_t pinReadTime;
    
    int scrollIndex = 0;
    int gotCurrentPIN = 0;

    // Keep scrolling until we are informed that the box has the PIN
    while(!hacker_box_ready()) {
        nextDisplayTime = (clock() + displayTimePeriod);

        click_and_display(PIN_STRINGS[scrollIndex]);

        scrollIndex = ((scrollIndex + 1) % NUM_PIN_STRINGS);
        
        SLEEP_UNTIL(nextDisplayTime)
    }
    
    // Box has PIN, keep scrolling until we have obtained all PIN values
    for(int i = 0; i < 4; ++i) {
        gotCurrentPIN = 0;
        request_pin_digit(i);
        pinReadTime = (clock() + pinReadTimePeriod);

        while(!gotCurrentPIN) {
            if(clock() > pinReadTime) {
               finalPin[i] = read_pin_digit();
            
                break;
            }        
            nextDisplayTime = (clock() + displayTimePeriod);

            click_and_display(PIN_STRINGS[scrollIndex]);

            scrollIndex = ((scrollIndex + 1) % NUM_PIN_STRINGS);
        
            SLEEP_UNTIL(nextDisplayTime)
        }
    }
}

void main(int argc, char **argv) {
    if(argc > 2) {
        fprintf(stdout, "Too many arguments supplied\n");
        fprintf(stdout, "0 arguments will attempt to read from hardware\n");
        fprintf(stdout, "1 argument will bypass hardware and display supplied PIN\n");
        exit(1);
    }

    int usingHardware = (argc < 2);
    char *finalPIN = usingHardware ? "9003" : argv[1];

    // Initialize hardware
    if(usingHardware) {
        init_ports();
        reset_hacker_box();    
    }

    // Display initial user prompt
    splash_screen();

    // Scroll display until the box returns a PIN
    if(usingHardware) {
        wait_for_hacker_box_pin(finalPIN);
    }

    // Scan to the final PIN string
    pin_scan(finalPIN);
    
    // Exit cleanly
    if(usingHardware) {
        shutdown_hacker_box();
    }
}
