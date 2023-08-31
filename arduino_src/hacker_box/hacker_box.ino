#include "portfolio_parallel_port.h"
#include "bluetooth_card_reader.h"


BluetoothCardReader btReader(14, 15);

PortfolioParallelPort parallelPort = {
    Array<char, 3>({2, 3, 4}),          // Pins for outputting PIN reader status
    Array<char, 4>({5, 6, 7, 8}),       // Pins for outputting PIN digit values
    9,                                  // Pin for indicating outputted PIN is valid
    Array<char, 4>({10, 11, 12, 13})    // Control pins    
};


void setup() {
    btReader.init();
    parallelPort.init();
    
    // For testing
    Serial.begin(9600);
}

void loop() {
    // If we are inactive don't process anything
    if(parallelPort.isHardwareInactive()) {
        btReader.reset();
        return;
    }

    // Otherwise proceed and attempt to obtain pin, respond to requests etc
    switch(btReader.getStatus()) {
        case BT_MODULE_NO_PIN:
            btReader.triggerPINRead();
            break;
            
        case BT_MODULE_READING_PIN:
            // No action required
            break;
            
        case BT_MODULE_HAS_VALID_PIN:
            // Maybe cache the PIN? Seems redundant since we'd have to reset this on error anyway
            Serial.print("Box active. PIN obtained:");
            Serial.print(btReader.getPINDigit(0), DEC);
            Serial.print(btReader.getPINDigit(1), DEC);
            Serial.print(btReader.getPINDigit(2), DEC);
            Serial.println(btReader.getPINDigit(3), DEC);
            break;
            
        case BT_MODULE_PIN_ERROR:
            // Try re-triggering?
            btReader.triggerPINRead();
            break;
    }
    btReader.update();
    
    // Output reader status
    parallelPort.outputReaderStatus(btReader.getStatus());

    // Check if a PIN digit has been requested
    int requestedDigit = parallelPort.pinDigitRequested();
    if(requestedDigit != PortfolioParallelPort::INVALID_PIN_DIGIT_POSITION) {
        if(
            (btReader.getStatus() == BT_MODULE_HAS_VALID_PIN) && 
            (requestedDigit < BluetoothCardReader::MAX_PIN_LENGTH)
        ) {
            Serial.print("Pin digit ");
            Serial.print(requestedDigit);
            Serial.println(" requested");

            char digitValue = btReader.getPINDigit(requestedDigit);
            parallelPort.outputPINDigit(digitValue);
        } else {
            Serial.println("ERROR - PIN digit was requested but reader was unable to fulfill request");

            Serial.print("Reader status: ");
            Serial.println(btReader.getStatus());
        
            Serial.print("Requested digit: ");
            Serial.println(requestedDigit);

            Serial.print("Current PIN length: ");
            Serial.println(BluetoothCardReader::MAX_PIN_LENGTH);
        }
    }
}
