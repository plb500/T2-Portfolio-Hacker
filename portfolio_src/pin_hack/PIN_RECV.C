#include "PIN_RECV.H"
#include "PR_DEFS.H"


void init_pin_receiver_serial(PINReceiver *pinReceiver) {
    if(!pinReceiver) {
        return;
    }

    pinReceiver->m_type = PIN_RECEIVER_SERIAL;
    init_serial_reader(&pinReceiver->m_hardware.m_serial);
}

void init_pin_receiver_parallel(PINReceiver *pinReceiver) {
    if(!pinReceiver) {
        return;
    }

    pinReceiver->m_type = PIN_RECEIVER_PARALLEL;
    init_parallel_reader(&pinReceiver->m_hardware.m_parallel);
}

void init_pin_receiver_hard_coded(PINReceiver *pinReceiver, int *pin) {
    if(!pinReceiver) {
        return;
    }
    
    pinReceiver->m_type = PIN_RECEIVER_HARD_CODE;
    init_hard_coded_reader(&pinReceiver->m_hardware.m_hardCoded, pin);
}

void reset_pin_receiver(PINReceiver *pinReceiver) {
    if(!pinReceiver) {
        return;
    }
    
    switch(pinReceiver->m_type) {
        case PIN_RECEIVER_SERIAL:
            reset_serial_reader(&pinReceiver->m_hardware.m_serial);
            break;
            
        case PIN_RECEIVER_PARALLEL:
            reset_parallel_reader(&pinReceiver->m_hardware.m_parallel);
            break;
            
        case PIN_RECEIVER_HARD_CODE:
            reset_hard_coded_reader(&pinReceiver->m_hardware.m_hardCoded);
            break;
            
        default:
            break;
    }
}

void update_pin_receiver(PINReceiver *pinReceiver) {
    if(!pinReceiver) {
        return;
    }
    
    switch(pinReceiver->m_type) {
        case PIN_RECEIVER_SERIAL:
            update_serial_reader(&pinReceiver->m_hardware.m_serial);
            break;
            
        case PIN_RECEIVER_PARALLEL:
            update_parallel_reader(&pinReceiver->m_hardware.m_parallel);
            break;
            
        case PIN_RECEIVER_HARD_CODE:
            update_hard_coded_reader(&pinReceiver->m_hardware.m_hardCoded);
            break;
            
        default:
            break;
    }
}

int pin_receiver_has_pin(PINReceiver *pinReceiver) {
    if(!pinReceiver) {
        return 0;
    }
    
    switch(pinReceiver->m_type) {
        case PIN_RECEIVER_SERIAL:
            return serial_reader_has_pin(&pinReceiver->m_hardware.m_serial);
            
        case PIN_RECEIVER_PARALLEL:
            return parallel_reader_has_pin(&pinReceiver->m_hardware.m_parallel);
            
        case PIN_RECEIVER_HARD_CODE:
            return serial_hard_coded_has_pin(&pinReceiver->m_hardware.m_hardCoded);
            
        default:
            return 0;
    }
}

int get_pin_receiver_pin_digit(PINReceiver *pinReceiver, int digitIndex) {
    if(!pinReceiver) {
        return INVALID_PIN_VALUE;
    }
    
    switch(pinReceiver->m_type) {
        case PIN_RECEIVER_SERIAL:
            return get_serial_reader_pin_digit(&pinReceiver->m_hardware.m_serial, digitIndex);
            
        case PIN_RECEIVER_PARALLEL:
            return get_parallel_reader_pin_digit(&pinReceiver->m_hardware.m_parallel, digitIndex);
            
        case PIN_RECEIVER_HARD_CODE:
            return get_hard_coded_reader_pin_digit(&pinReceiver->m_hardware.m_hardCoded, digitIndex);
            
        default:
            return INVALID_PIN_VALUE;
    }
}
