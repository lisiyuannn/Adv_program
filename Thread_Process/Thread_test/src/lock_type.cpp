//互斥锁的类型
//属性创建
/*
//进程共享属性
pthread_mutexattr_getpshared();
pthread_mutexattr_setpshared();
//成功返回0，出错返回错误编号
//默认情况只能用于一个进程内部的线程进行互斥

//互斥锁类型操作
pthread_mutexattr_gettype();
pthread_mutexattr_settype();
//成功返回0，出错返回错误编号
//标准互斥锁：PTHREAD_MUTEX_NORMAL
//递归互斥锁：PTHREAD_MUTEX_RECURSIVE
//检错互斥锁：PTHREAD_MUTEX_ERRORCHECK
//默认互斥锁：PTHREAD_MUTEX_DEFAULT
*/

#include <iostream>
#include <assert.h>
#include <string.h>
#include <unistd.h>
using namespace std;

int main(int argc, char const *argv[])
{
    if(argc < 2)
    {
        cout << "arg error: " << "[error|norma|recursive]" << endl;
    }

    pthread_mutex_t mutex;

    //定义互斥锁属性
    pthread_mutexattr_t mutexattr;
    //初始化互斥锁属性
    pthread_mutexattr_init(&mutexattr);

    if(!strcmp(argv[1], "error"))
    {
        //设置互斥锁类型
        pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK);
        //不会阻塞，会出错
    }
    else if(!strcmp(argv[1], "normal"))
    {
        pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_NORMAL);
        //会阻塞
    }
    else if(!strcmp(argv[1], "recursive"))
    {
        pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
        //会计数
    }
    else
    {
        cout << "type error" << endl;
        exit(1);
    }


    //开始上锁
    //初始化锁
    pthread_mutex_init(&mutex, &mutexattr);
    //上锁
    if(pthread_mutex_lock(&mutex) == 0)
    {
        cout << "lock success" << endl;
    }
    else
    {
        cout << "lock failed" << endl;
    }

    //连续上锁两次
    if(pthread_mutex_lock(&mutex) == 0)
    {
        cout << "lock success" << endl;
    }
    else
    {
        cout << "lock failed" << endl;
    }

    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex);

    //锁，锁属性的销毁
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mutexattr);
    return 0;
}
