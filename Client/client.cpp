#include <iostream>
#include "../ASN/ASN.h"
#include "client.h"

int main() {
    ASN<std::vector<int>> intTest;
    int size = 15;
    std::vector<int> vals;
    std::vector<uint8_t> encodedMessage;
    for (int i = 0; i < size; i++) {
        vals.push_back(i);
    }
    intTest.handle(vals, encodedMessage);
    client client;
    std::string addr = "127.0.0.1";
    client.bindToServer(1234, addr);
    client.sendMessage(encodedMessage);
    return 0;
}
