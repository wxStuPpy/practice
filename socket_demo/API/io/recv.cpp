#include "../include/head.h"

// 自定义异常类
class NetworkException : public std::runtime_error {
    public:
        NetworkException(const std::string& message) : std::runtime_error(message) {}
    };
    
    #define BUF_SIZE 1024
    
    int main(int argc, char *argv[]) {
        try {
            if (argc <= 2) {
                throw NetworkException("参数不足，需要提供服务器 IP 地址和端口号。");
            }
    
            const char *ip = argv[1];
            int port = atoi(argv[2]);
    
            // 创建服务器地址结构体
            struct sockaddr_in addr;
            std::memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
                throw NetworkException("转换 IP 地址失败。");
            }
            addr.sin_port = htons(port);
    
            // 创建套接字
            int sockfd = socket(PF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                throw NetworkException("创建套接字失败。");
            }
    
            // 绑定套接字
            int ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
            if (ret == -1) {
                close(sockfd);
                throw NetworkException("绑定套接字失败。");
            }
    
            // 监听连接
            ret = listen(sockfd, 5);
            if (ret == -1) {
                close(sockfd);
                throw NetworkException("监听套接字失败。");
            }
    
            // 接受客户端连接
            struct sockaddr_in client;
            socklen_t client_addr_len = sizeof(client);
            int connfd = accept(sockfd, (struct sockaddr*)&client, &client_addr_len);
            if (connfd < 0) {
                close(sockfd);
                throw NetworkException("接受客户端连接失败。");
            }
    
            // 接收数据
            char buffer[BUF_SIZE];
    
            // 接收普通数据
            std::memset(buffer, 0, BUF_SIZE);
            ret = recv(connfd, buffer, BUF_SIZE - 1, 0);
            if (ret < 0) {
                close(connfd);
                close(sockfd);
                throw NetworkException("接收普通数据失败。");
            }
            std::cout << "普通数据: " << buffer << std::endl;
    
            // 接收带外数据
            std::memset(buffer, 0, BUF_SIZE);
            ret = recv(connfd, buffer, BUF_SIZE - 1, MSG_OOB);
            if (ret < 0) {
                close(connfd);
                close(sockfd);
                throw NetworkException("接收带外数据失败。");
            }
            std::cout << "带外数据: " << buffer << std::endl;
    
            // 接收普通数据
            std::memset(buffer, 0, BUF_SIZE);
            ret = recv(connfd, buffer, BUF_SIZE - 1, 0);
            if (ret < 0) {
                close(connfd);
                close(sockfd);
                throw NetworkException("接收普通数据失败。");
            }
            std::cout << "普通数据: " << buffer << std::endl;
    
            // 关闭连接
            close(connfd);
            close(sockfd);
        } catch (const NetworkException& e) {
            std::cerr << "异常: " << e.what() << std::endl;
            return 1;
        }
    
        return 0;
    }