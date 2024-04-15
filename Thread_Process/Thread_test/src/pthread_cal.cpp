//线程同步
//两个线程，一个线程用作计算，另一个线程去读取计算结果
//只有当一个线程计算完毕后，另一个线程才能读取计算结果

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

using namespace std;

typedef struct 
{
    int res;
    int is_wait;  //判断条件，由用户给出
    pthread_cond_t cond;  //条件变量
    pthread_mutex_t mutex;  //保护等待队列的锁
}Result;

//计算并将结果放在Result中的线程运行运行函数
void * cal_func(void *arg)
{
    Result * r = (Result *)arg;
    int i = 1;
    int sum = 0;
    for (; i <= 100; ++i)
    {
        sum += i;
    }

    r->res = sum;

    //对两个线程都可以操作的is_wait加锁
    pthread_mutex_lock(&(r->mutex));
    //判断获取结果的线程是否准备好
    while(!r->is_wait)
    {//如果还没准备好就睡眠一会
        pthread_mutex_unlock(&(r->mutex));
        usleep(10000);
        pthread_mutex_lock(&(r->mutex));
    }
    pthread_mutex_unlock(&(r->mutex));

    //唤醒正在等待的线程
    pthread_cond_broadcast(&(r->cond));
    return (void *)0;
}
void * get_func(void *arg)
{
    Result *r = (Result *)arg;

    //对两个线程都可以操作的is_wait加锁
    pthread_mutex_lock(&(r->mutex));
    r->is_wait = 1;

    pthread_cond_wait(&(r->cond), &(r->mutex));
    //被唤醒后
    pthread_mutex_unlock(&(r->mutex));
    int res = r->res;
    cout << "restult: " << res << endl;
    return (void *)0;
}



int main(int argc, char const *argv[])
{
    int err;
    pthread_t cal, get;

    Result r; //计算结构体
    r.is_wait = 0;  //初始化判断条件
    //初始化条件变量和互斥锁
    pthread_cond_init(&(r.cond), NULL);
    pthread_mutex_init(&(r.mutex), NULL);

    if((err = pthread_create(&cal, NULL, cal_func, (void *)&r)) != 0)
    {
        perror("thread cal create failed");
        exit(1);
    }
    if((err = pthread_create(&get, NULL, get_func, (void *)&r)) != 0)
    {
        perror("thread get create failed");
        exit(1);
    }

    pthread_join(cal, NULL);
    pthread_join(get, NULL);

    //销毁条件变量和互斥锁
    pthread_cond_destroy(&(r.cond));
    pthread_mutex_destroy(&(r.mutex));


    return 0;
}
