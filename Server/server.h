#ifndef KAURI_SERVER_H
#define KAURI_SERVER_H

#include "../ASN/ASN.h"

struct server {

    server(int port) {
        serverAddr.set_port_number(1234);
        acceptor.open(serverAddr, 1);
    }

    void handler(ACE_SOCK_Stream TCPSock) {
        int packetLength;
        TCPSock.recv_n(&packetLength, 4, 0);
        uint8_t *packet = new uint8_t[packetLength];
        TCPSock.recv_n(packet, packetLength, 0);
        ASN<std::vector<int>> intTest;
        std::vector<int> message = intTest.decode(packet);
        for (auto element : message) {
            std::cout << element << ' ';
        }
        std::cout << std::endl;
        delete[] packet;
        TCPSock.close_reader();
        TCPSock.close();
    }
    
    void handlingLoop() {
        ACE_SOCK_Stream TCPSock;
        ACE_INET_Addr clientAddr;
        while (true) {
            if (acceptor.accept(TCPSock, &clientAddr, &timeout, 0) != -1) {
                clents.push_back(std::thread([this, &TCPSock] {
                    handler(TCPSock);
                    clentsId.push_back(std::this_thread::get_id());
                }));
            }
            if (!clentsId.empty())
                for (std::list<std::thread::id>::iterator id_it = clentsId.begin(); !clentsId.empty(); id_it = clentsId.begin())
                    for (std::list<std::thread>::iterator thr_it = clents.begin();
                         thr_it != clents.end(); ++thr_it)
                        if (thr_it->get_id() == *id_it) {
                            thr_it->join();
                            clents.erase(thr_it);
                            clentsId.erase(id_it);
                            break;
                        }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

private:
    ACE_INET_Addr serverAddr;
    ACE_SOCK_Acceptor acceptor;
    std::list<std::thread> clents;
    std::list<std::thread::id> clentsId;
    ACE_Time_Value timeout = ACE_Time_Value(10, 0);
};

#endif //KAURI_SERVER_H
