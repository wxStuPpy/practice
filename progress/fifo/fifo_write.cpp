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

#if 0
int main(int argc, char* argv[]) {
    const char *pipe_path = "./tmp/myfifo";

    // Create the directory if it doesn't exist
    mkdir("./tmp", 755);

    // Create the FIFO
    if (mkfifo(pipe_path, 664) == -1) {
        // Check if the FIFO already exists
        if (errno != EEXIST) {
            // If the error is not that the FIFO already exists, print an error message
            std::cerr << "Failed to create FIFO: " << strerror(errno) << std::endl;
            return 1;
        } else {
            std::cout << "FIFO already exists." << std::endl;
        }
    } else {
        std::cout << "FIFO created successfully." << std::endl;
    }

    // The FIFO will remain after the program exits unless it is deleted.
    // Consider adding code to delete the FIFO if it's no longer needed.
 
    return 0;
}

#endif
//有名管道的好处:不在局限于父子之间的通讯
int main(int argc, char* argv[]) {
    const char * pipe_path = "./tmp/myfifo";
    // 检查并创建 tmp 目录
    if (mkdir("./tmp", 0777) == -1 && errno != EEXIST) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    }
    if (mkfifo(pipe_path, 0664) != 0) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    // 对有名管道的特殊文件 创建 fd
    int fd = open(pipe_path, O_WRONLY);

    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buf[100];
    ssize_t read_num;
    // 读取控制台数据写入到管道中
    while ((read_num = read(STDIN_FILENO, buf, sizeof(buf) - 1)) > 0) {
        buf[read_num] = '\0'; // 添加空字符
        ssize_t write_num = write(fd, buf, read_num);
        if (write_num == -1) {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }
        if (write_num != read_num) {
            std::cerr << "Partial write to FIFO." << std::endl;
        }
    }
    if (read_num < 0) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }
    std::cout << "发送数据到管道完成" << std::endl;
    close(fd);
    //有名管道使用完毕之后
    // 删除命名管道
    if (unlink(pipe_path) == -1) {
        perror("unlink");
    }
    return 0;
}