#include "portfolio_parallel_port.h"
#include <Arduino.h>

PortfolioParallelPort::PortfolioParallelPort(
    Array<char, 3> statusPins,
    Array<char, 4> pinValuePins,
    char pinValueOutputValidPin,
    Array<char, 4> controlPins       
) :
    m_statusPins(statusPins),
    m_pinValuePins(pinValuePins),
    m_pinValueOutputValidPin(pinValueOutputValidPin),
    m_controlPins(controlPins) {}

void PortfolioParallelPort::init() {
    for(int i = 0; i < 3; ++i) {
        pinMode(m_statusPins[i], OUTPUT);
        digitalWrite(m_statusPins[i], LOW);
    }

    for(int i = 0; i < 4; ++i) {
        pinMode(m_pinValuePins[i], OUTPUT);
        digitalWrite(m_pinValuePins[i], LOW);
    }

    pinMode(m_pinValueOutputValidPin, OUTPUT);
    digitalWrite(m_pinValueOutputValidPin, LOW);

    for(int i = 0; i < 4; ++i) {
        pinMode(m_controlPins[i], INPUT_PULLUP);
    }
}

int PortfolioParallelPort::isHardwareInactive() {
    return (digitalRead(m_controlPins[CONTROL_READER_ACTIVE_PIN_POSITION]) == LOW) ? 1 : 0;
}

int PortfolioParallelPort::pinDigitRequested() {
    if(digitalRead(m_controlPins[CONTROL_DIGIT_REQUEST_VALID_PIN_POSITION]) == LOW) {
        int highBit = (digitalRead(m_controlPins[CONTROL_DIGIT_REQUEST_PIN_POSITION_HIGH]) == HIGH) ? 1 << 1: 0;
        int lowBit = (digitalRead(m_controlPins[CONTROL_DIGIT_REQUEST_PIN_POSITION_LOW]) == HIGH) ? 1 : 0;
        return (highBit | lowBit);
    } else {
        return INVALID_PIN_DIGIT_POSITION;
    }
}

void PortfolioParallelPort::outputPINDigit(char value) {
    digitalWrite(m_pinValueOutputValidPin, LOW);

    int bit0 = (value & 0x1);
    int bit1 = (value & 0x2) >> 1;
    int bit2 = (value & 0x4) >> 2;
    int bit3 = (value & 0x8) >> 3;

    digitalWrite(m_pinValuePins[0], bit0);
    digitalWrite(m_pinValuePins[1], bit1);
    digitalWrite(m_pinValuePins[2], bit2);
    digitalWrite(m_pinValuePins[3], bit3);

    digitalWrite(m_pinValueOutputValidPin, HIGH);
}

void PortfolioParallelPort::outputReaderStatus(int statusValue) {
    digitalWrite(m_statusPins[0], (statusValue & 0x1));
    digitalWrite(m_statusPins[1], (statusValue & 0x2) >> 1);
    digitalWrite(m_statusPins[2], (statusValue & 0x4) >> 2);
}
