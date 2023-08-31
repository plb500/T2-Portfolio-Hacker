#ifndef __PORTFOLIO_PARALLEL_PORT__H
#define __PORTFOLIO_PARALLEL_PORT__H

#include <Array.h>

//
//              HARDWARE CONFIGURATION
//
// +--------------------+---------------------------------------+-----------+-------------------+-------------------+
// | Portfolio Port     | Description                           | Direction | Values            | # Bits required   |
// +--------------------+---------------------------------------+-----------+-------------------+-------------------+
// |                    | Hardware active                       | IN        | 0/1               | 1                 | --+
// | PORT C             | PIN digit position request valid      | IN        | 0/1               | 1                 | . +-- m_controlPins
// | (Portfolio OUT)    | Pin address (digit number)            | IN        | 0-3               | 2                 | --+
// +--------------------+---------------------------------------+-----------+-------------------+-------------------+
// |                    | Pin digit value                       | OUT       | 0-9               | 4                 | - m_pinValuePins
// | PORT A             | Pin digit output valid                | OUT       | 0/1               | 1                 | - m_pinValueOutputValidPin
// | (Portfolio IN)     | Hardware status                       | OUT       | 0x0-0x7           | 3                 | - m_statusPins
// +--------------------+---------------------------------------+-----------+-------------------+-------------------+
//
// Portfolio Port configuration mode: 144 (0x0090)
//


class PortfolioParallelPort {
    public:
        PortfolioParallelPort(
            Array<char, 3> statusPins,
            Array<char, 4> pinValuePins,
            char pinValueOutputValidPin,
            Array<char, 4> controlPins
        );
    
        void init();
        int isHardwareInactive();
        int pinDigitRequested();
        void outputPINDigit(char value);
        void outputReaderStatus(int statusValue);
    
        static const int INVALID_PIN_DIGIT_POSITION                  = 0xFF;

    private:
        static const int CONTROL_READER_ACTIVE_PIN_POSITION          = 0;
        static const int CONTROL_DIGIT_REQUEST_VALID_PIN_POSITION    = 1;
        static const int CONTROL_DIGIT_REQUEST_PIN_POSITION_LOW      = 2;
        static const int CONTROL_DIGIT_REQUEST_PIN_POSITION_HIGH     = 3;

        Array<char, 3> m_statusPins;           // Pins for outputting PIN reader status
        Array<char, 4> m_pinValuePins;      // Pins for outputting PIN digit values
        char m_pinValueOutputValidPin;              // Pin for indicating outputted PIN is valid
        Array<char, 4> m_controlPins;       // Pins for reading control values
};


#endif      // __PORTFOLIO_PARALLEL_PORT__H
