#ifndef __BLUETOOTH_CARD_READER_H__
#define __BLUETOOTH_CARD_READER_H__

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
//                          | Incoming byte descriptions |
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
enum BTModuleStatus {
    BT_MODULE_NO_PIN                = 0,
    BT_MODULE_READING_PIN           = 1,
    BT_MODULE_HAS_VALID_PIN         = 2,   
    BT_MODULE_PIN_ERROR             = 3
};


class BluetoothCardReader {
    public:
        BluetoothCardReader(int txPin, int rxPin);

        void init();
        void reset();
        void update();
        BTModuleStatus getStatus();
        byte getPINDigit(int digitIndex);
        void triggerPINRead();

        static const int MAX_PIN_LENGTH        = 4;
        static const byte INVALID_PIN_VALUE    = 'X';

    private:    
        void processControlByte(byte c);
        void processPINByte(byte c);
        void processPINBuffer(byte checksum);
        byte getChecksum(byte *bytes, int byteCount);

        static const byte TRIGGER_PIN_READ      = 0x81;
        
        int m_txPin, m_rxPin;
        SoftwareSerial m_serialPort;

        BTModuleStatus m_status;
        byte m_digitBuffer[MAX_PIN_LENGTH];
};


#endif      // __BLUETOOTH_CARD_READER_H__
