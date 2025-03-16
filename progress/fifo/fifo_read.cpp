#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    const char * pipe_path = "./tmp/myfifo";

    // 检查并创建 tmp 目录
    if (mkdir("./tmp", 0777) == -1 && errno != EEXIST) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    }

    // 尝试打开命名管道
    int fd = open(pipe_path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buf[100];
    ssize_t read_num;
    // 从管道读取数据并输出到控制台
    while ((read_num = read(fd, buf, sizeof(buf) - 1)) > 0) {
        // 在数据末尾添加空字符以处理字符串
        buf[read_num] = '\0'; 
        ssize_t write_num = write(STDOUT_FILENO, buf, read_num);
        if (write_num == -1) {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }
        if (write_num != read_num) {
            std::cerr << "Partial write to stdout." << std::endl;
        }
    }

    if (read_num < 0) {
        perror("read");
    }

    std::cout << "接收管道数据完成" << std::endl;
    close(fd);
    
    return 0;
}