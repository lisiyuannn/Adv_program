//类似龟兔赛跑的程序，用来学习多线程知识
//使用结构体可以做到将多个参数封装到结构体里一起传给线程起始函数中
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

typedef struct  //线程入口函数参数结构体
{
    char name[20];
    int time;
    int start;
    int end;
}RaceArg;

void * th_fn(void *);

int main(int argc, char const *argv[])
{
    int err;
    pthread_t rabbit, turtle;

    //参数结构体赋值
    RaceArg r_a = 
        {"rabbit", (int)drand48()*200000000, 20, 50 };
    RaceArg t_a = 
        {"turtle", (int)drand48()*200000000, 10, 60 };

    //第四个参数应该传结构体指针，转换成（void*）
    if((err = pthread_create(&rabbit, NULL, th_fn, (void *)&r_a)) != 0)
    {
        perror("pthread create error");
        exit(1);
    }
    if((err = pthread_create(&turtle, NULL, th_fn, (void *)&t_a)) != 0)
    {
        perror("pthread create error");
        exit(1);
    }

    // pthread_join(rabbit, NULL);
    // pthread_join(turtle, NULL);
    // cout << "control thread id is " << pthread_self() << endl;
    

    long *result1;
    long *result2;

    pthread_join(rabbit, (void **)&result1);
    pthread_join(turtle, (void **)&result2);
    cout << "rabbit run " << ((RaceArg *)result1)->end - ((RaceArg *)result1)->start << endl;
    cout << "turtle run " << ((RaceArg *)result2)->end - ((RaceArg *)result2)->start << endl;

    return 0;
}


void * th_fn(void *arg)
{
    RaceArg *r = (RaceArg *)arg;
    int i = r->start;
    for (i; i <= r->end; ++i)
    {
        cout << r->name << "is running at " << i << endl;
        usleep(r->time);
    }
    return (void *)r;
}
