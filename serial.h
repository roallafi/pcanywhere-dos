#ifndef SERIAL_H
#define SERIAL_H

#include <dos.h>
#include <conio.h>

#define COM1_BASE 0x3F8
#define COM2_BASE 0x2F8
#define COM3_BASE 0x3E8
#define COM4_BASE 0x2E8

#define UART_DATA 0
#define UART_IER  1
#define UART_FCR  2
#define UART_LCR  3
#define UART_MCR  4
#define UART_LSR  5
#define UART_MSR  6
#define UART_SCR  7

#define LSR_DATA_READY 0x01
#define LSR_TX_EMPTY   0x20

class SerialPort {
private:
    int base_port;
    int com_number;
    int is_open;

public:
    SerialPort(int com_num = 1);
    ~SerialPort();

    int init(int baudrate);
    int send_byte(unsigned char data);
    int send_string(const char* str);
    int recv_byte(unsigned char* data);
    int is_data_ready();
    int is_tx_ready();
    void close();
    int is_initialized();
};

#endif
