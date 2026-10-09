#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

#include "serial.h"
#include "protocol.h"
#include "keyboard.h"
#include "screen.h"
#include "display.h"

#define DEFAULT_COM 1
#define DEFAULT_BAUD 9600

SerialPort* g_serial = NULL;
KeyboardCapture* g_keyboard = NULL;
ScreenCapture* g_screen = NULL;
Display* g_display = NULL;
int g_running = 1;
int g_connected = 0;

static void show_main_menu() {
    clrscr();
    printf("\n");
    printf("==================================================\n");
    printf("PCAnywhere v4 DOS Clone\n");
    printf("==================================================\n\n");
    printf("1. Client Mode\n");
    printf("2. Server Mode\n");
    printf("3. Configure COM Port\n");
    printf("4. Test Connection\n");
    printf("5. About\n");
    printf("6. Exit\n\n");
    printf("Current: COM%d @ %d baud\n", DEFAULT_COM, DEFAULT_BAUD);
    printf("Select option: ");
}

static void configure_serial_port() {
    int comnum, baudopt;
    int baudrate = 9600;

    clrscr();
    printf("\nSelect COM port (1-4): ");
    scanf("%d", &comnum);

    printf("\nSelect baud rate:\n");
    printf("1) 9600\n2) 19200\n3) 38400\n4) 57600\n5) 115200\n");
    baudopt = getch() - '0';

    switch (baudopt) {
        case 1: baudrate = 9600; break;
        case 2: baudrate = 19200; break;
        case 3: baudrate = 38400; break;
        case 4: baudrate = 57600; break;
        case 5: baudrate = 115200; break;
        default: baudrate = 9600; break;
    }

    if (g_serial != NULL) delete g_serial;
    g_serial = new SerialPort(comnum);

    if (g_serial->init(baudrate) < 0) {
        printf("\nFailed to initialize serial port.\n");
        getch();
        return;
    }

    printf("\nSerial port initialized successfully.\n");
    printf("COM%d at %d baud\n", comnum, baudrate);
    getch();
}

static void test_connection() {
    Packet pkt, resp;
    unsigned char ping_data[] = "PING";

    clrscr();
    printf("Testing serial connection...\n");

    if (!g_serial || !g_serial->is_initialized()) {
        printf("Serial port is not configured.\n");
        getch();
        return;
    }

    Protocol::create_packet(&pkt, MSG_PING, 0, ping_data, 4);

    if (Protocol::send_packet(g_serial, &pkt) < 0) {
        printf("Failed to send PING.\n");
        getch();
        return;
    }

    printf("Waiting for reply...\n");

    while (1) {
        if (Protocol::recv_packet(g_serial, &resp) == 0) {
            if (resp.type == MSG_PING) {
                printf("Reply received: connection is working.\n");
                getch();
                return;
            }
        }

        if (kbhit()) {
            getch();
            return;
        }
    }
}

static void client_mode() {
    unsigned char buffer[256];
    Packet pkt, reply;
    unsigned char key;

    clrscr();
    printf("Client mode selected.\n");

    if (!g_serial || !g_serial->is_initialized()) {
        printf("Configure COM port first.\n");
        getch();
        return;
    }

    strcpy((char*)buffer, "PCANYWHERE_CLIENT");
    Protocol::create_packet(&pkt, MSG_HANDSHAKE, 0, buffer, strlen((char*)buffer));

    if (Protocol::send_packet(g_serial, &pkt) < 0) {
        printf("Failed to send handshake.\n");
        getch();
        return;
    }

    printf("Waiting for server response...\n");

    if (Protocol::recv_packet(g_serial, &reply) == 0) {
        if (reply.type == MSG_ACK) {
            printf("Connection established.\n");
            printf("Press ESC to disconnect.\n");
            g_connected = 1;

            g_keyboard->install();

            while (g_connected) {
                if (g_keyboard->key_available()) {
                    key = (unsigned char)g_keyboard->get_key();

                    if (key == 27) {
                        Protocol::create_packet(&pkt, MSG_DISCONNECT, 0, NULL, 0);
                        Protocol::send_packet(g_serial, &pkt);
                        g_connected = 0;
                        break;
                    }

                    Protocol::create_packet(&pkt, MSG_KEYBOARD, 0, &key, 1);
                    Protocol::send_packet(g_serial, &pkt);
                }

                if (Protocol::recv_packet(g_serial, &reply) == 0) {
                    if (reply.type == MSG_SCREEN) {
                        // Simple screen update placeholder
                    }
                }

                delay(50);
            }

            g_keyboard->uninstall();
            printf("Disconnected.\n");
            getch();
        }
    } else {
        printf("No response from server.\n");
        getch();
    }
}

static void server_mode() {
    Packet pkt, req;
    unsigned char buffer[256];
    unsigned char key;

    clrscr();
    printf("Server mode selected.\n");

    if (!g_serial || !g_serial->is_initialized()) {
        printf("Configure COM port first.\n");
        getch();
        return;
    }

    printf("Waiting for client connection...\n");

    while (1) {
        if (Protocol::recv_packet(g_serial, &req) == 0) {
            if (req.type == MSG_HANDSHAKE) {
                printf("Client connected.\n");

                strcpy((char*)buffer, "PCANYWHERE_SERVER_OK");
                Protocol::create_packet(&pkt, MSG_ACK, 0, buffer, strlen((char*)buffer));
                Protocol::send_packet(g_serial, &pkt);

                printf("Press ESC to stop server.\n");
                g_connected = 1;
                g_keyboard->install();

                while (g_connected) {
                    if (g_keyboard->key_available()) {
                        key = (unsigned char)g_keyboard->get_key();
                        if (key == 27) {
                            g_connected = 0;
                            break;
                        }
                    }

                    if (Protocol::recv_packet(g_serial, &req) == 0) {
                        if (req.type == MSG_KEYBOARD) {
                            // keyboard data received
                        }
                        else if (req.type == MSG_DISCONNECT) {
                            g_connected = 0;
                            break;
                        }
                    }

                    delay(50);
                }

                g_keyboard->uninstall();
                printf("Server stopped.\n");
                getch();
                return;
            }
        }

        if (kbhit()) {
            if (getch() == 27) return;
        }
    }
}

static void show_about() {
    clrscr();
    printf("\nPCAnywhere v4 DOS Clone\n");
    printf("A simple educational serial remote-control prototype.\n");
    printf("Targets Borland C++ 4.0 and MS-DOS.\n");
    printf("Includes serial COM routines, packet protocol,\n");
    printf("keyboard capture hooks, and screen capture logic.\n\n");
    printf("Press any key to continue...\n");
    getch();
}

int main() {
    int option;

    g_serial = new SerialPort(DEFAULT_COM);
    g_serial->init(DEFAULT_BAUD);

    g_keyboard = new KeyboardCapture();
    g_screen = new ScreenCapture();
    g_display = new Display();

    while (g_running) {
        show_main_menu();
        option = getch() - '0';

        switch (option) {
            case 1:
                client_mode();
                break;
            case 2:
                server_mode();
                break;
            case 3:
                configure_serial_port();
                break;
            case 4:
                test_connection();
                break;
            case 5:
                show_about();
                break;
            case 6:
                g_running = 0;
                break;
            default:
                break;
        }
    }

    if (g_serial) delete g_serial;
    if (g_keyboard) delete g_keyboard;
    if (g_screen) delete g_screen;
    if (g_display) delete g_display;

    clrscr();
    printf("Goodbye!\n");
    return 0;
}
