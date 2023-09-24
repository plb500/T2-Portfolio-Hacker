#include "PIN_HC.H"

void init_hard_coded_reader(PINHardCodedReader *reader, int *pin) {
    if(!reader || !pin) {
        return;
    }
    
    memcpy(reader->m_pin, pin, sizeof(int) * MAX_PIN_LENGTH);
}

void reset_hard_coded_reader(PINHardCodedReader *reader) {
    /* Does nothing */
    (void)(reader);
}

void update_hard_coded_reader(PINHardCodedReader *reader) {
    /* Does nothing */
    (void)(reader);
}

int serial_hard_coded_has_pin(PINHardCodedReader *reader) {
    if(!reader) {
        return 0;
    }

    return 1;
}

int get_hard_coded_reader_pin_digit(PINHardCodedReader *reader, int digitIndex) {
    if(!reader) {
        return INVALID_PIN_VALUE;
    }
    
    if((digitIndex < 0) || (digitIndex >= MAX_PIN_LENGTH)) {
        return INVALID_PIN_VALUE;
    }

    return reader->m_pin[digitIndex];
}
