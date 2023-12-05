#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <thread>

int readFromClient(int socket,  int *connections, int *connectionCount);
void acceptConnection(int sockfd, int *connection) {
    struct sockaddr_in peeraddr;
    socklen_t peeraddr_len;

    std::cout << "ACCEPT CONNECTIONS\n";
    *connection = accept(sockfd, (struct sockaddr*) &peeraddr, &peeraddr_len);
    if (*connection < 0){
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "Connection from IP "
            << ( ( ntohl(peeraddr.sin_addr.s_addr) >> 24) & 0xff ) << "."  // High byte of address
            << ( ( ntohl(peeraddr.sin_addr.s_addr) >> 16) & 0xff ) << "."
            << ( ( ntohl(peeraddr.sin_addr.s_addr) >> 8) & 0xff )  << "."
            <<   ( ntohl(peeraddr.sin_addr.s_addr) & 0xff ) << ", port "   // Low byte of addr
            << ntohs(peeraddr.sin_port);
}

int readFromClient(int socket, int *connections, int *connectionCount) {
    char buffer[1024];
    const char *ack = "Acknowledge!\n";
    int res;
    while(1) {
        res = read(socket, buffer, 1023);
        if (res < 0) {
            std::cerr << "Error: " << strerror(errno) << std::endl;
            exit(1);
        }
        buffer[res] = 0;
        std::cout << "Received " << res << " bytes: " << buffer << "\n";
        for (int i = 0; i < *connectionCount; ++i) {
            if (socket == connections[i]) {
                continue;
            }
            write(connections[i], buffer, strlen(buffer));
        }
        
    }

}

void awaitClientConnection(int sockfd) {
    int clientSockets[4] = {-1, -1, -1, -1}, clientCount = 0;
    while(1) {
        acceptConnection(sockfd, &clientSockets[clientCount]);
        write(clientSockets[clientCount], "Hello!!\n", 8);
        std::thread *clientThread = new std::thread(readFromClient,
            clientSockets[clientCount], clientSockets, &clientCount);
        ++clientCount;
    }
}
int main() {
    int sockfd;
    printf("Hello world\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int listenport = 3000;
    if (sockfd < 0) {
        std::cout << "ERROR SOCKET\n";
        exit(1);
    }

    struct sockaddr_in myServer;
    memset(&myServer, 0, sizeof(struct sockaddr_in));
    myServer.sin_family = AF_INET;
    myServer.sin_port = htons(listenport);
    myServer.sin_addr.s_addr = htonl(INADDR_ANY);
    int res = bind(sockfd, (struct sockaddr*)&myServer, sizeof(myServer));
    if (res < 0) {
        std::cerr << "ERROR BIND\n";
        exit(1);
    }

    struct linger linger_opt = { 1, 0 };
    setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));
    res = listen(sockfd, 1);
    if (res < 0) {
        std::cerr << "ERROR LISTEN\n";
        exit(1);
    }

    //std::thread awaitClients(awaitClientConnection, (sockfd));
    //awaitClients.join();
    awaitClientConnection(sockfd);
    /*int clientSockets[4], clientCount = 0;
    acceptConnection(sockfd, clientSockets, clientCount);

    write(clientSockets[0], "Hello!!\n", 8);
 
    readFromClient(clientSockets[0]);

    close(clientSockets[0]); */
    std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX END\n";
    return 0;
}