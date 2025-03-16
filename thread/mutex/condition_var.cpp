#if 0
#include <iostream>
#include <pthread.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;

// 初始化互斥锁
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// 初始化条件变量
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// 向buffer中写数据
void *producer(void *arg)
{
    int i = 1;
    while (1)
    {
        // 使用共同的变量 首先获取锁
        pthread_mutex_lock(&mutex);

        // 如果缓冲区写满 暂停当前线程
        if (count == BUFFER_SIZE)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        buffer[count++] = i++;
        std::cout << "P:" << buffer[count - 1] << std::endl;
        // 唤醒消费者
        pthread_cond_signal(&cond);
        // 最后释放锁
        pthread_mutex_unlock(&mutex);
    }
}
// 向buffer中读数据
void *consumer(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (count == 0)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        std::cout << "C:" << buffer[--count] << std::endl;
        // 唤醒生产者
        pthread_cond_signal(&cond);
        // 最后释放锁
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char *crgv[])
{
    pthread_t pid, cid;
    pthread_create(&pid, nullptr, producer, nullptr);
    pthread_create(&cid, nullptr, consumer, nullptr);

    pthread_join(pid, nullptr);
    pthread_join(cid, nullptr);
}
#endif

#include <iostream>
#include <pthread.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;

// 初始化互斥锁
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// 初始化条件变量
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// 向buffer中写数据
void *producer(void *arg)
{
    int i = 1;
     // 使用共同的变量 首先获取锁
     pthread_mutex_lock(&mutex);
    while (1)
    {

        // 如果缓冲区写满 暂停当前线程
        if (count == BUFFER_SIZE)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        buffer[count++] = i++;
        std::cout << "P:" << buffer[count - 1] << std::endl;
        // 唤醒消费者
        pthread_cond_signal(&cond);
    }
     // 最后释放锁
     pthread_mutex_unlock(&mutex);
}
// 向buffer中读数据
void *consumer(void *arg)
{  pthread_mutex_lock(&mutex);
    while (1)
    {
        if (count == 0)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        std::cout << "C:" << buffer[--count] << std::endl;
        // 唤醒生产者
        pthread_cond_signal(&cond);
    }
       // 最后释放锁
       pthread_mutex_unlock(&mutex);
}

int main(int argc, char *crgv[])
{
    pthread_t pid, cid;
    pthread_create(&pid, nullptr, producer, nullptr);
    pthread_create(&cid, nullptr, consumer, nullptr);

    pthread_join(pid, nullptr);
    pthread_join(cid, nullptr);
}