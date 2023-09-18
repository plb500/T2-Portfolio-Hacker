#ifndef _PIN_SER_H_
#define _PIN_SER_H_

//
//                          +----------------------------+
//                          | Incoming byte descriptions |
//                          +----------------------------+
//
// Sequence control byte (bit 7 HIGH)
// +---------+---------+---------+---------+---------+---------+---------+---------+
// |    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
// +---------+---------+---------+---------+---------+---------+---------+---------+
// |    1    |    0    |    0    |    0    |    0    |    0    |Status: H|Status: L|
// +---------+---------+---------+---------+---------+---------+---------+---------+
//
//      Status: CARD_READER_NEW_PIN_START
//      +---------+---------+---------+---------+---------+---------+---------+---------+
//      |    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
//      +---------+---------+---------+---------+---------+---------+---------+---------+
//      |    1    |    0    |    0    |    0    |    0    |    0    |    0    |    0    |
//      +---------+---------+---------+---------+---------+---------+---------+---------+
//
//      Status: CARD_READER_NEW_PIN_END
//      +---------+---------+---------+---------+---------+---------+---------+---------+
//      |    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
//      +---------+---------+---------+---------+---------+---------+---------+---------+
//      |    1    |    0    |                Checksum               |    0    |    1    |
//      +---------+---------+---------+---------+---------+---------+---------+---------+
//      Checksum is XOR of all PIN digit bytes below (bits 0-3)
//
// PIN digit byte (bit 7 LOW)
// +---------+---------+---------+---------+---------+---------+---------+---------+
// |    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
// +---------+---------+---------+---------+---------+---------+---------+---------+
// |    0    |    0    | Digit H | Digit L | PIN: 3  | PIN: 2  | PIN: 1  | PIN: 0  |
// +---------+---------+---------+---------+---------+---------+---------+---------+
//
//
//
//                          +----------------------------+
//                          | Outgoing byte descriptions |
//                          +----------------------------+
//
// Sequence control bytes (bit 7 HIGH)
//
// Trigger PIN transmit
// +---------+---------+---------+---------+---------+---------+---------+---------+
// |    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
// +---------+---------+---------+---------+---------+---------+---------+---------+
// |    1    |    0    |    0    |    0    |    0    |    0    |    0    |    1    |
// +---------+---------+---------+---------+---------+---------+---------+---------+
//
//


#define MAX_PIN_LENGTH              (4)
#define INVALID_PIN_VALUE           ('X')


// Control byte status
enum CardReaderStatus {
    CARD_READER_NEW_PIN_START       = 0,
    CARD_READER_NEW_PIN_END         = 1
};


class PINSerialReader {
    public:
        PINSerialReader();

        void init();
        void reset();
        void update();
        int hasPIN();
        int getPINDigit(int digitIndex);

    private:    
        void resetPINBuffer();
        void processControlByte(unsigned char c);
        void processPINByte(unsigned char  c);
        void processPINBuffer(unsigned char  checksum);
        unsigned char getChecksum(unsigned char  *bytes, int byteCount);

        unsigned char m_digitBuffer[MAX_PIN_LENGTH];
        unsigned char m_latchedDigits[MAX_PIN_LENGTH];
};

#endif      // _PIN_SER_H_
