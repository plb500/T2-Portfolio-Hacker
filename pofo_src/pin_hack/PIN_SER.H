#ifndef _PIN_SER_H_
#define _PIN_SER_H_

/*
 *
 *                         +----------------------------+
 *                         | Incoming byte descriptions |
 *                         +----------------------------+
 * 
 * Sequence control byte (bit 7 HIGH)
 * +---------+---------+---------+---------+---------+---------+---------+---------+
 * |    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
 * +---------+---------+---------+---------+---------+---------+---------+---------+
 * |    1    |    0    |    0    |    0    |    0    |    0    |Status: H|Status: L|
 * +---------+---------+---------+---------+---------+---------+---------+---------+
 * 
 *      Status: CARD_READER_NEW_PIN_START
 *      +---------+---------+---------+---------+---------+---------+---------+---------+
 *      |    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
 *      +---------+---------+---------+---------+---------+---------+---------+---------+
 *      |    1    |    0    |    0    |    0    |    0    |    0    |    0    |    0    |
 *      +---------+---------+---------+---------+---------+---------+---------+---------+
 * 
 *      Status: CARD_READER_NEW_PIN_END
 *      +---------+---------+---------+---------+---------+---------+---------+---------+
 *      |    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
 *      +---------+---------+---------+---------+---------+---------+---------+---------+
 *      |    1    |    0    |                Checksum               |    0    |    1    |
 *      +---------+---------+---------+---------+---------+---------+---------+---------+
 *      Checksum is XOR of all PIN digit bytes below (bits 0-3)
 * 
 * PIN digit byte (bit 7 LOW)
 * +---------+---------+---------+---------+---------+---------+---------+---------+
 * |    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
 * +---------+---------+---------+---------+---------+---------+---------+---------+
 * |    0    |    0    | Digit H | Digit L | PIN: 3  | PIN: 2  | PIN: 1  | PIN: 0  |
 * +---------+---------+---------+---------+---------+---------+---------+---------+
 * 
 * 
 * 
 *                          +----------------------------+
 *                          | Outgoing byte descriptions |
 *                          +----------------------------+
 * 
 * Sequence control bytes (bit 7 HIGH)
 * 
 * Trigger PIN transmit
 * +---------+---------+---------+---------+---------+---------+---------+---------+
 * |    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
 * +---------+---------+---------+---------+---------+---------+---------+---------+
 * |    1    |    0    |    0    |    0    |    0    |    0    |    0    |    1    |
 * +---------+---------+---------+---------+---------+---------+---------+---------+
*/

#include "PR_DEFS.H"

/* Control byte status */
typedef enum {
    CARD_READER_NEW_PIN_START       = 0,
    CARD_READER_NEW_PIN_END         = 1
} CardReaderStatus;


typedef struct {
    unsigned char m_digitBuffer[MAX_PIN_LENGTH];
    unsigned char m_latchedDigits[MAX_PIN_LENGTH];
} PINSerialReader;


void init_serial_reader(PINSerialReader *reader);
void reset_serial_reader(PINSerialReader *reader);
void update_serial_reader(PINSerialReader *reader);
int serial_reader_has_pin(PINSerialReader *reader);
int get_serial_reader_pin_digit(PINSerialReader *reader, int digitIndex);


#endif      /* _PIN_SER_H_ */
