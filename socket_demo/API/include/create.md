这段代码主要用于创建一个 TCP 套接字并初始化服务器的地址信息，下面为你逐行详细解释：

### 1. 定义并初始化 `server_address` 结构体
```cpp
struct sockaddr_in server_address;
std::memset(&server_address, 0, sizeof(server_address));
```
- `struct sockaddr_in server_address;`：定义了一个 `sockaddr_in` 类型的结构体变量 `server_address`。`sockaddr_in` 是一个在网络编程中用于存储 IPv4 地址信息的结构体，它在 `<netinet/in.h>` 头文件中定义。
- `std::memset(&server_address, 0, sizeof(server_address));`：使用 `std::memset` 函数将 `server_address` 结构体的所有字节初始化为 0。这是为了确保结构体中的所有成员都被初始化为已知状态，避免出现未定义的值。

### 2. 设置地址族和 IP 地址
```cpp
server_address.sin_family = AF_INET;
inet_pton(AF_INET, ip, &server_address.sin_addr);
```
- `server_address.sin_family = AF_INET;`：将 `server_address` 结构体的 `sin_family` 成员设置为 `AF_INET`，表示使用 IPv4 地址族。`sin_family` 是 `sockaddr_in` 结构体中的一个成员，用于指定地址族类型。
- `inet_pton(AF_INET, ip, &server_address.sin_addr);`：
    - `inet_pton` 是一个函数，用于将点分十进制的 IP 地址字符串（如 `127.0.0.1`）转换为二进制形式的 IP 地址。
    - 第一个参数 `AF_INET` 表示使用 IPv4 地址族。
    - 第二个参数 `ip` 是一个指向点分十进制 IP 地址字符串的指针，这个字符串通常是从命令行参数或其他地方获取的。
    - 第三个参数 `&server_address.sin_addr` 是一个指向 `in_addr` 结构体的指针，用于存储转换后的二进制 IP 地址。`sin_addr` 是 `sockaddr_in` 结构体中的一个成员，用于存储 IP 地址。

### 3. 设置端口号
```cpp
server_address.sin_port = htons(port);
```
- `server_address.sin_port` 是 `sockaddr_in` 结构体中的一个成员，用于存储端口号。
- `htons` 是一个函数，用于将主机字节序（通常是小端字节序）的端口号转换为网络字节序（大端字节序）。因为不同的计算机体系结构可能使用不同的字节序，为了确保在网络上传输的数据能够被正确解析，需要将端口号转换为网络字节序。`port` 是一个整数类型的变量，表示要使用的端口号。

### 4. 创建套接字
```cpp
int sockfd = ::socket(PF_INET, SOCK_STREAM, 0);
```
- `::socket` 是一个系统调用函数，用于创建一个套接字。`::` 是作用域解析运算符，这里使用它是为了确保调用的是全局命名空间中的 `socket` 函数。
- 第一个参数 `PF_INET` 表示使用 IPv4 协议族，`PF_INET` 与 `AF_INET` 在大多数情况下是等价的。
- 第二个参数 `SOCK_STREAM` 表示创建一个面向连接的、可靠的 TCP 套接字。
- 第三个参数 `0` 表示使用默认的协议，对于 TCP 套接字，默认协议是 TCP 协议。
- 函数返回一个整数类型的文件描述符 `sockfd`，用于后续对该套接字进行操作，如绑定地址、监听连接等。如果创建套接字失败，`sockfd` 的值为 -1。

综上所述，这段代码的主要作用是创建一个 TCP 套接字，并初始化服务器的地址信息，为后续的网络通信做准备。