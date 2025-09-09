/*
 * tcp_protocol.cpp
 */

#include "democle.h"

#ifndef HAS_EMBEDDED

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int lookup_host (const char *host, struct sockaddr_in * destination_address, int * destination_address_length)
{
    struct addrinfo hints, *res, *result;
    int errcode;
    char addrstr[100];
    void *ptr;

    memset (&hints, 0, sizeof (hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags |= AI_CANONNAME;

    errcode = getaddrinfo (host, NULL, &hints, &result);
    if (errcode != 0) {
        perror ("getaddrinfo");
        return -1;
    }

    res = result;

    while (res) {
        switch (res->ai_family) {
        case AF_INET:
            *destination_address = *((struct sockaddr_in *) res->ai_addr);
            *destination_address_length = res->ai_addrlen;
            freeaddrinfo(result);

            // destination_socket = socket(AF_INET, SOCK_DGRAM, 0);

            return 0;
            // //case AF_INET6:
            // ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
            // break;
        }
        res = res->ai_next;
    }

    freeaddrinfo(result);

    return -1;
}



void tcp_thread_start(TCPProtocol * p)
{
    p->run();
}


TCPProtocol::TCPProtocol(string _host, int port_num)
    : AbstractProtocol(), host(_host), port_number(port_num)
{
    tcp_thread = new thread(tcp_thread_start, this);
    tcp_thread->detach();
}

void TCPProtocol::run()
{
    int new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_number);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
            perror("accept");
            continue;
        }
        char buffer[1024];
        ssize_t valread = read(new_socket, buffer, 1024);
        // std::cout << "Received bytes: " << valread << std::endl;
        // for (auto i = 0; i < valread;i++)
        //     std::cout << std::hex << (unsigned int)buffer[i] << std::dec << " ";
        // std::cout << std::endl;

        DEMOCLEPacket p((uint8_t *)buffer, valread);
        std::string sender_agent_name, dest_agent_name;
        term_vector t;
        AtomicFormula af(t);
        p.get(sender_agent_name);
        p.get(dest_agent_name);
        p.get(af);

        // std::cout << "Agent : " << dest_agent_name << ", AtomicFormula : " << af << std::endl;

        DEMOCLE::put_message_in_queue(dest_agent_name, sender_agent_name, af);

        close(new_socket);
    }
}


void TCPProtocol::send_message(Agent * sender, url & destination, AtomicFormula & a)
{
    int client_fd;
    struct sockaddr_in address, peer_address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("client socket failed");
        return;
    }

    memset(&address, 0, sizeof(address));
    int len;
    lookup_host(destination.host.c_str(), &address, &len);
    address.sin_port = htons(atoi(destination.port.c_str()));

    if (connect(client_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return;
    }

    std::string source_url = "tcp://" + host + ":" + std::to_string(port_number) + "/" + sender->get_name();
    DEMOCLEPacket p;
    p.put(source_url);
    p.put(destination.path);
    p.put(a);
    send(client_fd, p.data(), p.size(), 0);
    close(client_fd);
}

#endif
