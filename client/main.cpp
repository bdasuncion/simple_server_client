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
#include "client.h"

static void usage();

char* findUsername(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (!strcasecmp("-u", argv[i]) && (i + 1) < argc) {
            return argv[i + 1];
        }
    }
    return NULL;
}

char* findHost(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (!strcasecmp("-h", argv[i]) && (i + 1) < argc) {
            return argv[i + 1];
        }
    }
    return NULL;
}

char* findPort(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (!strcasecmp("-p", argv[i]) && (i + 1) < argc) {
            return argv[i + 1];
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
   /* if (argc > 1 && *(argv[1]) == '-')
    {
        usage(); exit(1);
    }
*/
    const char *u = findUsername(argc, argv);
    std::string username(u ?  u: "default"); 

    const char *h = findHost(argc, argv);
    std::string host(h ?  h: "cpp_server_client-server-1");
    const char *p = findPort(argc, argv);
    std::string port(p ?  p: "3000");

    Client client;
    client.startConnection(username, host, port);

    return 0;
}

static void usage()
{
    std::cout << "A simple Internet client application.\n"
              << "Usage:\n"
              << "         client [IP_address_of_server [port_of_server]]\n"
              << "     where IP_address_of_server is either IP number of server\n"
              << "     or a symbolic Internet name, default is \"localhost\";\n"
              << "     port_of_server is a port number, default is 1234.\n"
              << "The client connects to a server which address is given in a\n"
              << "command line, receives a message from a server, sends the message\n"
              << "\"Thanks! Bye-bye...\", and terminates.\n";
}