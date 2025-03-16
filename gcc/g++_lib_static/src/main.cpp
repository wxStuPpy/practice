#include"../inc/head.h"

int main(){
    int a=1;
    int b=2;
    std::cout<<add(a,b)<<" "<<sub(a,b)<<std::endl;
} 

//g++ main.cpp -o main -L../lib -lmath -I../inc -std=c++11