在 Linux 系统编程里，静态锁和动态锁是并发编程中用于保护共享资源的重要机制，下面以互斥锁（mutex）和读写锁（rwlock）为例，分别介绍它们的静态和动态初始化、销毁等操作。

### 互斥锁（Mutex）

#### 静态初始化和销毁
静态初始化是在编译时就完成锁的初始化，无需在运行时调用额外的初始化函数。
```c
#include <stdio.h>
#include <pthread.h>

// 静态初始化互斥锁
pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_function(void* arg) {
    // 加锁
    pthread_mutex_lock(&my_mutex);
    printf("Thread has locked the mutex.\n");
    // 模拟一些工作
    for (int i = 0; i < 1000000; i++);
    printf("Thread is unlocking the mutex.\n");
    // 解锁
    pthread_mutex_unlock(&my_mutex);
    return NULL;
}

int main() {
    pthread_t thread;
    // 创建线程
    pthread_create(&thread, NULL, thread_function, NULL);
    // 等待线程结束
    pthread_join(thread, NULL);

    // 静态初始化的互斥锁无需显式销毁
    return 0;
}
```
静态初始化使用 `PTHREAD_MUTEX_INITIALIZER` 宏，这种方式简洁明了，但它缺乏错误检查。并且，静态初始化的互斥锁在程序结束时无需显式调用销毁函数。

#### 动态初始化和销毁
动态初始化在运行时调用 `pthread_mutex_init` 函数来初始化互斥锁，同时可以传入参数进行定制。
```c
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t my_mutex;

void* thread_function(void* arg) {
    // 加锁
    pthread_mutex_lock(&my_mutex);
    printf("Thread has locked the mutex.\n");
    // 模拟一些工作
    for (int i = 0; i < 1000000; i++);
    printf("Thread is unlocking the mutex.\n");
    // 解锁
    pthread_mutex_unlock(&my_mutex);
    return NULL;
}

int main() {
    // 动态初始化互斥锁
    if (pthread_mutex_init(&my_mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return 1;
    }

    pthread_t thread;
    // 创建线程
    pthread_create(&thread, NULL, thread_function, NULL);
    // 等待线程结束
    pthread_join(thread, NULL);

    // 销毁互斥锁
    if (pthread_mutex_destroy(&my_mutex) != 0) {
        perror("Mutex destruction failed");
        return 1;
    }

    return 0;
}
```
动态初始化通过 `pthread_mutex_init` 函数进行，第一个参数是要初始化的互斥锁指针，第二个参数可以传入 `NULL` 表示使用默认属性。使用完互斥锁后，需要调用 `pthread_mutex_destroy` 函数进行销毁，以释放相关资源。

### 读写锁（Rwlock）

#### 静态初始化和销毁
```c
#include <stdio.h>
#include <pthread.h>

// 静态初始化读写锁
pthread_rwlock_t my_rwlock = PTHREAD_RWLOCK_INITIALIZER;

void* reader_function(void* arg) {
    // 加读锁
    pthread_rwlock_rdlock(&my_rwlock);
    printf("Reader has locked the rwlock.\n");
    // 模拟一些工作
    for (int i = 0; i < 1000000; i++);
    printf("Reader is unlocking the rwlock.\n");
    // 解读锁
    pthread_rwlock_unlock(&my_rwlock);
    return NULL;
}

int main() {
    pthread_t thread;
    // 创建线程
    pthread_create(&thread, NULL, reader_function, NULL);
    // 等待线程结束
    pthread_join(thread, NULL);

    // 静态初始化的读写锁无需显式销毁
    return 0;
}
```
静态初始化读写锁使用 `PTHREAD_RWLOCK_INITIALIZER` 宏，和静态初始化的互斥锁一样，在程序结束时无需显式销毁。

#### 动态初始化和销毁
```c
#include <stdio.h>
#include <pthread.h>

pthread_rwlock_t my_rwlock;

void* reader_function(void* arg) {
    // 加读锁
    pthread_rwlock_rdlock(&my_rwlock);
    printf("Reader has locked the rwlock.\n");
    // 模拟一些工作
    for (int i = 0; i < 1000000; i++);
    printf("Reader is unlocking the rwlock.\n");
    // 解读锁
    pthread_rwlock_unlock(&my_rwlock);
    return NULL;
}

int main() {
    // 动态初始化读写锁
    if (pthread_rwlock_init(&my_rwlock, NULL) != 0) {
        perror("Rwlock initialization failed");
        return 1;
    }

    pthread_t thread;
    // 创建线程
    pthread_create(&thread, NULL, reader_function, NULL);
    // 等待线程结束
    pthread_join(thread, NULL);

    // 销毁读写锁
    if (pthread_rwlock_destroy(&my_rwlock) != 0) {
        perror("Rwlock destruction failed");
        return 1;
    }

    return 0;
}
```
动态初始化读写锁使用 `pthread_rwlock_init` 函数，第一个参数是要初始化的读写锁指针，第二个参数可传入 `NULL` 表示使用默认属性。使用完读写锁后，需要调用 `pthread_rwlock_destroy` 函数进行销毁。

综上所述，静态初始化适用于简单、固定的场景，代码简洁；而动态初始化更灵活，可进行错误检查和属性定制，但需要额外的初始化和销毁操作。 