#include <conio.h>
#include <stdio.h>
#include <time.h>

#include "PIN_SER.H"
#include "PIN_DISP.H"



void splash_loop(PINSerialReader *reader) {
    do_splash_screen();

    /* Wait for user to hit a key */
    while(!kbhit()) {
        update_serial_reader(reader);
    }
    getch();
    fputs("\n", stdout);
}

void scan_loop(PINSerialReader *reader, PINDisplay *display) {
    int readPIN[MAX_PIN_LENGTH];
    int keepScrolling = 1;
    int i;
    clock_t currentTime;
    
    /* For having a semi-consistent refresh rate */
    clock_t displayTimePeriod = ((CLK_TCK * 20) / 1000);     /* 20ms */
    clock_t nextDisplayTime = 0;

    while(keepScrolling) {
        /* If we haven't stored a PIN yet, update the hardware */
        if(!display_has_final_pin(display)) {
            update_serial_reader(reader);
    
            if(serial_reader_has_pin(reader)) {
                for(i = 0; i < MAX_PIN_LENGTH; ++i) {
                    readPIN[i] = get_serial_reader_pin_digit(reader, i);
                }
                
                set_display_final_pin(display, readPIN);
            }
        }
        
        /* If it's time to refresh the display, do so */
        currentTime = clock();
        if(currentTime >= nextDisplayTime) {
            keepScrolling = !do_pin_scroll(display);
            nextDisplayTime = currentTime + displayTimePeriod;
        }
    }
}

void main() {
    PINSerialReader pinReader;
    PINDisplay pinDisplay;
    
    init_serial_reader(&pinReader);
    init_pin_display(&pinDisplay);
    
    splash_loop(&pinReader);
    scan_loop(&pinReader, &pinDisplay);
    display_final_pin(&pinDisplay);
}
