#ifndef _PIN_HC_H_
#define _PIN_HC_H_

#include "PR_DEFS.H"

typedef struct {
    int m_pin[MAX_PIN_LENGTH];
} PINHardCodedReader;


void init_hard_coded_reader(PINHardCodedReader *reader, int *pin);
void reset_hard_coded_reader(PINHardCodedReader *reader);
void update_hard_coded_reader(PINHardCodedReader *reader);
int serial_hard_coded_has_pin(PINHardCodedReader *reader);
int get_hard_coded_reader_pin_digit(PINHardCodedReader *reader, int digitIndex);


#endif      /* _PIN_SER_H_ */
