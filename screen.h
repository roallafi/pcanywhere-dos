#ifndef SCREEN_H
#define SCREEN_H

#include <dos.h>
#include <string.h>
#include "serial.h"
#include "protocol.h"

#define SCREEN_SIZE 4000
#define SCREEN_SEG  0xB800

class ScreenCapture {
private:
    unsigned char buffer[SCREEN_SIZE];
    unsigned char last_buffer[SCREEN_SIZE];
    int active;

public:
    ScreenCapture();
    ~ScreenCapture();

    int capture_screen();
    int send_screen(SerialPort* port);
    int receive_screen(SerialPort* port);
    int display_screen();
    int has_changed();
};

#endif
