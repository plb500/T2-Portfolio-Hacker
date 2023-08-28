#ifndef __PIN_READER_H__
#define __PIN_READER_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PIN_DIGITS          (12)
#define INVALID_PIN_DIGIT       ('X')

typedef enum {
    PIN_READER_INACTIVE         = 0x0,
    PIN_READER_OBTAINING_PIN    = 0x1,
    PIN_READER_PIN_OBTAINED     = 0x2,
    PIN_READER_ERROR            = 0x3
} PinReaderStatus;

typedef struct {
    PinReaderStatus     m_status;
    char                m_pinDigits[MAX_PIN_DIGITS];
    int                 m_pinLength;
    unsigned long       m_pinRetrieveTime;              // TODO: For testing, remove when implemented
} PinReader;


void init_pin_reader(PinReader *pinReader);
void reset_pin_reader(PinReader *pinReader);
void update_pin_reader(PinReader *pinReader);

#ifdef __cplusplus
} // extern "C"
#endif

#endif      // __PIN_READER_H__
