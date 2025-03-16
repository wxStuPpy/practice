#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstring>
#include <string>

int main(int argc, char *argv[])
{
    // 1.create a shared memory object
    std::string shm_name = "./letter";
    int fd = shm_open(shm_name.c_str(), O_RDWR | O_CREAT, 0664);
    if (fd < 0)
    {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    // 2.Set shared memory object size
    ftruncate(fd, 1024);

    // 3.Memory mapping
    char *shared = static_cast<char *>(mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if (shared == MAP_FAILED)
    {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // 4.Closing the fd connection after mapping is not releasing
    close(fd);
    // 5.Use memory to map communication between processes
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        strcpy(shared, "子进程");
        std::cout << getpid() << "allala" << std::endl;
    }
    else
    {
        waitpid(pid, NULL, 0);
        std::cout << getpid() << "ni" << pid << "hao" << shared << std::endl;
    }
    // 6.Release mapping area
    int res = munmap(shared, 1024);
    if (res == -1)
    {
        perror("mummap");
        exit(EXIT_FAILURE);
    }
    // 7.relieve the shared memory object  父子进程映射数均-1
    return 0;
}