#include "display.h"
#include "hardware.h"
#include "sleep.h"


extern char *PIN_STRINGS[];


/* Scroll the "random" digits until the hardware has returned the final PIN */
void wait_for_hacker_box_pin(char *finalPin) {
    clock_t displayTimePeriod = ((CLK_TCK * 20) / 1000);
    clock_t pinReadTimePeriod = ((CLK_TCK * 60) / 1000);
    
    clock_t nextDisplayTime;
    clock_t pinReadTime;
    
    int scrollIndex = 0;
    int gotCurrentPIN = 0;
    int i;

    /* Keep scrolling until we are informed that the box has the PIN */
    while(!hacker_box_ready()) {
        nextDisplayTime = (clock() + displayTimePeriod);

        click_and_display(PIN_STRINGS[scrollIndex]);

        scrollIndex = ((scrollIndex + 1) % NUM_PIN_STRINGS);
        
        SLEEP_UNTIL(nextDisplayTime)
    }
    
    /* Box has PIN, keep scrolling until we have obtained all PIN values */
    for(i = 0; i < 4; ++i) {
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
    int usingHardware;
    char *finalPIN = "9003";
    
    if(argc > 2) {
        puts("Too many arguments supplied");
        puts("0 arguments will attempt to read from hardware");
        puts("1 argument will bypass hardware and display supplied PIN");
        
        exit(1);
    }

    usingHardware = (argc < 2);
    if(!usingHardware) {
        finalPIN = argv[1];
    }

    /* Initialize hardware */
    if(usingHardware) {
        init_ports();
        reset_hacker_box();    
    }

    /* Display initial user prompt */
    splash_screen();

    /* Scroll display until the box returns a PIN */
    if(usingHardware) {
        wait_for_hacker_box_pin(finalPIN);
    }

    /* Scan to the final PIN string */
    pin_scan(finalPIN);
    
    /* Exit cleanly */
    if(usingHardware) {
        shutdown_hacker_box();
    }
}
