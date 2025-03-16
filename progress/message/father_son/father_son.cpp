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
    const char *mq_name = "/father_son";
    struct mq_attr attr;
    // 有用的参数 表示消息队列的容量
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;
    // 被忽略的消息 在创建消息队列时候用不到
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;
    // 创建消息队列
    mqd_t mqdes = mq_open(mq_name, O_RDWR | O_CREAT, 0664, &attr);
    if (mqdes == static_cast<mqd_t>(-1))
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // 创建父子进程
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // 子进程 等待接收消息队列中的消息
        char recv_buf[100];
        struct timespec time_info;
        for (int i = 0; i < 10; ++i)
        {
            // 清空接收缓冲区
            memset(recv_buf, '\0', sizeof(recv_buf));
            // 获取当前时间
            clock_gettime(CLOCK_REALTIME, &time_info);
            // 等待2秒
            time_info.tv_sec += 2;
            // 接受消息 并打印到控制台
            ssize_t bytes_received = mq_timedreceive(mqdes, recv_buf, sizeof(recv_buf), 0, &time_info);
            if (bytes_received == -1)
            {
                perror("recv");
                continue;
            }
            std::cout << recv_buf << std::endl;
        }
        std::cout << "子进程接收完毕\n";
    }
    else
    {
        // 父进程 发送消息到消息队列中
        char send_buf[100];
        struct timespec time_info;
        for (int i = 0; i < 10; ++i)
        {
            // 清空发送缓冲区
            memset(send_buf, '\0', sizeof(send_buf));
            sprintf(send_buf, "父进程第%d次发送消息", i + 1);
            // 获取当前时间
            clock_gettime(CLOCK_REALTIME, &time_info);
            // 等待2秒
            time_info.tv_sec += 2;
            if (mq_timedsend(mqdes, send_buf, strlen(send_buf), 0, &time_info) == -1)
            {
                perror("send");
                continue;
            }
            sleep(1);
        }
        std::cout << "父进程发送完毕\n";
    }

    // 父子进程 都需要进行释放消息队列的引用
    if (mq_close(mqdes) == -1)
    {
        perror("mq_close");
    }

    // 清除消息队列只需要一次 因为这个消息队列可以认为是公有的
    if (pid > 0)
    {
        if (mq_unlink(mq_name) == -1)
        {
            perror("mq_unlink");
        }
    }

    return 0;
}