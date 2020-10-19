#ifndef KAURI_CLIENT_H
#define KAURI_CLIENT_H

struct client {

    void bindToServer(int port, std::string &addr) {
        address.set(port, addr.data(), PF_INET);
        int isConnect = -1;
        while (isConnect == -1) {
            isConnect = connector.connect(TCPSock, address);
        }
        TCPSock.enable(ACE_NONBLOCK);
    }

    void sendMessage(std::vector<uint8_t> &msg) {
        int packLen = msg.size();
        TCPSock.send_n(&packLen, 4, MSG_NOSIGNAL);
        TCPSock.send_n(msg.data(), packLen, MSG_NOSIGNAL);
        TCPSock.close_writer();
        TCPSock.close();
    }

private:
    ACE_SOCK_Stream TCPSock;
    ACE_INET_Addr address;
    ACE_SOCK_Connector connector;
};
#endif //KAURI_CLIENT_H
