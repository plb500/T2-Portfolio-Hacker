#include "pin_reader.h"
#include <Arduino.h>


// Internal functions ----------------------------------------------
void activate_pin_reader(PinReader *pinReader);
void pin_reader_pin_read_update(PinReader *pinReader);
char digit_to_char(int digit);
// -----------------------------------------------------------------


void init_pin_reader(PinReader *pinReader) {
    if(!pinReader) {
        return;    
    }

    reset_pin_reader(pinReader);
}

void reset_pin_reader(PinReader *pinReader) {
    if(!pinReader) {
        return;    
    }

    pinReader->m_status = PIN_READER_INACTIVE;
    memset(pinReader->m_pinDigits, INVALID_PIN_DIGIT, MAX_PIN_DIGITS);
    pinReader->m_pinLength = -1;
}

void update_pin_reader(PinReader *pinReader) {
    if(!pinReader) {
        return;    
    }

    switch(pinReader->m_status) {
        case PIN_READER_INACTIVE:
            activate_pin_reader(pinReader);
            break;
        case PIN_READER_OBTAINING_PIN:
            pin_reader_pin_read_update(pinReader);
            break;
        case PIN_READER_PIN_OBTAINED:
            // Nothing to do here
            break;
        case PIN_READER_ERROR:
            // Nothing to do here
            break;
    }
 }

void activate_pin_reader(PinReader *pinReader) {
    // TODO: Trigger activation
    // Testing code, take between 3 - 5 seconds to obtain the PIN
    long randInterval = 3000 + (rand() % 2000);
    pinReader->m_pinRetrieveTime = millis() + randInterval;
    // ------------------ end testing code
    
    pinReader->m_status = PIN_READER_OBTAINING_PIN;
}

char digit_to_char(int digit) {
    switch(digit) {
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        default:
            return 'X';
    }    
}


void pin_reader_pin_read_update(PinReader *pinReader) {
    // Testing code, take between 3 - 5 seconds to obtain the PIN
    if(millis() > pinReader->m_pinRetrieveTime) {
        pinReader->m_pinLength = 4;
        for(int i = 0; i < 4; ++i) {
            int digit = rand() % 10;    
            pinReader->m_pinDigits[i] = digit_to_char(digit);
            
        }

        pinReader->m_status = PIN_READER_PIN_OBTAINED;
    }
    // ------------------ end testing code
}
