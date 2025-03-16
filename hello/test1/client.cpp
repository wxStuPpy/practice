#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <cerrno>

using std::string;

int main()
{
    // 1、创建socket
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        std::cerr << "Failed to create socket: " << std::strerror(errno) << std::endl;
        return 1;
    }
    std::cout << "Create socket successfully" << std::endl;

    // 2、连接服务端
    std::string ip = "127.0.0.1";
    int port = 8080;

    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr.sin_port = htons(port);

    if (::connect(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        std::cerr << "socket connect error: " << std::strerror(errno) << std::endl;
        close(sockfd);
        return 1;
    }
    std::cout << "Connected to server successfully" << std::endl;

    // 3、向服务端发送数据
    std::string data = "hello world";
    ssize_t sent_bytes = ::send(sockfd, data.c_str(), data.size(), 0);
    if (sent_bytes < 0)
    {
        std::cerr << "Failed to send data: " << std::strerror(errno) << std::endl;
        close(sockfd);
        return 1;
    }
    else if (sent_bytes != static_cast<ssize_t>(data.size()))
    {
        std::cerr << "Only sent " << sent_bytes << " bytes out of " << data.size() << " bytes." << std::endl;
    }

    // 4、接受服务端的数据
    char buf[1024] = {0};
    ssize_t recv_bytes = ::recv(sockfd, buf, sizeof(buf) - 1, 0);
    if (recv_bytes < 0)
    {
        std::cerr << "Failed to receive data: " << std::strerror(errno) << std::endl;
    }
    else if (recv_bytes == 0)
    {
        std::cout << "Server closed the connection." << std::endl;
    }
    else
    {
        buf[recv_bytes] = '\0'; // 确保字符串以 '\0' 结尾
        std::cout << "Received from server: " << buf << std::endl;
    }

    // 5、关闭socket
    ::close(sockfd);
    return 0;
}