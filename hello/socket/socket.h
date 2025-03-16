#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>

using std::string;

namespace yazi{
    namespace socket{  
        class Socket{
        public:
          Socket();
          ~Socket();
        protected:
        string m_ip;
        int m_port;
        int m_sockfd;
        };
    }
}