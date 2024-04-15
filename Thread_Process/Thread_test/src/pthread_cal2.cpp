//线程同步
//多个线程用作计算，一个线程去读取计算结果
//只有当一个线程计算完毕后，其余线程才能读取计算结果

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

using namespace std;

typedef struct 
{
    int res;
    int count;  //用于统计获取结果线程的个数
    pthread_cond_t cond;  //条件变量
    pthread_mutex_t mutex;  //保护等待队列的锁
}Result;

//计算并将结果放在Result中的线程运行运行函数
void * cal_func(void *arg)
{
    sleep(1);
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
    //while(!r->is_wait)
    while(r->count < 2)  //如果小于两个将进入循环，等待等待线程大于两个跳出循环
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
    //定义一个用于调用io函数的互斥锁
    pthread_mutex_t mutex_std;
    pthread_mutex_init(&mutex_std, NULL);

    //对两个线程都可以操作的is_wait加锁
    pthread_mutex_lock(&(r->mutex));
    //r->is_wait = 1;
    r->count++;

    pthread_cond_wait(&(r->cond), &(r->mutex));
    //被唤醒后
    pthread_mutex_unlock(&(r->mutex));
    int res = r->res;
    pthread_mutex_lock(&(r->mutex));
    cout << "restult: " << res << endl;
    pthread_mutex_unlock(&(r->mutex));
    return (void *)0;
}



int main(int argc, char const *argv[])
{
    int err;
    pthread_t cal, get1, get2, get3;

    Result r; //计算结构体
    r.count = 0;  //初始化判断条件
    //初始化条件变量和互斥锁
    pthread_cond_init(&(r.cond), NULL);
    pthread_mutex_init(&(r.mutex), NULL);

    if((err = pthread_create(&cal, NULL, cal_func, (void *)&r)) != 0)
    {
        perror("thread cal create failed");
        exit(1);
    }
    if((err = pthread_create(&get1, NULL, get_func, (void *)&r)) != 0)
    {
        perror("thread get create failed");
        exit(1);
    }
    if((err = pthread_create(&get2, NULL, get_func, (void *)&r)) != 0)
    {
        perror("thread get create failed");
        exit(1);
    }
    if((err = pthread_create(&get3, NULL, get_func, (void *)&r)) != 0)
    {
        perror("thread get create failed");
        exit(1);
    }

    pthread_join(cal, NULL);
    pthread_join(get1, NULL);
    pthread_join(get2, NULL);
    pthread_join(get3, NULL);

    //销毁条件变量和互斥锁
    pthread_cond_destroy(&(r.cond));
    pthread_mutex_destroy(&(r.mutex));


    return 0;
}
