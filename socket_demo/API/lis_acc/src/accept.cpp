#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <iostream>
#include <errno.h>
#include <stdexcept>
#include <cstring>
#include <cstdlib>

class NetworkException : public std::runtime_error {
public:
    NetworkException(const std::string& message) : std::runtime_error(message) {}
};

// 信号处理函数
volatile sig_atomic_t stop = 0;
void handle_signal(int sig) {
    stop = 1;
}

int main(int argc, char* argv[]) {
    // 注册信号处理函数
    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);

    try {
        if (argc <= 3) {
            throw NetworkException("参数不足，需要提供 IP 地址、端口号和监听队列长度。");
        }

        const char* ip = argv[1];
        int port = atoi(argv[2]);
        int backlog = atoi(argv[3]);

        int sockfd = ::socket(PF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            throw NetworkException("创建套接字失败: " + std::string(strerror(errno)));
        }

        // 创建一个 IPv4 socket 地址
        struct sockaddr_in address;
        std::memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        if (inet_pton(AF_INET, ip, &address.sin_addr) <= 0) {
            close(sockfd);
            throw NetworkException("转换 IP 地址失败: " + std::string(strerror(errno)));
        }
        address.sin_port = htons(port);

        int ret = ::bind(sockfd, (sockaddr*)&address, sizeof(address));
        if (ret == -1) {
            close(sockfd);
            throw NetworkException("绑定套接字失败: " + std::string(strerror(errno)));
        }

        ret = ::listen(sockfd, backlog);
        if (ret == -1) {
            close(sockfd);
            throw NetworkException("监听套接字失败: " + std::string(strerror(errno)));
        }

        std::cout << "服务器开始监听..." << std::endl;

        // 等待 20 秒或直到收到终止信号
        for (int i = 0; i < 20 && !stop; ++i) {
            sleep(1);
        }

        if (stop) {
            std::cout << "收到终止信号，正在关闭服务器..." << std::endl;
        } else {
            struct sockaddr_in client;
            socklen_t client_addrlen = sizeof(client);
            int connfd = accept(sockfd, (sockaddr*)&client, &client_addrlen);
            if (connfd < 0) {
                throw NetworkException("接受客户端连接失败: " + std::string(strerror(errno)));
            }

            char remote[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN);
            std::cout << "接受来自 " << remote << ":" << ntohs(client.sin_port) << " 的连接" << std::endl;

            close(connfd);
        }

        close(sockfd);
    } catch (const NetworkException& e) {
        std::cerr << "异常: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}