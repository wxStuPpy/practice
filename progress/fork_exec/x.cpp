#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cstring>
#include <errno.h>

int main()
{
    std::string name = "xxx";
    std::cout << "before fork " << name << " " << getpid() << std::endl;
    pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "fork failed" << std::endl;
        return 0;
    }
    else if (pid == 0)
    {
        std::string newName = "yyy";
        std::cout<<getpid()<<" else if "<<getppid()<<std::endl;
        char * argv[] = {"./y", const_cast<char *>(newName.c_str()), NULL};
        char **envp = environ;
        int res = execve(argv[0], argv, envp);
        if (res == -1)
        {   
            std::cerr<<"失败"<<std::endl;
            return -1;
        }
        // 成功   不执行
    }
    else
    {   waitpid(pid,NULL,0);
        //pid_t waitpid(pid_t pid, int *status, int options);
// 参数说明
// pid：指定要等待的子进程，其取值有以下几种情况：
// pid > 0：等待进程 ID 为 pid 的子进程。
// pid == -1：等待任意子进程，此时 waitpid 与 wait 函数的功能类似。
// pid == 0：等待与调用进程处于同一进程组的任意子进程。
// pid < -1：等待进程组 ID 等于 pid 绝对值的任意子进程。
// status：是一个指向整数的指针，用于存储子进程的退出状态信息。如果不需要获取子进程的退出状态，可以将其设置为 NULL。通过一些宏可以从 status 中提取有用的信息，例如：
// WIFEXITED(status)：判断子进程是否正常退出。
// WEXITSTATUS(status)：如果子进程正常退出，返回其退出状态码。
// WIFSIGNALED(status)：判断子进程是否因信号而终止。
// WTERMSIG(status)：如果子进程因信号而终止，返回终止它的信号编号。
// options：是一个选项参数，用于控制 waitpid 的行为，常用的选项有：
// WNOHANG：如果没有子进程状态发生变化，立即返回而不阻塞。
// WUNTRACED：除了返回终止子进程的状态信息外，还返回因信号而暂停的子进程的状态信息。
// WCONTINUED：返回因信号 SIGCONT 恢复执行的子进程的状态信息。
        std::cout << "after exec " << getpid() << " " << pid << std::endl;
    }
    return 0;
}