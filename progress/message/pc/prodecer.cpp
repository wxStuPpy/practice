#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <cstring>
#include <mqueue.h>
#include <time.h>

int main(int argc, char *argv[])
{
    char write_buf[100];
    const char *mq_name = "/pc";
    struct mq_attr attr;
    // 有用的参数 表示消息队列的容量
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;
    // 被忽略的消息 在创建消息队列时候用不到
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;
    // 创建时间结构体
    struct timespec time_info;
    // 创建消息队列
    mqd_t mqdes = mq_open(mq_name, O_RDWR | O_CREAT, 0664, &attr);
    if (mqdes == static_cast<mqd_t>(-1))
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // 不断接收控制台的数据 发送到消息队列
    while (1)
    {
        memset(write_buf, '\0', sizeof(write_buf));
        ssize_t read_count = read(STDIN_FILENO, write_buf, sizeof(write_buf));
        time_info.tv_sec += 3;
        // 出错
        if (read_count == -1)
        {
            perror("read");
            continue;
        }
        // // ctrl+d 关闭
        else if (read_count == 0)
        {

            std::cout << "EOF,exit\n";
            char eof = EOF;
            if (mq_timedsend(mqdes, &eof, sizeof(char), 0, &time_info) == -1)
            {
                perror("mq_timesend");
            }
            break;
        }
        // 正常接受到控制台可读消息
        else
        {
            if (mq_timedsend(mqdes, write_buf, sizeof(write_buf), 0, &time_info) == -1)
            {
                perror("mq_timesend");
            }
        }
    }
    // 关闭文件描述符
    mq_close(mqdes);

    // mq_unlink 应该由消费者清理 便于读完消息
    return 0;
}