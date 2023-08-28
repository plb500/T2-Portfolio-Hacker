#ifndef __PORTFOLIO_PARALLEL_PORT__H
#define __PORTFOLIO_PARALLEL_PORT__H

#ifdef __cplusplus
extern "C" {
#endif

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


#define CONTROL_READER_ACTIVE_PIN_POSITION          (0)
#define CONTROL_DIGIT_REQUEST_VALID_PIN_POSITION    (1)
#define CONTROL_DIGIT_REQUEST_PIN_POSITION_LOW      (2)
#define CONTROL_DIGIT_REQUEST_PIN_POSITION_HIGH     (3)
#define INVALID_PIN_DIGIT_POSITION                  (0xFF)


typedef struct {
    char m_statusPins[3];           // Pins for outputting PIN reader status
    char m_pinValuePins[4];         // Pins for outputting PIN digit values
    char m_pinValueOutputValidPin;  // Pin for indicating outputted PIN is valid
    char m_controlPins[4];          // Pins for reading control values
} PortfolioParallelPort;

void init_parallel_port(PortfolioParallelPort *port);
int reader_deactivated(PortfolioParallelPort *port);
int pin_digit_requested(PortfolioParallelPort *port);
void output_pin_digit(PortfolioParallelPort *port, char value);
void output_reader_status(PortfolioParallelPort *port, int statusValue);

#ifdef __cplusplus
} // extern "C"
#endif

#endif      // __PORTFOLIO_PARALLEL_PORT__H
