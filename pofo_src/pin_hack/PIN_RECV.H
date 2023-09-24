#ifndef _PIN_RECV_H_
#define _PIN_RECV_H_

#include "PIN_SER.H"
#include "PIN_PAR.H"
#include "PIN_HC.H"

typedef enum {
    PIN_RECEIVER_SERIAL     = 0,
    PIN_RECEIVER_PARALLEL   = 1,
    PIN_RECEIVER_HARD_CODE  = 2
} PINReceiverType;

typedef union {
    PINSerialReader m_serial;
    PINParallelReader m_parallel;
    PINHardCodedReader m_hardCoded;
} PINReaderHardware;

typedef struct {
    PINReceiverType m_type;
    PINReaderHardware m_hardware;
} PINReceiver;


void init_pin_receiver_serial(PINReceiver *pinReceiver);
void init_pin_receiver_parallel(PINReceiver *pinReceiver);
void init_pin_receiver_hard_coded(PINReceiver *pinReceiver, int *pin);

void reset_pin_receiver(PINReceiver *pinReceiver);
void update_pin_receiver(PINReceiver *pinReceiver);
int pin_receiver_has_pin(PINReceiver *pinReceiver);
int get_pin_receiver_pin_digit(PINReceiver *pinReceiver, int digitIndex);

#endif      /* _PIN_RECV_H_ */
