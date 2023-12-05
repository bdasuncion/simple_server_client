#include <iostream>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <thread>

class Client {
    private:
        int getConnectionToServer(const char *peerHost, short peerPort);
        void readFromServer(int socket);
        int writeToServer(int socket, std::string name);
    public:
        void startConnection(std::string username, std::string host, std::string port);
};