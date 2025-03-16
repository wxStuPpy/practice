#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <mutex>
#include <condition_variable>

#define BUF_LEN 1024

char buf[BUF_LEN];
std::mutex mtx;
std::condition_variable cv;
bool data_available = false;

// 读线程执行的代码逻辑
void *input_thread(void *arg)
{
    int i = 0;
    while (1)
    {
        char c = fgetc(stdin);
        if (c != EOF)
        {
            std::unique_lock<std::mutex> lock(mtx);
            buf[i++] = c;
            if (i >= BUF_LEN)
            {
                i = 0;
            }
            data_available = true;
            cv.notify_one(); // 唤醒等待的输出线程
        }
    }
    return nullptr;
}

// 写线程执行的代码逻辑
void *output_thread(void *arg)
{
    int i = 0;
    while (1)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return data_available; }); // 等待数据可用

        if (buf[i])
        {
            std::cout << buf[i] << " ";
            std::cout.flush(); // 手动刷新输出缓冲区
            buf[i++] = '\0';
            if (i >= BUF_LEN)
            {
                i = 0;
            }
        }

        if (!buf[i]) {
            data_available = false;
        }
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    pthread_t pid_input, pid_output;
    memset(buf, '\0', BUF_LEN);

    // 创建读线程
    if (pthread_create(&pid_input, nullptr, input_thread, nullptr) != 0)
    {
        perror("pthread_create input_thread");
        return 1;
    }

    // 创建写线程
    if (pthread_create(&pid_output, nullptr, output_thread, nullptr) != 0)
    {
        perror("pthread_create output_thread");
        return 1;
    }

    // 主线程等待读写线程
    if (pthread_join(pid_input, nullptr) != 0)
    {
        perror("pthread_join input_thread");
        return 1;
    }

    if (pthread_join(pid_output, nullptr) != 0)
    {
        perror("pthread_join output_thread");
        return 1;
    }

    return 0;
}