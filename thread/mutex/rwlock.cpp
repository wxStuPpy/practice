#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>

pthread_rwlock_t rwlock;
int shared_data = 0;

// 写线程函数
void* write_lock(void *arg) {
    // 加写锁
    pthread_rwlock_wrlock(&rwlock);
    int temp = shared_data + 1;
    shared_data = temp;
    std::cout << static_cast<const char*>(arg) << ": shared_data = " << shared_data << std::endl;
    // 解锁
    pthread_rwlock_unlock(&rwlock);
    return nullptr;
}

// 读线程函数
void* read_lock(void* arg) {
    // 加读锁
    sleep(1);
    pthread_rwlock_rdlock(&rwlock);
    std::cout << static_cast<const char*>(arg) << ": shared_data = " << shared_data << std::endl;
    // 解锁
    pthread_rwlock_unlock(&rwlock);
    return nullptr;
}

int main() {
    // 动态初始化读写锁
    pthread_rwlock_init(&rwlock, nullptr);

    pthread_t tid1, tid2, tid3, tid4;

    // 创建两个写线程
    pthread_create(&tid1, nullptr, write_lock, const_cast<void*>(static_cast<const void*>("111")));
    pthread_create(&tid2, nullptr, write_lock, const_cast<void*>(static_cast<const void*>("222")));

    // 创建读线程
    pthread_create(&tid3, nullptr, read_lock, const_cast<void*>(static_cast<const void*>("333")));
    pthread_create(&tid4, nullptr, read_lock, const_cast<void*>(static_cast<const void*>("444")));

    // 主线程等待子线程
    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);
    pthread_join(tid3, nullptr);
    pthread_join(tid4, nullptr);

    // 销毁读写锁
    pthread_rwlock_destroy(&rwlock);

    return 0;
}