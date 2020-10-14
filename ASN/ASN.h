#ifndef KAURI_ASN_H
#define KAURI_ASN_H

#include <iostream>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

template<typename T>
struct ASN {
    uint8_t *encode(T &a);

    T decode(uint8_t *a);
};

template<typename T>
struct ASN<std::vector<T>> {
    void snd(std::vector<uint8_t> &msg) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in newAddr;
        newAddr.sin_family = AF_INET;
        newAddr.sin_port = htons(1234);
        newAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        int optval = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        int res = connect(sock, (struct sockaddr *) &newAddr, sizeof(newAddr));
        if (res == -1) {
            std::cout << "Connection failed!" << std::endl;
        } else {
            int packLen = msg.size();
            send(sock, &packLen, 4, MSG_NOSIGNAL);
            send(sock, msg.data(), packLen, MSG_NOSIGNAL);
        }
        shutdown(sock, SHUT_WR);
        close(sock);
    }

    void encode(int start, int &initial, int len, std::vector<uint8_t> &packet) {
        while (initial > 0) {
            if (len >= 256) {
                len = len >> 8;
                start++;
            } else {
                packet.insert(packet.end(), reinterpret_cast<uint8_t *>(&len), reinterpret_cast<uint8_t *>(&len) + 4);
                len = initial - (len << (8 * start));
                start = 0;
                initial = len;
            }
        }
    }

    void handle(std::vector<T> &a, std::vector<uint8_t> &packet) {
        int size = a.size() * sizeof(T), bytesAmount = 0;
        bool isLongForm = false;
        if (size < 128) {
            packet.insert(packet.begin(), reinterpret_cast<uint8_t *>(&size), reinterpret_cast<uint8_t *>(&size) + 4);
        } else {
            encode(0, size, size, packet);
            bytesAmount = packet.size() / 4;
            isLongForm = true;
        }
        packet.insert(packet.end(), reinterpret_cast<uint8_t *>(a.data()),reinterpret_cast<uint8_t *>(a.data() + a.size()));
        if (isLongForm) {
            bytesAmount += 128;
            packet.insert(packet.begin(), reinterpret_cast<uint8_t *>(&bytesAmount),
                          reinterpret_cast<uint8_t *>(&bytesAmount) + 4);
        }
    }

    std::vector<T> decode(uint8_t *a) {
        int firstByte, blockAmount, messageLength = 0;
        memcpy(&firstByte, a, sizeof(int));
        if (firstByte > 127) {
            blockAmount = firstByte - 128;
            int power = firstByte - 129;
            for (int i = 0; i < blockAmount; i++) {
                int arg;
                memcpy(&arg, a + sizeof(int) * (i + 1), sizeof(int));
                messageLength += arg << (8 * power);
                power--;
            }
        } else {
            blockAmount = 0;
            messageLength = firstByte;
        }
        std::vector<T> val(messageLength / sizeof(T));
        memcpy(val.data(), a + sizeof(int) * (blockAmount + 1), messageLength);
        return val;
    }
};

template<>
struct ASN<int> {
    uint8_t *encode(int &a);

    int decode(uint8_t *a);
};

template<>
struct ASN<std::string> {
    uint8_t *encode(std::string &a);

    std::string decode(uint8_t *a);
};

#endif //KAURI_ASN_H
