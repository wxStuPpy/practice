#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
    // 修改为 const char* 类型
    constexpr const char *shm_value_name = "value";
    constexpr const char *shm_sem_name = "sem";

    // 创建共享内存对象
    int value_fd = shm_open(shm_value_name, O_RDWR | O_CREAT, 0664);
    // 使用正确的共享内存名称创建信号量的共享内存对象
    int sem_fd = shm_open(shm_sem_name, O_RDWR | O_CREAT, 0664);

    // 调整共享内存对象的大小
    ftruncate(value_fd, sizeof(int));
    ftruncate(sem_fd, sizeof(sem_t));

    // 将共享内存信号量映射到共享内存区域
    int *value = static_cast<int *>(mmap(nullptr, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, value_fd, 0));
    sem_t *sem = static_cast<sem_t *>(mmap(nullptr, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, sem_fd, 0));

    // 初始化
    (*value) = 0;
    // 对于命名信号量使用 sem_open
    sem = sem_open(shm_sem_name, O_CREAT, 0664, 1);

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        sem_wait(sem);
        int tmp = *value + 1;
        sleep(1);
        *value = tmp;
        sem_post(sem);
    }
    else
    {
        sem_wait(sem);
        int tmp = *value + 1;
        sleep(1);
        *value = tmp;
        sem_post(sem);
        waitpid(pid, 0, 0);
        // 关闭命名信号量
        sem_close(sem);
        // 删除命名信号量
        sem_unlink(shm_sem_name);
        std::cout << "final value:" << *value << std::endl;
    }
    munmap(value, sizeof(int));
    munmap(sem, sizeof(sem_t));

    close(value_fd);
    close(sem_fd);

    if (pid > 0)
    {
        shm_unlink(shm_value_name);
        shm_unlink(shm_sem_name);
    }
    return 0;
}
 