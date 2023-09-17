#include <stdio.h>
#include <stdlib.h>
#include <bios.h>



#define COM1       0
#define DATA_READY 0x0100

// Serial port settings: 1200/8/None/1
#define SETTINGS ( _COM_1200 | _COM_NOPARITY | _COM_CHR8 | _COM_STOP1 )


#define BUFFER_SIZE         (128)

char readBuffer[BUFFER_SIZE + 1];


int main(void)
{
    int status = 0;
    int done = 0;
    int readByte = 0;
    int c;
    int bufferPos = 0;

    // Open serial port
    bioscom(0, SETTINGS, COM1);

    fprintf(stdout, "COM1 Opened ([ESC] to exit)....\n");
    while(!done) {
        // Get port status
        status = _bios_serialcom(_COM_STATUS, COM1, 0);
        if(status & DATA_READY) {
            // There is data to read
            readByte = _bios_serialcom(_COM_RECEIVE, COM1, 0);
            if(readByte > 0x00FF) {
                fprintf(stdout, "Error reading byte\n");
            } else {
                c = (readByte & 0x00FF);
                if(c == '\x1B') {
                    done = 1;
                } else {
                    readBuffer[bufferPos++] = c;
                }
            }
        } else if(bufferPos) {
            readBuffer[bufferPos] = 0;
            fprintf(stdout, "%s", readBuffer);
            bufferPos = 0;
        }
    }
    
    return 0;
}
