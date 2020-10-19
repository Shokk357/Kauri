#ifndef KAURI_ASN_H
#define KAURI_ASN_H

#include <iostream>
#include <cstring>
#include <vector>
#include <ace/SOCK_Connector.h>

template<typename T>
struct ASN {
    uint8_t *encode(T &message);

    T decode(uint8_t *message);
};

template<typename T>
struct ASN<std::vector<T>> {
    void encode(int powVal, int &startVal, int initialSize, std::vector<uint8_t> &packet) {
        while (startVal > 0) {
            if (initialSize >= 256) {
                initialSize = initialSize >> 8;
                powVal++;
            } else {
                uint8_t curBlock = initialSize;
                packet.insert(packet.end(), reinterpret_cast<uint8_t *>(&curBlock), reinterpret_cast<uint8_t *>(&curBlock) + 1);
                initialSize = startVal - (initialSize << (8 * powVal));
                powVal = 0;
                startVal = initialSize;
            }
        }
    }

    void handle(std::vector<T> &message, std::vector<uint8_t> &packet) {
        int size = message.size() * sizeof(T);
        uint8_t LengthBlock = size;
        bool isLongForm = false;
        if (size < 128) {
            packet.insert(packet.begin(), reinterpret_cast<uint8_t *>(&LengthBlock), reinterpret_cast<uint8_t *>(&LengthBlock) +
                    sizeof(uint8_t));
        } else {
            encode(0, size, size, packet);
            LengthBlock = packet.size();
            isLongForm = true;
        }
        packet.insert(packet.end(), reinterpret_cast<uint8_t *>(message.data()),
                      reinterpret_cast<uint8_t *>(message.data() + message.size()));
        if (isLongForm) {
            LengthBlock += 128;
            packet.insert(packet.begin(), reinterpret_cast<uint8_t *>(&LengthBlock),
                          reinterpret_cast<uint8_t *>(&LengthBlock) + sizeof(uint8_t));
        }
    }

    std::vector<T> decode(uint8_t *message) {
        uint8_t firstByte;
        int blockAmount, messageLength = 0;
        memcpy(&firstByte, message, sizeof(uint8_t));
        if (firstByte > 127) {
            blockAmount = firstByte - 128;
            int power = firstByte - 129;
            for (int i = 0; i < blockAmount; i++) {
                uint8_t arg;
                memcpy(&arg, message + sizeof(uint8_t) * (i + 1), sizeof(uint8_t));
                messageLength += arg << (8 * power);
                power--;
            }
        } else {
            blockAmount = 0;
            messageLength = firstByte;
        }
        std::vector<T> val(messageLength / sizeof(T));
        memcpy(val.data(), message + sizeof(uint8_t) * (blockAmount + 1), messageLength);
        return val;
    }
};

template<>
struct ASN<int> {
    uint8_t *encode(int &message);

    int decode(uint8_t *message);
};

template<>
struct ASN<std::string> {
    uint8_t *encode(std::string &message);

    std::string decode(uint8_t *message);
};

#endif //KAURI_ASN_H
