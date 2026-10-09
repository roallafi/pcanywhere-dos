#include "serial.h"

SerialPort::SerialPort(int com_num) {
    com_number = com_num;
    is_open = 0;

    switch (com_num) {
        case 1: base_port = COM1_BASE; break;
        case 2: base_port = COM2_BASE; break;
        case 3: base_port = COM3_BASE; break;
        case 4: base_port = COM4_BASE; break;
        default: base_port = COM1_BASE; break;
    }
}

SerialPort::~SerialPort() {
    close();
}

int SerialPort::init(int baudrate) {
    unsigned char lcr, dll, dlm;
    int divisor;

    if (is_open) close();

    divisor = 115200 / baudrate;
    dll = divisor & 0xFF;
    dlm = (divisor >> 8) & 0xFF;

    outportb(base_port + UART_IER, 0x00);

    lcr = inportb(base_port + UART_LCR);
    outportb(base_port + UART_LCR, lcr | 0x80);

    outportb(base_port + UART_DATA, dll);
    outportb(base_port + UART_IER, dlm);

    lcr = 0x03;
    outportb(base_port + UART_LCR, lcr);
    outportb(base_port + UART_FCR, 0x07);
    outportb(base_port + UART_MCR, 0x03);

    is_open = 1;
    return 0;
}

int SerialPort::send_byte(unsigned char data) {
    int timeout = 10000;

    if (!is_open) return -1;

    while (!is_tx_ready() && timeout--) {
    }

    if (timeout <= 0) return -1;

    outportb(base_port + UART_DATA, data);
    return 0;
}

int SerialPort::send_string(const char* str) {
    if (!is_open) return -1;

    while (*str) {
        if (send_byte(*str++) < 0) return -1;
    }

    return 0;
}

int SerialPort::recv_byte(unsigned char* data) {
    if (!is_open) return -1;
    if (!is_data_ready()) return -1;

    *data = inportb(base_port + UART_DATA);
    return 0;
}

int SerialPort::is_data_ready() {
    if (!is_open) return 0;
    unsigned char lsr = inportb(base_port + UART_LSR);
    return (lsr & LSR_DATA_READY) != 0;
}

int SerialPort::is_tx_ready() {
    if (!is_open) return 0;
    unsigned char lsr = inportb(base_port + UART_LSR);
    return (lsr & LSR_TX_EMPTY) != 0;
}

void SerialPort::close() {
    if (!is_open) return;

    outportb(base_port + UART_MCR, 0x00);
    outportb(base_port + UART_IER, 0x00);
    is_open = 0;
}

int SerialPort::is_initialized() {
    return is_open;
}
