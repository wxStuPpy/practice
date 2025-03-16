#include <iostream>
#include <sys/socket.h>  // 包含套接字相关的函数和数据结构定义，如 socket、bind、connect 等函数，以及 sockaddr 结构体的定义
#include <netinet/in.h>  // 包含 Internet 地址族相关的定义，如 sockaddr_in 结构体、in_addr 结构体，以及一些常量定义（如 AF_INET）
#include <arpa/inet.h>   // 包含网络地址转换函数，如 inet_pton、inet_ntop 等，用于在点分十进制和二进制网络地址之间转换

// 定义通用的套接字地址结构，用于表示各种类型的地址
// 该结构是一个通用的地址结构，可用于不同的协议族
// 许多网络编程函数使用这个通用结构作为参数
struct sockaddr
{
    // 地址族，指定使用的地址类型，例如 AF_INET 表示 IPv4 地址族
    unsigned short sa_family;
    // 存储具体地址信息的数组，长度为 14 字节
    // 不同的地址族使用这个数组的方式不同
    unsigned char sa_data[14];
};

// 定义 IPv4 地址结构，用于表示一个 32 位的 IPv4 地址
struct in_addr{
    // 存储 IPv4 地址的 32 位无符号整数
    // 该地址通常以网络字节序存储
    unsigned int s_addr;
};

// 定义 IPv4 套接字地址结构，专门用于表示 IPv4 地址和端口号
// 这个结构是 sockaddr 结构的一个特定版本，用于 IPv4 协议
struct sockaddr_in{
    // 地址族，对于 IPv4 地址，通常设置为 AF_INET
    unsigned short sin_family;
    // 存储端口号的 16 位无符号整数
    // 端口号以网络字节序存储
    unsigned short sin_port;
    // 存储 IPv4 地址的结构体
    struct in_addr sin_addr;
    // 填充字节，用于使 sockaddr_in 结构的长度与 sockaddr 结构相同
    // 这个数组必须全部设置为 0，以确保与 sockaddr 结构兼容
    unsigned char sin_zero[8];
};

