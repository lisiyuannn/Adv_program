//线程的属性初始化、销毁、设置
//获得线程的分离属性
/*
int pthread_attr_getdetachstat(const pthread_attr_t *restrict attr, int *detachstate);
int pthread_attr_setdetachstat(const pthread_attr_t *attr, int detachstate);
//成功返回0，出错返回错误编号
*/
//detachstate取值
//PTHREAD_CREATE_JOINABLE (默认值)  正常启动线程 使用pthread_join函数释放资源
//PTHREAD_CERATE_DETACHED   以分离状态启动线程 自动释放资源 无法使用pthread_join函数
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
using namespace std;

//声明分离状态输出函数
void stat_out(const pthread_attr_t *);
//声明线程入口函数
void * th_func(void *);


int main(int argc, char const *argv[])
{
    int err;
    pthread_t default_th, detach_th;
    //定义线程属性
    pthread_attr_t attr;
    //线程属性初始化
    pthread_attr_init(&attr);
    

    //以默认属性创建线程
    //输出分离属性
    stat_out(&attr);
    if((err = pthread_create(&default_th, &attr, th_func, (void*)0)) == -1)
    {
        perror("default_th create faild");
        exit(1);
    }

    long result;
    if((err = pthread_join(default_th, (void**)&result)) != 0)  //需要对void**进行一次寻址，然后写入
    {
        perror("pthread_join error");
    }
    else
    {
        cout << "default_th return :" << (long)result << endl;

    }
    cout << "-----------------------------------------------" << endl;
    

    //以分离属性创建线程
    //设置分离属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    //输出分离属性
    stat_out(&attr);
    if((err = pthread_create(&detach_th, &attr, th_func, NULL)) == -1)
    {
        perror("default_th create faild");
        exit(1);
    }

    if((err = pthread_join(detach_th, (void**)&result)) != 0)
    {
        perror("pthread_join error");
    }
    else
    {
        cout << "detach_th return :" << (long)result << endl;

    }
    cout << "-----------------------------------------------" << endl;

    //销毁线程属性
    pthread_attr_destroy(&attr);
    cout << pthread_self() << " is finshed" << endl;
    sleep(1);

    return 0;
}

//声明分离状态输出函数
void stat_out(const pthread_attr_t * attr)
{
    int state;
    if((pthread_attr_getdetachstate(attr, &state)) != 0)
    {
        perror("getdetachstate error");
    }
    else
    {
        if(state == PTHREAD_CREATE_JOINABLE)
        {
            cout << "joinable state" << endl;
        }
        else if(state == PTHREAD_CREATE_DETACHED)
        {
            cout << "detached state" << endl;
        }
        else
        {
            cout << "error state" << endl;
        }
    }
}
//声明线程入口函数
void * th_func(void *)
{
    long sum = 0;
    for (int i = 1; i <= 100; i++)
    {
        sum += i;
    }
    return (void*)sum;
}
