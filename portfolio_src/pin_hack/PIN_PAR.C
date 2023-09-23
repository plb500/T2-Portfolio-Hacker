#include "PIN_PAR.H"

#include <dos.h>


#define PORT_A                  (0x8078)
#define PORT_B                  (0x8079)        /* Currently not used */
#define PORT_C                  (0x807A)
#define CONTROL_REGISTER        (0x807B)

/* Hardware control byte flags */
#define BOX_ACTIVE              (1 << 5)
#define NO_PIN_READ             (1 << 4)
#define BOX_CONTROL_IDLE        (BOX_ACTIVE | NO_PIN_READ)

/*
 * PORT A is used for input from the hacker box
 * PORT B is currently unused so direction doesn't matter
 * PORT C is used for outputting control values to the hacker box
 * Therefore we can use either modes 9x90 or 0x92 here
  */
#define PORT_CONTROL_VALUE      (0x90)

#define PIN_READ_TIME_PERIOD    ((CLK_TCK * 60) / 1000)


typedef enum {
    PIN_READER_INACTIVE         = 0x0,
    PIN_READER_OBTAINING_PIN    = 0x1,
    PIN_READER_PIN_OBTAINED     = 0x2,
    PIN_READER_ERROR            = 0x3
} PinReaderStatus;


/* Internal functions */
void init_ports();                      /* Initialize the various Parallel Interface (8255) Ports */
void reset_hacker_box();                /* Reset the connected hardware */
void shutdown_hacker_box();             /* Close down the connected hardware */
void request_pin_digit(char digit);     /* Trigger the connected hardware to write the digit at the supplied digit index */
int read_pin_digit();                   /* Read the digit returned by the connected hardware */
int hacker_box_ready();                 /* Returns whether the connected hardware is ready (has a PIN) or not */
void set_port_modes(int mode);          /* Set input/output directions for different 8255 ports (see documentation for values) */
void reset_parallel_reader_pin_buffer(PINParallelReader *reader);


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
    char portOut;
    if(digit > 3) {
        /* Only support 4-digit PINS right now */
        return;
    }

    /*
     * PORT C details:
     * Pin 10 (Bit 5)       -> Activate box (LOW = box off, HIGH = box on)
     * Pin 11 (Bit 4)       -> PIN digit request valid (LOW valid)
     * Pin 12 (Bit 0)       -> PIN digit position low bit
     * Pin 13 (Bit 1)       -> PIN digit position high bit
     */
    portOut = BOX_ACTIVE | (digit & 0x3);
    outportb(PORT_C, portOut);
}

int read_pin_digit() {
    char portInput;
    char digitValue = 0xFF;

    char gotInput = 0;
    while(!gotInput) {
        portInput = inportb(PORT_A);
        if(portInput & 0x80) {
            /* Digit seems to be ok. Bits 3-6 contain the value */
            digitValue = ((portInput & 0x78) >> 3);
            gotInput = 1;
        }
    }
    
    /* Reset port lines */
    outportb(PORT_C, 0xFF);    
    
    return digitValue;
}

int hacker_box_ready() {
    int box_input = 0;
    int status = 0;
    
    box_input = inportb(PORT_A);
        
    /* First three bits are box status */
    status = box_input & 0x7;
    return (status == PIN_READER_PIN_OBTAINED);
}

void set_port_modes(int mode) {
    outportb(CONTROL_REGISTER, mode);
}

void reset_parallel_reader_pin_buffer(PINParallelReader *reader) {
    int i;
    for(i = 0; i < MAX_PIN_LENGTH; ++i) {
        reader->m_digitBuffer[i] = INVALID_PIN_VALUE;
    }
}
/* ------------------------------------- */


void init_parallel_reader(PINParallelReader *reader) {
    if(!reader) {
        return;
    }
    
    init_ports();
    
    reset_parallel_reader(reader);
}

void reset_parallel_reader(PINParallelReader *reader) {
    int i;
    if(!reader) {
        return;
    }

    reset_hacker_box();
    reset_serial_reader_pin_buffer(reader);

    for(i = 0; i < MAX_PIN_LENGTH; ++i) {
        reader->m_latchedDigits[i] = INVALID_PIN_VALUE;
    }
    reader->m_currentRequestedDigit = -1;
}

void update_parallel_reader(PINParallelReader *reader) {
    int i = 0;
    int fullPIN = 1;
    if(!reader) {
        return;
    }
    
    if(!hacker_box_ready()) {
        return;
    }

    if(reader->m_currentRequestedDigit == -1) {
        /* We are not awaiting a digit, request the next digit */
        for(i = 0; i < MAX_PIN_LENGTH; ++i) {
            if(reader->m_digitBuffer[i] == INVALID_PIN_VALUE) {
                request_pin_digit(i);
                reader->m_currentRequestedDigit = i;
                reader->m_currentRequestedDigitReadTime = (clock() + PIN_READ_TIME_PERIOD);
                return;
            }
        }
    } else {
        /* We are waiting for the digit response */
        if(clock() > reader->m_currentRequestedDigitReadTime) {
            /* Time to read the PIN digit */
            reader->m_digitBuffer[reader->m_currentRequestedDigit] = read_pin_digit();
            reader->m_currentRequestedDigit = -1;
            
            /* Check for all digits */
            for(i = 0; i < MAX_PIN_LENGTH; ++i) {
                if(reader->m_digitBuffer[i] == INVALID_PIN_VALUE) {
                    fullPIN = 0;
                    break;
                }
            }
            if(fullPIN) {
                memcpy(reader->m_latchedDigits, reader->m_digitBuffer, sizeof(int) * MAX_PIN_LENGTH);
                reset_parallel_reader_pin_buffer(reader);
            }
        }
    }
}

int parallel_reader_has_pin(PINParallelReader *reader) {
    int i;
    
    if(!reader) {
        return 0;
    }
    
    for(i = 0; i < MAX_PIN_LENGTH; ++i) {
        if(reader->m_latchedDigits[i] == INVALID_PIN_VALUE) {
            return 0;
        }
    }

    return 1;
}

int get_parallel_reader_pin_digit(PINParallelReader *reader, int digitIndex) {
    if(!reader) {
        return INVALID_PIN_VALUE;
    }
    
    return parallel_reader_has_pin(reader) ? reader->m_latchedDigits[digitIndex] : INVALID_PIN_VALUE;
}
