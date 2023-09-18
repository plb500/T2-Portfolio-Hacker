#include <iostream.h>
#include "PIN_SER.H"


int buffersMatch(unsigned char *a, unsigned char *b, int len) {
    int i = 0;
    for(i = 0; i < len; ++i) {
        if(a[i] != b[i]) {
            return 0;
        }
    }

    return 1;
}


void main() {
    PINSerialReader pinReader;
    unsigned char oldPIN[MAX_PIN_LENGTH];
    unsigned char newPIN[MAX_PIN_LENGTH];
    
    for(int i = 0; i < MAX_PIN_LENGTH; ++i) {    
        oldPIN[i] = INVALID_PIN_VALUE;
        newPIN[i] = INVALID_PIN_VALUE;
    }
    
    
    pinReader.init();

    while(1) {
        pinReader.update();
        if(pinReader.hasPIN()) {
            newPIN[0] = pinReader.getPINDigit(0);
            newPIN[1] = pinReader.getPINDigit(1);
            newPIN[2] = pinReader.getPINDigit(2);
            newPIN[3] = pinReader.getPINDigit(3);
        
            if(!buffersMatch(newPIN, oldPIN, MAX_PIN_LENGTH)) {
                cout << "New PIN obtained: ";
                cout << (int) newPIN[0] << (int) newPIN[1]; 
                cout << (int) newPIN[2] << (int) newPIN[3] << endl;
            
                memcpy(oldPIN, newPIN, MAX_PIN_LENGTH);
            }
        }
    }
}
