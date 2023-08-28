#include "portfolio_parallel_port.h"
#include <Arduino.h>

void init_parallel_port(PortfolioParallelPort *port) {
    if(!port) {
        return;    
    }

    for(int i = 0; i < 3; ++i) {
        pinMode(port->m_statusPins[i], OUTPUT);
        digitalWrite(port->m_statusPins[i], LOW);
    }

    for(int i = 0; i < 4; ++i) {
        pinMode(port->m_pinValuePins[i], OUTPUT);
        digitalWrite(port->m_pinValuePins[i], LOW);
    }

    pinMode(port->m_pinValueOutputValidPin, OUTPUT);
    digitalWrite(port->m_pinValueOutputValidPin, LOW);

    for(int i = 0; i < 4; ++i) {
        pinMode(port->m_controlPins[i], INPUT_PULLUP);
    }
}

int reader_deactivated(PortfolioParallelPort *port) {
    if(!port) {
        return 0;    
    }
    
    return (digitalRead(port->m_controlPins[CONTROL_READER_ACTIVE_PIN_POSITION]) == LOW) ? 1 : 0;
}

int pin_digit_requested(PortfolioParallelPort *port) {
    if(!port) {
        return INVALID_PIN_DIGIT_POSITION;    
    }

    if(digitalRead(port->m_controlPins[CONTROL_DIGIT_REQUEST_VALID_PIN_POSITION]) == LOW) {
        int highBit = (digitalRead(port->m_controlPins[CONTROL_DIGIT_REQUEST_PIN_POSITION_HIGH]) == HIGH) ? 1 << 1: 0;
        int lowBit = (digitalRead(port->m_controlPins[CONTROL_DIGIT_REQUEST_PIN_POSITION_LOW]) == HIGH) ? 1 : 0;
        return (highBit | lowBit);
    } else {
        return INVALID_PIN_DIGIT_POSITION;
    }
}

void output_pin_digit(PortfolioParallelPort *port, char value) {
    if(!port) {
        return;    
    }

    digitalWrite(port->m_pinValueOutputValidPin, LOW);

    int bit0 = (value & 0x1);
    int bit1 = (value & 0x2) >> 1;
    int bit2 = (value & 0x4) >> 2;
    int bit3 = (value & 0x8) >> 3;

    digitalWrite(port->m_pinValuePins[0], bit0);
    digitalWrite(port->m_pinValuePins[1], bit1);
    digitalWrite(port->m_pinValuePins[2], bit2);
    digitalWrite(port->m_pinValuePins[3], bit3);

    digitalWrite(port->m_pinValueOutputValidPin, HIGH);
}

void output_reader_status(PortfolioParallelPort *port, int statusValue) {
    if(!port) {
        return;    
    }

    digitalWrite(port->m_statusPins[0], (statusValue & 0x1));
    digitalWrite(port->m_statusPins[1], (statusValue & 0x2) >> 1);
    digitalWrite(port->m_statusPins[2], (statusValue & 0x4) >> 2);
}
