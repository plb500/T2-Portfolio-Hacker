#include "PIN_SER.H"

#include <bios.h>
#include <iostream.h>
#include <string.h>



#define COM1       0
#define DATA_READY 0x0100

// Serial port settings: 600/8/None/1
#define SETTINGS ( _COM_600 | _COM_NOPARITY | _COM_CHR8 | _COM_STOP1 )


PINSerialReader::PINSerialReader() {}

void PINSerialReader::init() {
    // Open serial port
    bioscom(0, SETTINGS, COM1);
        
    reset();
}

void PINSerialReader::reset() {
    resetPINBuffer();
    
    for(int i = 0; i < MAX_PIN_LENGTH; ++i) {
        m_latchedDigits[i] = INVALID_PIN_VALUE;
    }
}

void PINSerialReader::update() {
    int readByte, status;
    unsigned char  c;
    int available = 1;
    
    while(available) {
        // Get port status
        int status = _bios_serialcom(_COM_STATUS, COM1, 0);

        if(status & DATA_READY) {
            // There is data to read
            readByte = _bios_serialcom(_COM_RECEIVE, COM1, 0);
            if(readByte <= 0x00FF) {
                c = (unsigned char)(readByte & 0x00FF);
                
                if(c & 0x80) {
                    // Control byte
                    processControlByte(c);
                } else {
                    // PIN digit byte    
                    processPINByte(c);
                }
            }
        } else {
            available = 0;
        }
    }
}

int PINSerialReader::hasPIN() {
    for(int i = 0; i < MAX_PIN_LENGTH; ++i) {
        if(m_latchedDigits[i] == INVALID_PIN_VALUE) {
            return 0;
        }
    }

    return 1;
}

int PINSerialReader::getPINDigit(int digitIndex) {
    if(digitIndex >= MAX_PIN_LENGTH) {
        return INVALID_PIN_VALUE;    
    }
    
    return hasPIN() ? m_latchedDigits[digitIndex] : INVALID_PIN_VALUE;
}

void PINSerialReader::resetPINBuffer() {
    for(int i = 0; i < MAX_PIN_LENGTH; ++i) {
        m_digitBuffer[i] = INVALID_PIN_VALUE;
    }
}

void PINSerialReader::processControlByte(unsigned char  c) {
    CardReaderStatus status = (CardReaderStatus) (c & 0x03);

    switch(status) {
        case CARD_READER_NEW_PIN_START:
            resetPINBuffer();
            break;
            
        case CARD_READER_NEW_PIN_END: 
            processPINBuffer((c & 0x3C) >> 2);
            break;
    }
}

void PINSerialReader::processPINByte(unsigned char c) {
    int digitPosition = ((c & 0x30) >> 4);
    int digitValue = (c & 0x0F);
    
    m_digitBuffer[digitPosition] = digitValue;
}

void PINSerialReader::processPINBuffer(unsigned char checksum) {
    int goodPIN = 1;

    // Check PIN values and store if good
    for(int i = 0; i < MAX_PIN_LENGTH; ++i) {
        if(m_digitBuffer[i] == INVALID_PIN_VALUE) {
            cout << "PIN buffer contains invalid value\n";
            goodPIN = 0;
            break;
        }
    }

    if(goodPIN) {
        // Validate checksum
        int ourChecksum = getChecksum(m_digitBuffer, MAX_PIN_LENGTH);

        if(ourChecksum != checksum) {
            cout << "PIN checksum mismatch (";
            cout << "local: " << ourChecksum;
            cout << "supplied: " << checksum << ")" << endl;
        } else {
            memcpy(m_latchedDigits, m_digitBuffer, MAX_PIN_LENGTH);
        }
    }
}

unsigned char PINSerialReader::getChecksum(unsigned char  *bytes, int byteCount) {
    unsigned char  checksum = 0;    

    for(int i = 0; i < byteCount; ++i) {
        checksum ^= bytes[i];
    }

    return checksum;
}
