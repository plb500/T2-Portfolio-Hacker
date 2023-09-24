#ifndef __SERIAL_CARD_READER_H__
#define __SERIAL_CARD_READER_H__

#include <Arduino.h>
#include <SoftwareSerial.h>


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

// Control byte status
enum CardReaderStatus {
    CARD_READER_NEW_PIN_START       = 0,        
    CARD_READER_NEW_PIN_END         = 1
};

// Module status
enum SerialReaderStatus {
    SERIAL_READER_NO_PIN                = 0,
    SERIAL_READER_READING_PIN           = 1,
    SERIAL_READER_HAS_VALID_PIN         = 2,
    SERIAL_READER_PIN_ERROR             = 3
};


class SerialCardReader {
    public:
        SerialCardReader(int txPin, int rxPin);

        void init();
        void reset();
        void update();
        SerialReaderStatus getStatus();
        bool hasPIN();
        byte getPINDigit(int digitIndex);

        static const int MAX_PIN_LENGTH        = 4;
        static const byte INVALID_PIN_VALUE    = 'X';

    private:    
        void resetPINBuffer();
        void processControlByte(byte c);
        void processPINByte(byte c);
        void processPINBuffer(byte checksum);
        byte getChecksum(byte *bytes, int byteCount);

        int m_txPin, m_rxPin;
        SoftwareSerial m_serialPort;

        SerialReaderStatus m_status;
        byte m_digitBuffer[MAX_PIN_LENGTH];
        byte m_latchedDigits[MAX_PIN_LENGTH];
};


#endif      // __SERIAL_CARD_READER_H__
