//类似龟兔赛跑的程序，用来学习多线程知识
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

void * th_fn(void *arg)
{
    int distance = 50;
    for (int i = 1; i <= distance; ++i)
    {
        cout << "thread " << pthread_self() << " run " << i << " m" << endl;
        int time = (int)(drand48() * 200000);  //drand生成范围在 [0.0, 1.0) 之间的双精度浮点数
        usleep(time);
    }
    return (void *)0;
}

int main(int argc, char const *argv[])
{
    int err;  //存放线程创建时发生的错误
    pthread_t rabbit, turtle;  //定义线程标识符

    //创建 rabbit 线程 (标识符地址，属性，线程开始运行的函数地址，运行函数的参数)
    if((err = pthread_create(&rabbit, NULL, th_fn, (void*)50)) != 0)
    {
        perror("pthread_create error");
        exit(1);
    }
    //创建 turtle 线程 (标识符地址，属性，线程开始运行的函数地址，运行函数的参数)

    if((err = pthread_create(&turtle, NULL, th_fn, (void*)50)) != 0)
    {
        perror("pthread_create error");
        exit(1);
    }

    //sleep(10);  //主控线程一旦结束，所有子线程都会结束

    //主控线程调用pthread_join，自己会阻塞，直到rabbit线程结束方可运行
    pthread_join(rabbit, NULL);
    pthread_join(turtle, NULL);
    cout << "control thread id is " << pthread_self() << endl;

    
    return 0;
}
