#ifndef _PIN_PAR_H_
#define _PIN_PAR_H_

#include "PR_DEFS.H"

#include <time.h>


typedef struct {
    unsigned char m_digitBuffer[MAX_PIN_LENGTH];
    unsigned char m_latchedDigits[MAX_PIN_LENGTH];
    int m_currentRequestedDigit;
    clock_t m_currentRequestedDigitReadTime;
} PINParallelReader;


void init_parallel_reader(PINParallelReader *reader);
void reset_parallel_reader(PINParallelReader *reader);
void update_parallel_reader(PINParallelReader *reader);
int parallel_reader_has_pin(PINParallelReader *reader);
int get_parallel_reader_pin_digit(PINParallelReader *reader, int digitIndex);

#endif      /* _PIN_PAR_H_ */
