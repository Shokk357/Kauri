#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <unordered_map>
#include "../ASN/ASN.h"

int main() {

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(1234);
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bind(server_sockfd, (struct sockaddr *) &server_address, sizeof(server_address));

    listen(server_sockfd, 5);

    while (true) {
        int client_sockfd = accept(server_sockfd, nullptr, nullptr);
        int packLen;
        recv(client_sockfd, &packLen, 4, 0);
        uint8_t *s = new uint8_t[packLen];
        recv(client_sockfd, s, packLen, 0);
        ASN<std::vector<int>> elem;
        std::vector<int> message = elem.decode(s);
        for (auto elem : message) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
        delete[] s;
        shutdown(client_sockfd, SHUT_RDWR);
        close(client_sockfd);
    }
}
