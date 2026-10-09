#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string.h>
#include "serial.h"

#define MSG_HANDSHAKE   0x01
#define MSG_ACK         0x02
#define MSG_NAK         0x03
#define MSG_FILE_REQUEST 0x04
#define MSG_FILE_DATA   0x05
#define MSG_FILE_END    0x06
#define MSG_SCREEN      0x07
#define MSG_KEYBOARD    0x08
#define MSG_DISCONNECT  0x09
#define MSG_PING        0x0A

#define PACKET_SIZE     256
#define MAX_FILENAME    128

struct Packet {
    unsigned char type;
    unsigned char seq;
    unsigned short length;
    unsigned char data[PACKET_SIZE];
    unsigned char checksum;
};

struct FileInfo {
    char filename[MAX_FILENAME];
    unsigned long size;
    unsigned short date;
    unsigned short time;
};

class Protocol {
public:
    static void create_packet(Packet* pkt, unsigned char type, unsigned char seq,
                             const unsigned char* data, unsigned short len);
    static unsigned char calculate_checksum(const Packet* pkt);
    static int verify_packet(const Packet* pkt);
    static int send_packet(SerialPort* port, const Packet* pkt);
    static int recv_packet(SerialPort* port, Packet* pkt);
};

#endif
