#include<iostream>
#include"../include/head.h"
int main(){
    auto a=10;
    int b=20;
    std::cout<<add(a,b)<<std::endl;
    #ifdef DEBUG
    std::cout<<a-b<<std::endl;
    #endif 
    return 0;
}

//g++ gcc_demo.cpp -o demo -std=c++11 -I../include -Wall -DDEBUG