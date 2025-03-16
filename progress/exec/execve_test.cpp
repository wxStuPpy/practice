#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <cstdlib>

extern char **environ; // 声明外部环境变量数组

int main()
{
    // 跳转之前
    std::string name = "111 ";
    std::cout << "跳转之前: " << name << getpid() << std::endl;

    // 执行跳转
    const char *path = "./test";
    // 准备参数数组
    //const_cast<char*>a //去除const来传参
    //static_cast<int>b
    //dynamic_cast<基类指针>
    char *argv[] = {const_cast<char*>(path), const_cast<char*>(name.c_str()), NULL};
    // 使用当前进程的环境变量
    char **envp = environ;
    // extern int execve (const char *__path, char *const __argv[],  char *const __envp[]) __THROW __nonnull ((1, 2));
    int ret = execve(path, argv, envp);
    //跳转后保留进程号
    if (ret == -1)
    {
        std::cerr << "execve 失败: " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}