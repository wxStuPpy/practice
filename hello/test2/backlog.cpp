#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <cerrno>

static bool stop = false;

static void handle_term(int sig)
{
    stop = true;
}

int main(char argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Failed to listen socket: " << std::strerror(errno) << std::endl;
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int backlog = atoi(argv[3]);
    int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Failed to listen socket: " << std::strerror(errno) << std::endl;
        return 1;
    }
    //Create an IPv4 socket address
    sockaddr_in sockaddr;
    std::memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family=AF_INET;
    
}