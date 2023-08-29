#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <time.h>
#include <string.h>

#include "sleep.h"


#define USER_INT                (0x61)
#define SOUND_GENERATE_FN       (0x15)
#define SOUND_KEY_CLICK         (0)
#define SOUND_BEEP              (1)
#define SOUND_ALARM             (2)

#define MAX_PIN_LENGTH          (38)
#define NUM_PIN_STRINGS         (6)
char *PIN_STRINGS[NUM_PIN_STRINGS] = {
    "12345678901234567890123457890123456780",
    "65989856487897878454548788945445454544",
    "67987895655545456423100026487998545714",
    "65989856487897878454548788945445454544",
    "65989856487897878454548788945445454544",
    "67987895655545456423100026487998545714"
};

// Play key-click sound
void emit_click() {
    union REGS regs;

    regs.h.ah = SOUND_GENERATE_FN;
    regs.h.al = SOUND_KEY_CLICK;
    
    int86(USER_INT, &regs, &regs);
}

// Show the initial user prompt
void splash_screen() {
    clrscr();
    
    fprintf(stdout, "PPPPP  IIIIIII   N    N               \n");
    fprintf(stdout, "P   PP    I      NN   N IDENTIFICATION\n");
    fprintf(stdout, "P   PP    I      N N  N               \n");
    fprintf(stdout, "PPPPP     I      N  N N   PROGRAM     \n");
    fprintf(stdout, "P         I      N   NN               \n");
    fprintf(stdout, "P      IIIIIII   N    N               \n");
    fprintf(stdout, "\n");
    fprintf(stdout, "Strike a key when ready ...");
    
    getch();

    fprintf(stdout, "\n");
}

void click_and_display(const char *displayString) {
        // Play click sound and output string
        emit_click();
        fprintf(stdout, "%s\n", displayString);
        fflush(stdout);
}


// Scan through a series of "random" PIN strings until settling on a final PIN to display
void pin_scan(const char *finalPIN) {
    // For having a semi-consistent refresh rate
    clock_t displayTimePeriod = ((CLOCKS_PER_SEC * 20) / 1000);     // 20ms
    clock_t nextDisplayTime;

    int stringLength = MAX_PIN_LENGTH;      // Current length of the scrolling PIN string
    int scrollIndex = 0;                    // The index into the "random" string array
    int scrollDisplayCount = 0;             // Total number of scrolling lines written
    int finalLen = strlen(finalPIN);
    int finalStringCount = 0;               // Count of how many times we've displayed the
                                            // final PIN string

    // We copy the strings into a fixed-size buffer and right pad the string with spaces
    // until the max display limit. This may seem like an odd way to do it but we need
    // to do this because we want to have a consistent timing to the background key click
    // noise. Longer strings have a noticeably longer time to print on screen than shorter
    // ones, which audibly affects the timing between click sounds. I've tried multiple
    // things to try and get the click delay consistent and this seems to be the only way 
    // I've tried which works
    char displayBuffer[MAX_PIN_LENGTH + 1];
    memset(displayBuffer, 0, (MAX_PIN_LENGTH + 1));

    int done = 0;
    do {
        nextDisplayTime = (clock() + displayTimePeriod);
        
        // Scroll "random" strings while the length > length of the final PIN
        if(stringLength > finalLen) {  
            memcpy(displayBuffer, PIN_STRINGS[scrollIndex], (MAX_PIN_LENGTH + 1));

            scrollIndex++;
            if(scrollIndex >= NUM_PIN_STRINGS) {
                // If we have displayed all of the "random" PIN strings in this batch,
                // shorten them for the next batch by one or two characters ("shorten" in
                // this context just means increase the amount of spaces at the end)
                stringLength -= (scrollDisplayCount++ & 1) ? 1 : 2;

                for(int i = 0; i < NUM_PIN_STRINGS; ++i) {
                    memset(
                        PIN_STRINGS[i] + stringLength,
                        ' ',
                        MAX_PIN_LENGTH - stringLength
                    );
                }
                scrollIndex = 0;
            }
        } else {                    // Final string
            memcpy(displayBuffer, finalPIN, finalLen);
            memset(
                displayBuffer + finalLen,
                ' ',
                MAX_PIN_LENGTH - finalLen
            );
            done = (++finalStringCount > 10);
        }

        click_and_display(displayBuffer);

        if(!done) {
            SLEEP_UNTIL(nextDisplayTime)
        }        
    } while(!done);

    fprintf(stdout, "\nPIN IDENTIFICATION NUMBER: %s\n", finalPIN);
}
