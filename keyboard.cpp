#include "keyboard.h"

KeyboardCapture* KeyboardCapture::instance = NULL;

void interrupt far KeyboardCapture::keyboard_isr(...) {
    unsigned char scan_code;

    scan_code = inportb(0x60);

    if ((scan_code & 0x80) == 0 && KeyboardCapture::instance != NULL) {
        KeyboardCapture::instance->key_buffer[KeyboardCapture::instance->buffer_head] = scan_code;
        KeyboardCapture::instance->buffer_head++;

        if (KeyboardCapture::instance->buffer_head >= KB_BUFFER_SIZE) {
            KeyboardCapture::instance->buffer_head = 0;
        }
    }

    outportb(0x20, 0x20);
}

KeyboardCapture::KeyboardCapture() {
    instance = this;
    buffer_head = 0;
    buffer_tail = 0;
    is_installed = 0;
    old_handler = NULL;
}

KeyboardCapture::~KeyboardCapture() {
    uninstall();
}

void KeyboardCapture::install() {
    if (is_installed) return;

    old_handler = getvect(KEYBOARD_INT);
    setvect(KEYBOARD_INT, keyboard_isr);
    is_installed = 1;
}

void KeyboardCapture::uninstall() {
    if (!is_installed) return;

    if (old_handler != NULL) {
        setvect(KEYBOARD_INT, old_handler);
    }

    is_installed = 0;
}

int KeyboardCapture::get_key() {
    if (buffer_head == buffer_tail) return -1;

    int key = key_buffer[buffer_tail];
    buffer_tail++;

    if (buffer_tail >= KB_BUFFER_SIZE) {
        buffer_tail = 0;
    }

    return key;
}

int KeyboardCapture::key_available() {
    return buffer_head != buffer_tail;
}

int KeyboardCapture::is_active() {
    return is_installed;
}
