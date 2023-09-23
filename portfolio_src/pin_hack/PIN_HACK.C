#include <conio.h>
#include <stdio.h>
#include <time.h>

#include "PIN_RECV.H"
#include "PIN_DISP.H"


void splash_loop(PINReceiver *pinReceiver) {
    do_splash_screen();

    /* Wait for user to hit a key */
    while(!kbhit()) {
        update_pin_receiver(pinReceiver);
    }
    getch();
    puts("");
}

void scan_loop(PINReceiver *pinReceiver, PINDisplay *display) {
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
            update_pin_receiver(pinReceiver);
    
            if(pin_receiver_has_pin(pinReceiver)) {
                for(i = 0; i < MAX_PIN_LENGTH; ++i) {
                    readPIN[i] = get_pin_receiver_pin_digit(pinReceiver, i);
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

int is_number(char *str) {
    int strLength = 0;
    int i;
    

    if(!str) {
        return 0;
    }

    strLength = strlen(str);
    for(i = 0; i < strLength; ++i) {
        if((str[i] < '0') || (str[i] > '9')) {
            return 0;
        }
    }
    
    return 1;
}

void error_exit() {
    puts("Usage: ");
    puts("");
    puts("PIN_HACK <\"S\" | \"P\" | [PIN code to display]");
    puts("");
    puts("S: Serial interface hardware");
    puts("P: Parallel interface hardware");
    puts("");

    exit(1);
}

void main(int argc, char **argv) {
    PINReceiver pinReceiver;
    PINDisplay pinDisplay;
    int pin[MAX_PIN_LENGTH];
    int i;

    /* Handle arguments */    
    if(argc > 2) {
        puts("Invalid argument");
        error_exit();
    }
    
    if(strcmp(argv[1], "S") == 0) {
        /* Serial interface mode */
        init_pin_receiver_serial(&pinReceiver);
    } else if(strcmp(argv[1], "P") == 0) {
        /* Parallel interface mode */
        init_pin_receiver_parallel(&pinReceiver);
    } else {
        /* Direct PIN mode - validate PIN */
        if(!is_number(argv[1])) {
            puts("Argument error");
            error_exit();
        }
        
        if(strlen(argv[1]) != MAX_PIN_LENGTH) {
            puts("Supplied PIN is incorrect length (must be 4 digits)");
            error_exit();
        }
        
        /* PIN looks ok, initialize */
        for(i = 0; i < MAX_PIN_LENGTH; ++i) {
            pin[i] = CHAR_TO_INT(argv[1][i]);        
        }
    
        init_pin_receiver_hard_coded(&pinReceiver, pin);
    }   

    init_pin_display(&pinDisplay);
    
    splash_loop(&pinReceiver);
    scan_loop(&pinReceiver, &pinDisplay);
    display_final_pin(&pinDisplay);
}
