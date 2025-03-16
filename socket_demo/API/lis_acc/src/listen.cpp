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

// 自定义异常类
class NetworkException : public std::runtime_error {
public:
    NetworkException(const std::string& message) : std::runtime_error(message) {}
};

static bool stop = false;

static void handle_term(int sig) {
    stop = true;
}

int main(int argc, char* argv[]) {
    signal(SIGTERM, handle_term);

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

        while (!stop) {
            ::sleep(1);
        }

        close(sockfd);
    } catch (const NetworkException& e) {
        std::cerr << "异常: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}