#include "pin_reader.h"
#include "portfolio_parallel_port.h"


PinReader pinReader;
PortfolioParallelPort parallelPort = {
    {2, 3, 4},          // Pins for outputting PIN reader status
    {5, 6, 7, 8},       // Pins for outputting PIN digit values
    9,                  // Pin for indicating outputted PIN is valid
    {10, 11, 12, 13}    // Control pins    
};


void setup() {
    init_pin_reader(&pinReader);
    init_parallel_port(&parallelPort);
    
    // For testing
    Serial.begin(9600);
}

void loop() {
    // If we are inactive don't process anything
    if(reader_deactivated(&parallelPort)) {
        reset_pin_reader(&pinReader);
        return;
    }

    // Otherwise proceed and attempt to obtain pin, respond to requests etc
    update_pin_reader(&pinReader);
    if(pinReader.m_status == PIN_READER_PIN_OBTAINED) {
        Serial.print("Box active. PIN obtained:");
        Serial.print(pinReader.m_pinDigits[0]);
        Serial.print(pinReader.m_pinDigits[1]);
        Serial.print(pinReader.m_pinDigits[2]);
        Serial.println(pinReader.m_pinDigits[3]);
    }

    // Output reader status
    output_reader_status(&parallelPort, pinReader.m_status);

    // Check if a PIN digit has been requested
    int requestedDigit = pin_digit_requested(&parallelPort);
    if(requestedDigit != INVALID_PIN_DIGIT_POSITION) {
        if(
            (pinReader.m_status == PIN_READER_PIN_OBTAINED) && 
            (requestedDigit < pinReader.m_pinLength)
        ) {
            Serial.print("Pin digit ");
            Serial.print(requestedDigit);
            Serial.println(" requested");

            char digitValue = pinReader.m_pinDigits[requestedDigit];
            output_pin_digit(&parallelPort, digitValue);
        } else {
            Serial.println("ERROR - PIN digit was requested but reader was unable to fulfill request");

            Serial.print("Reader status: ");
            Serial.println(pinReader.m_status);
        
            Serial.print("Requested digit: ");
            Serial.println(requestedDigit);

            Serial.print("Current PIN length: ");
            Serial.println(pinReader.m_pinLength);
        }
    }
}
