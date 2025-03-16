#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <cstring>

static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void *add(void *arg)
{
    int *p = static_cast<int *>(arg);
    for (int i = 0; i < 100000; ++i)
    {
        pthread_mutex_lock(&counter_mutex);
        (*p)++;
        pthread_mutex_unlock(&counter_mutex);
    }
    return nullptr;
}

int main()
{
    pthread_t tid1, tid2;
    int num = 0;
    pthread_create(&tid1, nullptr, add, &num);
    pthread_create(&tid2, nullptr, add, &num);

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);

    std::cout << num << std::endl;
    pthread_mutex_destroy(&counter_mutex);
}