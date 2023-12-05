#include "client.h"
#include <stdlib.h>

void Client::startConnection(std::string username, std::string host, std::string port) {
    std::cout << "USERNAME:" << username << "\n";

    int socket = getConnectionToServer(host.c_str(), (short)atoi(port.c_str()));

    std::thread inputThread(&Client::writeToServer, this, socket, username);
    readFromServer(socket);

    close(socket);
} 

int Client::writeToServer(int socket, std::string name) {
    while(1) {
        std::string input;
        std::getline(std::cin >> std::ws, input);
        std::string fullmessage = name + ">> " + input; 
        write(socket, fullmessage.c_str(), strlen(fullmessage.c_str()));
    }
}

int Client::getConnectionToServer(const char *peerHost, short peerPort) {
    // Create socket
    int s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (s0 < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    // Fill in server IP address
    struct sockaddr_in server;
    int serverAddrLen;
    bzero( &server, sizeof( server ) );

    // Resolve server address (convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(peerHost);
    if (host == NULL)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(peerPort);

    // Print a resolved address of server (the first IP of the host)
    std::cout << "server address = " << (host->h_addr_list[0][0] & 0xff) << "." <<
                                        (host->h_addr_list[0][1] & 0xff) << "." <<
                                        (host->h_addr_list[0][2] & 0xff) << "." <<
                                        (host->h_addr_list[0][3] & 0xff) << ", port " <<
                                        static_cast<int>(peerPort) << std::endl;

    // Write resolved IP address of a server to the address structure
    memmove(&(server.sin_addr.s_addr), host->h_addr_list[0], 4);

    // Connect to the remote server
    int res = connect(s0, (struct sockaddr*) &server, sizeof(server));
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "Connected. Reading a server message" << std::endl;

    char buffer[1024];
    res = read(s0, buffer, 1024);
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "Received:" << "\n" << buffer;

    return s0;
}

void Client::readFromServer(int socket) {
    char buffer[1024];
    int res;
    while(1) {
        res = read(socket, buffer, 1024);
        buffer[res] = 0;
        std::cout << buffer << "\n";
    }
}