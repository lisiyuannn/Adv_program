//pthread_join(pthread t_th, void **thread_return);
//调用者阻塞等待指定线程结束
//该函数可以不用等待进程结束就释放线程资源
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

typedef struct  //线程入口函数参数结构体
{
    long d1;
    long d2;
}Arg;

void * th_fn(void *arg)
{
    Arg *r = (Arg *)arg;
    return (void*)(r->d1 + r->d2);  //返回两个参数的和
}

int main(int argc, char const *argv[])
{
    Arg r = {20, 20};
    pthread_t pid;
    int err;

    err = pthread_create(&pid, NULL, th_fn, (void *)&r);
    if(err == -1)
    {
        perror("pthread_create faild");
        exit(1);
    }

    long *result;  //用来接收线程入口函数返回结果
    pthread_join(pid, (void**)&result);
    cout << "result: " << (long)result << endl;
    return 0;
}
