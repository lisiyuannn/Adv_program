//使用两个信号量控制三个线程的先后执行顺序
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

//定义信号量
sem_t sem1, sem2;

//线程入口函数
void * th_func1(void * arg)
{
    sem_wait(&sem2);
    cout << (long)arg << endl;
    return NULL;
}
void * th_func2(void * arg)
{
    sem_wait(&sem1);
    cout << (long)arg << endl;
    sem_post(&sem2);
    return NULL;
}
void * th_func3(void * arg)
{
    cout << (long)arg << endl;
    sem_post(&sem1);
    return NULL;
}

int main(int argc, char const *argv[])
{
    //初始化，参数为地址，是否不同进程共享，初始值
    sem_init(&sem1, 0, 0);  
    sem_init(&sem1, 0, 0);

    //开启三个线程
    int err;
    pthread_t th1, th2, th3;

    if((err = pthread_create(&th1, NULL, th_func1, (void *)1)) != 0)
    {
        perror("th1 create failed");
        exit(1);
    }
    if((err = pthread_create(&th2, NULL, th_func2, (void *)2)) != 0)
    {
        perror("th2 create failed");
        exit(1);
    }
    if((err = pthread_create(&th3, NULL, th_func3, (void *)3)) != 0)
    {
        perror("th3 create failed");
        exit(1);
    }

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return 0;
}
