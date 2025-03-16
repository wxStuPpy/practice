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
    char read_buf[100];
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
        memset(read_buf, '\0', sizeof(read_buf));
        time_info.tv_sec += 3;
        ssize_t read_count = mq_timedreceive(mqdes, read_buf, sizeof(read_buf), 0, &time_info);
        // 出错
        if (read_count == -1)
        {
            perror("read");
            continue;
        }
        // // ctrl+d 关闭
        else if (read_buf[0] == EOF)
        {
            std::cout << "接受到生产者发送的退出信息\n";
            break;
        }
        // 正常接受到控制台可读消息
        else
        {
            std::cout << read_buf << std::endl;
        }
    }
    // 关闭文件描述符
    mq_close(mqdes);

    // mq_unlink 应该由消费者清理 便于读完消息
    mq_unlink(mq_name);
    return 0;
}