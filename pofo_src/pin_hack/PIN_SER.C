#include "PIN_SER.H"

#include <bios.h>
#include <string.h>


#define COM1       0
#define DATA_READY 0x0100



/* cmd values for _bios_serialcom() */

#define _COM_INIT       0       /* set communication parms to a byte */
#define _COM_SEND       1       /* send a byte to port */
#define _COM_RECEIVE    2       /* read character from port */
#define _COM_STATUS     3       /* get status of port */

/* byte values for _COM_INIT cmd of _bios_serialcom() */

#define _COM_CHR7       0x02    /* 7 data bits */
#define _COM_CHR8       0x03    /* 8 data bits */
#define _COM_STOP1      0x00    /* 1 stop bit */
#define _COM_STOP2      0x04    /* 2 stop bits */
#define _COM_NOPARITY   0x00    /* no parity */
#define _COM_EVENPARITY 0x18    /* even parity */
#define _COM_ODDPARITY  0x08    /* odd parity */
#define _COM_110        0x00    /* 110 baud */
#define _COM_150        0x20    /* 150 baud */
#define _COM_300        0x40    /* 300 baud */
#define _COM_600        0x60    /* 600 baud */
#define _COM_1200       0x80    /* 1200 baud */
#define _COM_2400       0xa0    /* 2400 baud */
#define _COM_4800       0xc0    /* 4800 baud */
#define _COM_9600       0xe0    /* 9600 baud */


/* Serial port settings: 600/8/None/1 */
#define SETTINGS ( _COM_600 | _COM_NOPARITY | _COM_CHR8 | _COM_STOP1 )


/* Forward declarations */
void reset_serial_reader_pin_buffer(PINSerialReader *reader);
void serial_reader_process_control_byte(PINSerialReader *reader, unsigned char c);
void serial_reader_process_pin_byte(PINSerialReader *reader, unsigned char c);
void serial_reader_process_pin_buffer(PINSerialReader *reader, unsigned char checksum);
unsigned char serial_reader_get_checksum(unsigned char *bytes, int byteCount);
/* --------------------------------------------------------------- */


void init_serial_reader(PINSerialReader *reader) {
    if(!reader) {
        return;
    }
    
    /* Open serial port */
    bioscom(_COM_INIT, SETTINGS, COM1);
        
    reset_serial_reader(reader);
}

void reset_serial_reader(PINSerialReader *reader) {
    int i;

    if(!reader) {
        return;
    }
    
    reset_serial_reader_pin_buffer(reader);

    for(i = 0; i < MAX_PIN_LENGTH; ++i) {
        reader->m_latchedDigits[i] = INVALID_PIN_VALUE;
    }
}

void update_serial_reader(PINSerialReader *reader) {
    int readByte, status;
    unsigned char  c;
    int available = 1;

    if(!reader) {
        return;
    }
    
    while(available) {
        /* Get port status */
        
        status = bioscom(_COM_STATUS, 0, COM1);

        if(status & DATA_READY) {
            /* There is data to read */
            readByte = bioscom(_COM_RECEIVE, 0, COM1);
            if(readByte <= 0x00FF) {
                c = (unsigned char)(readByte & 0x00FF);
                
                if(c & 0x80) {
                    /* Control byte */
                    serial_reader_process_control_byte(reader, c);
                } else {
                    /* PIN digit byte */
                    serial_reader_process_pin_byte(reader, c);
                }
            }
        } else {
            available = 0;
        }
    }
}

int serial_reader_has_pin(PINSerialReader *reader) {
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

int get_serial_reader_pin_digit(PINSerialReader *reader, int digitIndex) {
    if(!reader) {
        return 0;
    }
    
    return serial_reader_has_pin(reader) ? reader->m_latchedDigits[digitIndex] : INVALID_PIN_VALUE;
}

void reset_serial_reader_pin_buffer(PINSerialReader *reader) {
    int i;
    for(i = 0; i < MAX_PIN_LENGTH; ++i) {
        reader->m_digitBuffer[i] = INVALID_PIN_VALUE;
    }
}

void serial_reader_process_control_byte(PINSerialReader *reader, unsigned char c) {
    CardReaderStatus status = (CardReaderStatus) (c & 0x03);

    switch(status) {
        case CARD_READER_NEW_PIN_START:
            reset_serial_reader_pin_buffer(reader);
            break;
            
        case CARD_READER_NEW_PIN_END: 
            serial_reader_process_pin_buffer(reader, ((c & 0x3C) >> 2));
            break;
    }
}

void serial_reader_process_pin_byte(PINSerialReader *reader, unsigned char c) {
    int digitPosition = ((c & 0x30) >> 4);
    int digitValue = (c & 0x0F);
    
    reader->m_digitBuffer[digitPosition] = digitValue;
}

void serial_reader_process_pin_buffer(PINSerialReader *reader, unsigned char checksum) {
    int i;
    int ourChecksum;
    int goodPIN = 1;

    /* Check PIN values and store if good */
    for(i = 0; i < MAX_PIN_LENGTH; ++i) {
        if(reader->m_digitBuffer[i] == INVALID_PIN_VALUE) {
            goodPIN = 0;
            break;
        }
    }

    if(goodPIN) {
        /* Validate checksum */
        ourChecksum = serial_reader_get_checksum(reader->m_digitBuffer, MAX_PIN_LENGTH);

        if(ourChecksum == checksum) {
            memcpy(reader->m_latchedDigits, reader->m_digitBuffer, MAX_PIN_LENGTH);
        }
    }
}

unsigned char serial_reader_get_checksum(unsigned char *bytes, int byteCount) {
    int i;
    unsigned char  checksum = 0;

    for(i = 0; i < byteCount; ++i) {
        checksum ^= bytes[i];
    }

    return checksum;
}
