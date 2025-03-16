#include"socket.h"

using namespace yazi::socket;

Socket::Socket():m_ip(""),m_port(0),m_sockfd(0){
    // 1、创建socket
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sockfd < 0)
    {
        
    }
 
}