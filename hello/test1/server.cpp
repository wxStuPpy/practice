#include <iostream>
#include <sys/socket.h> // 包含套接字相关的函数和数据结构定义，如 socket、bind、connect 等函数，以及 sockaddr 结构体的定义
#include <netinet/in.h> // 包含 Internet 地址族相关的定义，如 sockaddr_in 结构体、in_addr 结构体，以及一些常量定义（如 AF_INET）
#include <arpa/inet.h>  // 包含网络地址转换函数，如 inet_pton、inet_ntop 等，用于在点分十进制和二进制网络地址之间转换
#include <cstring>
#include <unistd.h>

using std::string;

int main(int argc, char *argv[])
{
    // 1、创建套接字
    // AF_INET 表示使用 IPv4 地址族
    // SOCK_STREAM 表示使用面向连接的流式套接字（TCP）
    // IPPROTO_TCP 明确指定使用 TCP 协议
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        // 若套接字创建失败，输出错误信息
        // std::strerror(errno) 会将 errno 对应的错误码转换为可读的错误信息字符串
        std::cerr << "Failed to create socket: " << std::strerror(errno) << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Create socket successfully" << std::endl;
    }

    // 2. 绑定 socket
    // 定义要绑定的 IP 地址，这里使用本地回环地址，仅能在本地访问
    std::string ip = "127.0.0.1";
    // 定义要绑定的端口号，客户端将通过此端口与服务器通信
    int port = 8080;
    // 定义一个 sockaddr_in 结构体变量，用于存储 IPv4 地址和端口信息
    struct sockaddr_in sockaddr;
    // 使用 memset 函数将 sockaddr 结构体的内存区域清零
    // 确保结构体中的所有成员初始化为 0，避免未定义行为
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    // 设置地址族为 IPv4，AF_INET 表示使用 IPv4 地址族
    sockaddr.sin_family = AF_INET;
    // 将点分十进制的 IP 地址字符串转换为 32 位的二进制网络字节序的 IP 地址
    // ip.c_str() 将 std::string 类型的 ip 转换为 C 风格的字符串
    // inet_addr 函数完成转换工作，返回值存储在 sockaddr.sin_addr.s_addr 中
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    // 将主机字节序的端口号转换为网络字节序
    // htons 函数（host to network short）用于进行端口号的字节序转换
    // 确保端口号在网络传输中能被正确解析
    sockaddr.sin_port = htons(port);
    // 调用 bind 函数将创建好的套接字（sockfd）与指定的地址和端口绑定
    // (struct sockaddr *)&sockaddr 将 sockaddr_in 结构体指针转换为通用的 sockaddr 结构体指针
    // 因为 bind 函数的第二个参数要求是 sockaddr 类型的指针
    // sizeof(sockaddr) 传递 sockaddr 结构体的大小
    // 如果 bind 函数返回值小于 0，表示绑定操作失败
    if (::bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        // 这里可以添加错误处理代码，例如输出错误信息
        std::cerr << "Failed to bind socket: " << std::strerror(errno) << std::endl;
        // 关闭已创建的套接字，释放资源
        close(sockfd);
        return 1;
    }
    else
    {
        std::cout << "Bind socket to ip: " << ip << " port: " << port << std::endl;
    }

    // 3、监听 socket
    // listen 函数将套接字设置为监听状态，等待客户端的连接请求
    // 第二个参数 1024 表示允许的最大连接请求队列长度
    if (::listen(sockfd, 1024) < 0)
    {
        std::cerr << "Failed to listen socket: " << std::strerror(errno) << std::endl;
        // 关闭已创建的套接字，释放资源
        close(sockfd);
        return 1;
    }
    else
    {
        std::cout << "Socket is listening for incoming connections..." << std::endl;
    }

    while (true)
    {
        // 4、接受客户端连接
        // accept 函数从监听队列中取出一个客户端连接请求，并创建一个新的套接字（connfd）用于与该客户端通信
        // 第二个和第三个参数分别用于获取客户端的地址信息，这里设为 nullptr 表示不获取
        int connfd = ::accept(sockfd, nullptr, nullptr);
        if (connfd < 0)
        {
            std::cerr << "Failed to accept socket: " << std::strerror(errno) << std::endl;
            // 这里不需要关闭监听套接字 sockfd，因为后续可能还有其他客户端连接请求
            continue;
        }

        char buf[1024] = {0};

        // 5、接受客户端数据
        // recv 函数从客户端套接字（connfd）接收数据，存储到 buf 缓冲区中
        // 第三个参数 sizeof(buf) 表示缓冲区的大小
        // 第四个参数 0 表示使用默认的接收方式
        size_t len = ::recv(connfd, buf, sizeof(buf), 0);
        if (len > 0)
        {
            std::cout << "Received data from connfd: " << connfd << " buf: " << buf << std::endl;
        }
        else if (len == 0)
        {
            std::cout << "Client connfd: " << connfd << " closed the connection." << std::endl;
        }
        else
        {
            std::cerr << "Failed to receive data from connfd: " << connfd << ": " << std::strerror(errno) << std::endl;
        }

        // 6. 向客户端发送数据
        // send 函数将接收到的数据原样发送回客户端
        // 第三个参数 len 表示要发送的数据长度
        // 第四个参数 0 表示使用默认的发送方式
        if (len > 0)
        {
            ::send(connfd, buf, len, 0);
        }

        // 7、关闭与客户端的连接套接字
        // 处理完当前客户端的请求后，关闭该客户端的套接字，释放资源
        ::close(connfd);
    }

    // 关闭监听套接字，通常不会执行到这里，因为是一个无限循环
    ::close(sockfd);
    return 0;
}