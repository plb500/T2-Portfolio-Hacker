#include "PIN_DISP.H"
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>


#define USER_INT                    (0x61)
#define SOUND_GENERATE_FN           (0x15)
#define SOUND_KEY_CLICK             (0)
#define SOUND_BEEP                  (1)
#define SOUND_ALARM                 (2)

#define NUM_PIN_SCROLL_STRINGS      (6)
char *PIN_SCROLL_STRINGS[NUM_PIN_SCROLL_STRINGS] = {
    "12345678901234567890123457890123456780",
    "65989856487897878454548788945445454544",
    "67987895655545456423100026487998545714",
    "65989856487897878454548788945445454544",
    "65989856487897878454548788945445454544",
    "67987895655545456423100026487998545714"
};

#define NUM_FULL_WIDTH_STRINGS      (10)
#define NUM_FINAL_PIN_DISPLAY       (10)


#define INT_TO_CHAR(X)              (X + '0')


/* Internal functions */
void emit_click() {
    union REGS regs;

    regs.h.ah = SOUND_GENERATE_FN;
    regs.h.al = SOUND_KEY_CLICK;
    
    int86(USER_INT, &regs, &regs);
}

void click_and_display(char *displayString) {
    /* Play click sound and output string */
    emit_click();
    puts(displayString);
    fflush(stdout);
}

void reset_pin_display(PINDisplay *display) {
    if(!display) {
        return;
    }

    display->m_scrollCounter = 0;
    display->m_currentDisplayWidth = MAX_PIN_DISPLAY_LENGTH;
    display->m_state = PIN_DISPLAY_SCROLL_FULL_WIDTH;
    memset(display->m_displayBuffer, 0, MAX_PIN_DISPLAY_LENGTH + 1);
}
/* -------------------------------------------------------- */


void init_pin_display(PINDisplay *display) {
    if(!display) {
        return;
    }

    display->m_currentPINValid = 0;
    reset_pin_display(display);
}

void do_splash_screen() {
    clrscr();
    
    puts("PPPPP  IIIIIII   N    N               ");
    puts("P   PP    I      NN   N IDENTIFICATION");
    puts("P   PP    I      N N  N               ");
    puts("PPPPP     I      N  N N   PROGRAM     ");
    puts("P         I      N   NN               ");
    puts("P      IIIIIII   N    N               ");
    puts("");
    fputs("Strike a key when ready ...", stdout);
}

int do_pin_scroll(PINDisplay *display) {
    int currentStringIndex = 0;
    int outputLength;
    int padding = 0;
    int numIterations = 0;
    int returnValue = 1;        /* 1 = Finished scrolling, 0 = not finished */

    if(!display) {
        return 1;
    }
    
    switch(display->m_state) {
        case PIN_DISPLAY_SCROLL_FULL_WIDTH:
            currentStringIndex = (display->m_scrollCounter % NUM_PIN_SCROLL_STRINGS);

            memcpy(
                display->m_displayBuffer, 
                PIN_SCROLL_STRINGS[currentStringIndex],
                MAX_PIN_DISPLAY_LENGTH
            );
            
            if(
                (++display->m_scrollCounter >= NUM_FULL_WIDTH_STRINGS) &&
                display_has_final_pin(display)
            ) {
                display->m_scrollCounter = 0;
                display->m_state = PIN_DISPLAY_SCROLL_DECREASING;
            }

            returnValue = 0;
            break;
            
        case PIN_DISPLAY_SCROLL_DECREASING:
            currentStringIndex = (display->m_scrollCounter % NUM_PIN_SCROLL_STRINGS);
            
            padding = (MAX_PIN_DISPLAY_LENGTH - display->m_currentDisplayWidth);
            memcpy(
                display->m_displayBuffer, 
                PIN_SCROLL_STRINGS[currentStringIndex],
                display->m_currentDisplayWidth
            );
            memset(
                (display->m_displayBuffer + display->m_currentDisplayWidth),
                ' ',
                padding
            );
            
            ++display->m_scrollCounter;
            
            /* 
             * Every full pass of the scroll strings we will shorten the overall
             * length we display by either 1 or 2 characters
             */
            if(currentStringIndex == (NUM_PIN_SCROLL_STRINGS - 1)) {
                /* Decrement string length */
                numIterations = (display->m_scrollCounter / NUM_PIN_SCROLL_STRINGS);
                display->m_currentDisplayWidth -= (numIterations & 1) ? 1 : 2;
                
                if(display->m_currentDisplayWidth <= MAX_PIN_LENGTH) {
                    display->m_scrollCounter = 0;
                    display->m_state = PIN_DISPLAY_SCROLL_FINAL_PIN;
                }
            }
                    
            returnValue = 0;
            break;
            
        case PIN_DISPLAY_SCROLL_FINAL_PIN:
            padding = (MAX_PIN_DISPLAY_LENGTH - MAX_PIN_LENGTH);
            memcpy(
                display->m_displayBuffer, 
                display->m_currentPIN,
                MAX_PIN_LENGTH
            );
            memset(
                (display->m_displayBuffer + display->m_currentDisplayWidth),
                ' ',
                padding
            );
            
            ++display->m_scrollCounter;
            
            returnValue = (display->m_scrollCounter >= NUM_FINAL_PIN_DISPLAY);
            break;
    }
    
    click_and_display(display->m_displayBuffer);
    return returnValue;
}

int display_final_pin(PINDisplay *display) {
    fprintf(stdout, "\nPIN IDENTIFICATION NUMBER: %s\n", display->m_currentPIN);
}

int display_has_final_pin(PINDisplay *display) {
    if(!display) {
        return 0;
    }

    return display->m_currentPINValid;
}

void set_display_final_pin(PINDisplay *display, int *pin) {
    int i;
    
    for(i = 0; i < MAX_PIN_LENGTH; ++i) {
        display->m_currentPIN[i] = INT_TO_CHAR(pin[i]);
    }
    display->m_currentPIN[MAX_PIN_LENGTH] = 0;

    display->m_currentPINValid = 1;
    reset_pin_display(display);
}
