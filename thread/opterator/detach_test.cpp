#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <cstring>

void *task(void *arg)
{
    std::cout << 1 << std::endl;
    sleep(3);
    std::cout << 2 << std::endl;
    return nullptr;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, task, nullptr);

    // 使用detach标记会等待线程完成之后回收相关资源
    pthread_detach(tid);

    // 保证主线程完成速度比创建的子线程慢
    std::cout << 3 << std::endl;
    //sleep(5); // 主线程不会再detach等待
    std::cout << 4 << std::endl;

    return 0;
}