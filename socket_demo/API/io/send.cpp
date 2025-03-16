#include "../include/head.h"

// 自定义异常类
class NetworkException : public std::runtime_error {
    public:
        NetworkException(const std::string& message) : std::runtime_error(message) {}
    };
    
    int main(int argc, char *argv[]) {
        try {
            if (argc <= 2) {
                throw NetworkException("参数不足，需要提供服务器 IP 地址和端口号。");
            }
    
            const char *ip = argv[1];
            int port = atoi(argv[2]);
    
            // 创建服务器地址结构体
            struct sockaddr_in server_address;
            std::memset(&server_address, 0, sizeof(server_address));
            server_address.sin_family = AF_INET;
            if (inet_pton(AF_INET, ip, &server_address.sin_addr) <= 0) {
                throw NetworkException("转换 IP 地址失败。");
            }
            server_address.sin_port = htons(port);
    
            // 创建套接字
            int sockfd = socket(PF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                throw NetworkException("创建套接字失败。");
            }
    
            // 连接到服务器
            if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
                close(sockfd);
                throw NetworkException("连接服务器失败。");
            }
    
            // 发送数据
            const char *oob_data = "abc";
            const char *normal_data = "123";
    
            // 发送普通数据
            if (send(sockfd, normal_data, strlen(normal_data), 0) < 0) {
                close(sockfd);
                throw NetworkException("发送普通数据失败。");
            }
    
            // 发送带外数据
            if (send(sockfd, oob_data, strlen(oob_data), MSG_OOB) < 0) {
                close(sockfd);
                throw NetworkException("发送带外数据失败。");
            }
    
            // 发送普通数据
            if (send(sockfd, normal_data, strlen(normal_data), 0) < 0) {
                close(sockfd);
                throw NetworkException("发送普通数据失败。");
            }
    
            // 关闭套接字
            close(sockfd);
        } catch (const NetworkException& e) {
            std::cerr << "异常: " << e.what() << std::endl;
            return 1;
        }
    
        return 0;
    }