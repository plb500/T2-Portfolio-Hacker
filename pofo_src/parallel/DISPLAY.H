#ifndef __DISPLAY_H__
#define __DISPLAY_H__


#define NUM_PIN_STRINGS         (6)

// Display the initial user prompt
void splash_screen();

// Output the scrolling digits animation, ending with the supplied PIN
void pin_scan(const char *finalPIN);

// Makes the key-click sound and displays the supplied string, followed by a newline
void click_and_display(const char *displayString);

#endif      // __DISPLAY_H__
