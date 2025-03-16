#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <memory>
#include <mutex>

std::mutex input_mutex;

typedef struct
{
    char *p;
    int len;
} Result;

//如果一个进程的多个线程使用到标准输入输出 会造冲突问题 最终之后导致只有一个线程适用到标准的输入输出

void *red_thread(void *arg)
{
    Result *res = new Result();
    // 解析传递的参数
    char code = *(static_cast<char*>(arg));

    std::string str;
    while (1)
    {
        {
            std::lock_guard<std::mutex> lock(input_mutex);
            std::cin >> str;
        }
        if (str[0] == code)
        {
            // 接受了回复的信息
            std::cout << "Received message starting with " << code << std::endl;
            char *redans = strdup("red back");
            res->p = redans;
            res->len = strlen(redans);
            // 结束线程
            pthread_exit(static_cast<void*>(res));
        }
        else
        {
            std::cout << "red error" << std::endl;
        }
    }
    return nullptr;
}

void *white_thread(void *arg)
{
    Result *res = new Result();
    // 解析传递的参数
    char code = *(static_cast<char*>(arg));

    std::string str;
    while (1)
    {
        {
            std::lock_guard<std::mutex> lock(input_mutex);
            std::cin >> str;
        }
        if (str[0] == code)
        {
            // 接受了回复的信息
            std::cout << "Received message starting with " << code << std::endl;
            char *redans = strdup("white back");
            res->p = redans;
            res->len = strlen(redans);
            // 结束线程
            pthread_exit(static_cast<void*>(res));
        }
        else
        {
            std::cout << "white error" << std::endl;
        }
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    // 创建线程
    pthread_t pid_red, pid_white;
    char red_code = 'r';
    char white_code = 'w';
    pthread_create(&pid_red, nullptr, red_thread, &red_code);
    pthread_create(&pid_white, nullptr, white_thread, &white_code);

    // 获取线程结果
    Result *red_res = nullptr;
    Result *white_res = nullptr;
    void *temp;

    // 获取红色线程的结果
    if (pthread_join(pid_red, &temp) == 0)
    {
        red_res = static_cast<Result*>(temp);
        // 可以在这里使用 red_res
        if (red_res)
        {
            std::cout << *(red_res->p)<<" len = " << red_res->len << std::endl;
            // 使用完后记得释放内存
            if (red_res->p)
            {
                free(red_res->p);
            }
            delete red_res;
        }
    }

    // 获取白色线程的结果
    if (pthread_join(pid_white, &temp) == 0)
    {
        white_res = static_cast<Result*>(temp);
        // 可以在这里使用 white_res
        if (white_res)
        {
            std::cout << *(white_res->p)<<" len = " << white_res->len << std::endl;
            // 使用完后记得释放内存
            if (white_res->p)
            {
                free(white_res->p);
            }
            delete white_res;
        }
    }
    return 0;
}