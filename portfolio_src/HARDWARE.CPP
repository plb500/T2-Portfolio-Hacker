#include "hardware.h"

#include "sleep.h"
#include <dos.h>


#define PORT_A                  (0x8078)
#define PORT_B                  (0x8079)        // Currently not used
#define PORT_C                  (0x807A)
#define CONTROL_REGISTER        (0x807B)

// Hardware control byte flags
#define BOX_ACTIVE              (1 << 5)
#define NO_PIN_READ             (1 << 4)
#define BOX_CONTROL_IDLE        (BOX_ACTIVE | NO_PIN_READ)

// PORT A is used for input from the hacker box
// PORT B is currently unused so direction doesn't matter
// PORT C is used for outputting control values to the hacker box
// Therefore we can use either modes 9x90 or 0x92 here
#define PORT_CONTROL_VALUE      (0x90)

typedef enum {
    PIN_READER_INACTIVE         = 0x0,
    PIN_READER_OBTAINING_PIN    = 0x1,
    PIN_READER_PIN_OBTAINED     = 0x2,
    PIN_READER_ERROR            = 0x3
} PinReaderStatus;


// Internal functions ----------------------------------------------
char digit_to_char(char digit);
void set_port_modes(int mode);
// -----------------------------------------------------------------


char digit_to_char(char digit) {
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

// Set input/output directions for different 8255 ports (see documentation for values)
void set_port_modes(int mode) {
    outportb(CONTROL_REGISTER, mode);
}

void init_ports() {
    set_port_modes(PORT_CONTROL_VALUE);

    outportb(PORT_C, BOX_CONTROL_IDLE);
}

void reset_hacker_box() {
    outportb(PORT_C, 0);

    SIMPLE_SLEEP(50);
    
    outportb(PORT_C, BOX_CONTROL_IDLE);
}

void shutdown_hacker_box() {
    outportb(PORT_C, 0);

    SIMPLE_SLEEP(50);
}

void request_pin_digit(char digit) {
    if(digit > 3) {
        // Only support 4-digit PINS right now
        return;
    }

    // PORT C details:
    // Pin 10 (Bit 5)       -> Activate box (LOW = box off, HIGH = box on)
    // Pin 11 (Bit 4)       -> PIN digit request valid (LOW valid)
    // Pin 12 (Bit 0)       -> PIN digit position low bit
    // Pin 13 (Bit 1)       -> PIN digit position high bit
    char portOut = BOX_ACTIVE | (digit & 0x3);
    outportb(PORT_C, portOut);
}

char read_pin_digit() {
    char portInput;
    char digitValue = 0xFF;

    char gotInput = 0;
    while(!gotInput) {
        portInput = inportb(PORT_A);
        if(portInput & 0x80) {
            // Digit seems to be ok. Bits 3-6 contain the value
            digitValue = ((portInput & 0x78) >> 3);
            gotInput = 1;
        }
    }
    
    // Reset port lines
    outportb(PORT_C, 0xFF);    
    
    return digit_to_char(digitValue);
}

int hacker_box_ready() {
    int box_input = 0;
    int status = 0;
    
    box_input = inportb(PORT_A);
        
    // First three bits are box status
    status = box_input & 0x7;
    return (status == PIN_READER_PIN_OBTAINED);
}
