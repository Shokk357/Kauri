#include "ASN.h"

uint8_t* ASN<int>::encode(int& message) {
    return reinterpret_cast<uint8_t*>(&message);
}

int ASN<int>::decode(uint8_t *message) {
    int val = 0;
    for (int i = 3; i >= 0; i--) {
        val = val << 8;
        val += message[i];
    }
    return val;
}

uint8_t* ASN<std::string>::encode(std::string &message) {
    return reinterpret_cast<uint8_t*>(const_cast<char*>(message.data()));
}

std::string ASN<std::string>::decode(uint8_t *message) {
    int size = 10;
    char* val = reinterpret_cast<char*>(message);
    std::string rez;
    rez.append(val,size);
    return rez;
}

