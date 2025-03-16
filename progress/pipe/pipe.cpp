#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <cstring>
#include <cstdlib>

int main(int argc, char *argv[]) {
    int fd[2];
    // fd[0] read
    // fd[1] write
    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE); // 使用 exit 而不是 _exit
    }
    
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE); // 使用 exit 而不是 _exit
    } else if (pid == 0) {
        // 子进程
        std::cout << "子进程" << std::endl;
        close(fd[1]); // 关闭写端
        char buf[100];
        ssize_t bytes_read = read(fd[0], buf, sizeof(buf) - 1); // 保留一个字节用于字符串的结尾
        if (bytes_read > 0) {
            buf[bytes_read] = '\0'; // 确保字符串正确终止
            std::cout << "读取到的数据: " << buf << std::endl;
        }
        close(fd[0]); // 关闭读端
        _exit(EXIT_SUCCESS); // 在子进程中使用 _exit
    } else {
        // 父进程
        std::cout << "父进程" << std::endl;
        close(fd[0]); // 关闭读端
        if (argc > 1) {
            write(fd[1], argv[1], strlen(argv[1])); // 写入命令行参数到管道
        }
        close(fd[1]); // 关闭写端
        waitpid(pid, NULL, 0); // 等待子进程结束
        exit(EXIT_SUCCESS); // 使用 exit 而不是 _exit
    }
    return 0; // 这个 return 0 实际上不会被执行，因为程序在 if-else 语句中已经退出了
}
