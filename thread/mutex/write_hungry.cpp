#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>

pthread_rwlock_t rwlock;
pthread_rwlockattr_t rwlock_attr;

// 读线程函数
void* read_thread(void* arg) {
    pthread_rwlock_rdlock(&rwlock);
    printf("Read thread acquired the lock.\n");
    // 模拟读操作
    sleep(1);
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

// 写线程函数
void* write_thread(void* arg) {
    pthread_rwlock_wrlock(&rwlock);
    printf("Write thread acquired the lock.\n");
    // 模拟写操作
    sleep(1);
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

int main() {
    // 初始化读写锁属性
    pthread_rwlockattr_init(&rwlock_attr);

    // 设置读写锁属性为写优先
    if (pthread_rwlockattr_setkind_np(&rwlock_attr, PTHREAD_RWLOCK_PREFER_WRITER_NP) != 0) {
        perror("pthread_rwlockattr_setkind_np");
        return 1;
    }

    // 初始化读写锁，并使用设置好的属性
    if (pthread_rwlock_init(&rwlock, &rwlock_attr) != 0) {
        perror("pthread_rwlock_init");
        return 1;
    }

    pthread_t read_tid, write_tid;

    // 创建读线程
    if (pthread_create(&read_tid, NULL, read_thread, NULL) != 0) {
        perror("pthread_create (read)");
        return 1;
    }

    // 创建写线程
    if (pthread_create(&write_tid, NULL, write_thread, NULL) != 0) {
        perror("pthread_create (write)");
        return 1;
    }

    // 等待读线程结束
    if (pthread_join(read_tid, NULL) != 0) {
        perror("pthread_join (read)");
        return 1;
    }

    // 等待写线程结束
    if (pthread_join(write_tid, NULL) != 0) {
        perror("pthread_join (write)");
        return 1;
    }

    // 销毁读写锁
    if (pthread_rwlock_destroy(&rwlock) != 0) {
        perror("pthread_rwlock_destroy");
        return 1;
    }

    // 销毁读写锁属性
    if (pthread_rwlockattr_destroy(&rwlock_attr) != 0) {
        perror("pthread_rwlockattr_destroy");
        return 1;
    }

    return 0;
}