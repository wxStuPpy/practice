#include<cstdio>
#include<cstdlib>
#include<signal.h>
#include<unistd.h>
#include<iostream>

void sigint_hander(int signum){
    std::cout<<"exit\n";
    exit(EXIT_SUCCESS);
}

int main(int argc,char *argv[]){
    if(signal(SIGINT,sigint_hander)==SIG_ERR){
        perror("signal");
        return 1;
    }
    while(1){
        sleep(1);
        std::cout<<"1"<<std::endl;
    }
}