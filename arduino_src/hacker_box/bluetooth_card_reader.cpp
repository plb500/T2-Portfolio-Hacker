#include "bluetooth_card_reader.h"
#include <Arduino.h>

BluetoothCardReader::BluetoothCardReader(int txPin, int rxPin) :
    m_txPin(txPin),
    m_rxPin(rxPin),
    m_serialPort(rxPin, txPin),
    m_status(BT_MODULE_NO_PIN) {}

void BluetoothCardReader::init() {
    pinMode(m_rxPin, INPUT);
    pinMode(m_txPin, OUTPUT);

    m_serialPort.begin(9600);
        
    reset();
}

void BluetoothCardReader::reset() {
    resetPINBuffer();
    
    for(int i = 0; i < MAX_PIN_LENGTH; ++i) {
        m_latchedDigits[i] = INVALID_PIN_VALUE;
    }

    m_status = BT_MODULE_NO_PIN;
}

void BluetoothCardReader::update() {
    while(m_serialPort.available()) {
        byte c = (byte) (m_serialPort.read());

        if(c & 0x80) {
            // Control byte
            processControlByte(c);
        } else {
            // PIN digit byte    
            processPINByte(c);
        }
    }
}

BTModuleStatus BluetoothCardReader::getStatus() {
    return m_status;
}

bool BluetoothCardReader::hasPIN() {
    for(int i = 0; i < MAX_PIN_LENGTH; ++i) {
        if(m_latchedDigits[i] == INVALID_PIN_VALUE) {
            return false;
        }
    }

    return true;
}

byte BluetoothCardReader::getPINDigit(int digitIndex) {
    if(digitIndex >= MAX_PIN_LENGTH) {
        return INVALID_PIN_VALUE;    
    }
    
    if(m_status != BT_MODULE_HAS_VALID_PIN) {
        return INVALID_PIN_VALUE;    
    }

    return m_latchedDigits[digitIndex];
}

void BluetoothCardReader::resetPINBuffer() {
    for(int i = 0; i < MAX_PIN_LENGTH; ++i) {
        m_digitBuffer[i] = INVALID_PIN_VALUE;
    }
}

void BluetoothCardReader::processControlByte(byte c) {
    CardReaderStatus status = (CardReaderStatus) (c & 0x03);

    switch(status) {
        case CARD_READER_NEW_PIN_START:
            Serial.println("Starting new PIN");
            resetPINBuffer();
            break;
            
        case CARD_READER_NEW_PIN_END: 
            Serial.println("Ending PIN");
            processPINBuffer((c & 0x3C) >> 2);
            break;
    }
}

void BluetoothCardReader::processPINByte(byte c) {
    int digitPosition = ((c & 0x30) >> 4);
    byte digitValue = (c & 0x0F);

    m_digitBuffer[digitPosition] = digitValue;
}

void BluetoothCardReader::processPINBuffer(byte checksum) {
    // Check PIN values and store if good
    for(int i = 0; i < MAX_PIN_LENGTH; ++i) {
        if(m_digitBuffer[i] == INVALID_PIN_VALUE) {
            Serial.println("PIN buffer contains invalid value");
            m_status = BT_MODULE_PIN_ERROR;
            return;
        }
    }

    // Validate checksum
    byte ourChecksum = getChecksum(m_digitBuffer, MAX_PIN_LENGTH);
    if(ourChecksum != checksum) {
        Serial.println("Supplied checksum does not match");
        Serial.print("Our checksum: 0x");
        Serial.println(ourChecksum, HEX);
        Serial.print("Supplied checksum: 0x");
        Serial.println(checksum, HEX);
        m_status = BT_MODULE_PIN_ERROR;
        return;
    }

    m_status = BT_MODULE_HAS_VALID_PIN;
    memcpy(m_latchedDigits, m_digitBuffer, MAX_PIN_LENGTH);
}

byte BluetoothCardReader::getChecksum(byte *bytes, int byteCount) {
    byte checksum = 0;    

    for(int i = 0; i < byteCount; ++i) {
        checksum ^= bytes[i];
    }

    return checksum;
}
