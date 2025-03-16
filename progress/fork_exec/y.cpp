#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <stdexcept>  
#include <string>

int main(int argc,char *argv[]){
    if(argc<2){
        return 1;
    }
    std::cout<<argv[1]<<" 子 "<<getpid()<<std::endl;
    return 0;
}