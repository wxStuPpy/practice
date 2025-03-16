#include <iostream>
#include <pthread.h>
#include <semaphore.h>

int num = 0;

sem_t unnamed_sem;

void *fun(void *)
{  //信号量互斥
    sem_wait(&unnamed_sem);
    for (int i = 0; i < 10000; ++i)
        num++;
    //信号量唤醒
    sem_post(&unnamed_sem);
    return nullptr;
}

int main()
{
    if (sem_init(&unnamed_sem, 0, 1) == -1)
    {
        perror("sem_init");
    }
    pthread_t pid1, pid2;
    pthread_create(&pid1, nullptr, fun, nullptr);
    pthread_create(&pid2, nullptr, fun, nullptr);

    pthread_join(pid1, nullptr);
    pthread_join(pid2, nullptr);

    //销毁信号量
    sem_destroy(&unnamed_sem);

    std::cout << num << std::endl;

    return 0;
}