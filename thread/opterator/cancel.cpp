#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <cstring>

void* task(void* arg) {
    std::cout << "1" << std::endl;
    sleep(3);
    // 人为调用取消点函数
    pthread_testcancel();
    std::cout << "2" << std::endl;
    return nullptr;
}

int main() {
    pthread_t tid;
    if (pthread_create(&tid, nullptr, task, nullptr) != 0) {
        perror("pthread_create");
        return 1;
    }

    // 取消子线程 只是发出一个停止的命令 并不是真的停止
    if (pthread_cancel(tid) != 0) {
        perror("pthread_cancel");
        return 1;
    }

    void* res;
    // 一直挂起 等待子线程运行结束
    if (pthread_join(tid, &res) != 0) {
        perror("pthread_join");
        return 1;
    }
    std::cout << "3" << std::endl;
    if (res == PTHREAD_CANCELED) {
        std::cout << "线程被取消" << std::endl;
    } else {
        std::cout << "线程未被取消" << std::endl;
    }
    return 0;
}
