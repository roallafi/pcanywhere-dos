#include "screen.h"

ScreenCapture::ScreenCapture() {
    active = 1;
    memset(buffer, 0, SCREEN_SIZE);
    memset(last_buffer, 0, SCREEN_SIZE);
}

ScreenCapture::~ScreenCapture() {
    active = 0;
}

int ScreenCapture::capture_screen() {
    if (!active) return -1;

    unsigned char far* video_memory = (unsigned char far*) MK_FP(SCREEN_SEG, 0);
    memcpy(buffer, video_memory, SCREEN_SIZE);
    return 0;
}

int ScreenCapture::send_screen(SerialPort* port) {
    Packet pkt;
    int chunks, i;

    if (!active || !port || !port->is_initialized()) return -1;

    chunks = (SCREEN_SIZE + PACKET_SIZE - 1) / PACKET_SIZE;

    for (i = 0; i < chunks; i++) {
        int offset = i * PACKET_SIZE;
        int size = SCREEN_SIZE - offset;

        if (size > PACKET_SIZE) size = PACKET_SIZE;

        Protocol::create_packet(&pkt, MSG_SCREEN, (unsigned char)i,
                                &buffer[offset], (unsigned short)size);

        if (Protocol::send_packet(port, &pkt) < 0) return -1;
        delay(5);
    }

    return 0;
}

int ScreenCapture::receive_screen(SerialPort* port) {
    Packet pkt;
    int expected = (SCREEN_SIZE + PACKET_SIZE - 1) / PACKET_SIZE;
    int received = 0;
    int timeout = 5000;

    if (!active || !port || !port->is_initialized()) return -1;

    memset(buffer, 0, SCREEN_SIZE);

    while (received < expected && timeout--) {
        if (Protocol::recv_packet(port, &pkt) == 0) {
            if (pkt.type == MSG_SCREEN) {
                int offset = pkt.seq * PACKET_SIZE;
                memcpy(&buffer[offset], pkt.data, pkt.length);
                received++;
            }
        }
    }

    if (received == expected) return 0;
    return -1;
}

int ScreenCapture::display_screen() {
    if (!active) return -1;

    unsigned char far* video_memory = (unsigned char far*) MK_FP(SCREEN_SEG, 0);
    memcpy(video_memory, buffer, SCREEN_SIZE);
    return 0;
}

int ScreenCapture::has_changed() {
    int i;

    for (i = 0; i < SCREEN_SIZE; i++) {
        if (buffer[i] != last_buffer[i]) {
            memcpy(last_buffer, buffer, SCREEN_SIZE);
            return 1;
        }
    }

    return 0;
}
