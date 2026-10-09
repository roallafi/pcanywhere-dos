#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <dos.h>

#define KEYBOARD_INT 0x09
#define KB_BUFFER_SIZE 256

class KeyboardCapture {
private:
    static KeyboardCapture* instance;
    unsigned char key_buffer[KB_BUFFER_SIZE];
    int buffer_head;
    int buffer_tail;
    int is_installed;

public:
    KeyboardCapture();
    ~KeyboardCapture();

    void install();
    void uninstall();
    int get_key();
    int key_available();
    int is_active();

    static void interrupt far keyboard_isr(...);
};

#endif
