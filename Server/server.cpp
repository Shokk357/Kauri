#include <iostream>
#include <ace/SOCK_Acceptor.h>
#include <ace/Thread_Manager.h>
#include <ace/Reactor.h>
#include <ace/Svc_Handler.h>
#include <thread>
#include <list>
#include "server.h"

int main() {
    server server(1234);
    server.handlingLoop();
}
