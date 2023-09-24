#ifndef _PIN_DISP_H
#define _PIN_DISP_H


#include "PIN_SER.H"


#define MAX_PIN_DISPLAY_LENGTH      (38)


typedef enum {
    PIN_DISPLAY_SCROLL_FULL_WIDTH   = 0,
    PIN_DISPLAY_SCROLL_DECREASING   = 1,
    PIN_DISPLAY_SCROLL_FINAL_PIN    = 2
} PINScrollState;

typedef struct {
    char m_currentPIN[MAX_PIN_LENGTH + 1];
    int m_currentPINValid;

    int m_scrollCounter;
    int m_currentDisplayWidth;
    PINScrollState m_state;
    char m_displayBuffer[MAX_PIN_DISPLAY_LENGTH + 1];
} PINDisplay;


void init_pin_display(PINDisplay *display);
void do_splash_screen();
int do_pin_scroll(PINDisplay *display);
int display_final_pin(PINDisplay *display);
int display_has_final_pin(PINDisplay *display);
void set_display_final_pin(PINDisplay *display, int *pin);


#endif      /* _PIN_DISP_H */
