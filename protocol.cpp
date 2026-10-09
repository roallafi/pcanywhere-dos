#include "protocol.h"

void Protocol::create_packet(Packet* pkt, unsigned char type, unsigned char seq,
                            const unsigned char* data, unsigned short len) {
    pkt->type = type;
    pkt->seq = seq;
    pkt->length = len;

    if (data && len > 0) {
        memcpy(pkt->data, data, len < PACKET_SIZE ? len : PACKET_SIZE);
    }

    pkt->checksum = calculate_checksum(pkt);
}

unsigned char Protocol::calculate_checksum(const Packet* pkt) {
    unsigned char sum = 0;
    int i;

    sum += pkt->type;
    sum += pkt->seq;
    sum += (pkt->length & 0xFF);
    sum += (pkt->length >> 8);

    for (i = 0; i < pkt->length && i < PACKET_SIZE; i++) {
        sum += pkt->data[i];
    }

    return (unsigned char)(~sum + 1);
}

int Protocol::verify_packet(const Packet* pkt) {
    unsigned char expected = calculate_checksum(pkt);
    return pkt->checksum == expected ? 0 : -1;
}

int Protocol::send_packet(SerialPort* port, const Packet* pkt) {
    unsigned char* buffer = (unsigned char*) pkt;
    int packet_len = sizeof(Packet);
    int i;

    if (!port || !port->is_initialized()) return -1;

    if (port->send_byte(0xAA) < 0) return -1;
    if (port->send_byte(0x55) < 0) return -1;

    for (i = 0; i < packet_len; i++) {
        if (port->send_byte(buffer[i]) < 0) return -1;
    }

    return 0;
}

int Protocol::recv_packet(SerialPort* port, Packet* pkt) {
    unsigned char sync1, sync2;
    unsigned char* buffer = (unsigned char*) pkt;
    int packet_len = sizeof(Packet);
    int i, timeout;

    if (!port || !port->is_initialized()) return -1;

    timeout = 50000;
    while (timeout--) {
        if (port->recv_byte(&sync1) == 0) {
            if (sync1 == 0xAA) {
                if (port->recv_byte(&sync2) == 0 && sync2 == 0x55) {
                    break;
                }
            }
        }
    }

    if (timeout <= 0) return -1;

    for (i = 0; i < packet_len; i++) {
        timeout = 10000;
        while (timeout-- && port->recv_byte(&buffer[i]) < 0) {
        }
        if (timeout <= 0) return -1;
    }

    return verify_packet(pkt);
}
