#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>  
#include "../../lib/inc/head.h"


int main(int argc, char *argv[]) {
    int a=1,b=2;
    add(a,b);
    // 调用 fork 之前，代码在父进程中运行
    std::cout << "fork 之前 " << getpid() << std::endl;

    // 利用 fork 创建子进程
    pid_t pid = fork();//y

    // fork 之后所有代码都是在父子进程中各自执行一次

    if (pid > 0) {
        std::cout << "父进程: " <<pid<<" "<< getpid() << std::endl;//x y
    } else if (pid == 0) {
        std::cout << "子进程: " <<getpid() <<" "<<pid <<":父进程为"<<getppid()<< std::endl;//x 0  y
    } else {
        // 抛出一个标准异常
        throw std::runtime_error("fork 调用失败");
    }
     
    return 0;
}