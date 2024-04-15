# Adv_program
Linux c/c++高级编程
* 线程标识
    > 每个进程内部的不同线程都有自己唯一的标识（ID）
    > 线程标识只在它所属的进程环境中有效
    > 线程标识是 pthread_t 数据类型 
    ```C++
    #include <pthread.h>
    int pthread equal(pthread_t, pthread_t);
    //判断线程标识是否一致，返回：相等返回非0，否则返回0
    pthread_t pthread_self(void);
    //返回调用线程的线程ID
    ```
### 线程创建
* 语法
    ```C++
    #include <pthread.h>
    int pthread_create(T tidp, T attr, T start_rtn, T arg);
    //成功返回0，否则返回错误编号
    ```
* 参数
    > tidp: 线程标识符指针
    > 
    > attr: 线程属性指针
    > 
    > start_rtn: 线程运行函数的起始地址
    > 
    > arg: 传递给线程运行函数的参数 __可使用结构体将多个参数封装起来一起传入，传入时需要穿结构体的地址，并传换成（void*）__
* 新创建线程从 start_rtn 函数的地址开始运行
* 不能保证新线程和调用线程的执行顺序
* 代码示例
    ```C++
    //类似龟兔赛跑的程序，用来学习多线程知识
    //使用结构体可以做到将多个参数封装到结构体里一起传给线程起始函数中
    #include <pthread.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <iostream>
    #include <math.h>
    using namespace std;

    typedef struct
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

        pthread_join(rabbit, NULL);
        pthread_join(turtle, NULL);
        cout << "control thread id is " << pthread_self() << endl;
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
        return (void *)0;
    }
    ```
### 线程终止

* 主动终止
    > 线程的执行函数中调用return语句
    > 
    > 调用pthread_exit()
* 被动终止
    > 线程可以被同一进程的其他线程取消，其他线程需调用pthread_cancel(pthid)。
    > 
    > __由于一个进程中的多个线程共享数据段，因此通常在线程退出后，退出线程所占用的资源并不会随线程结束而释放。__ 阻塞等待指定线程结束，pthread_join(pthread t_th, void **thread_return); 该函数可以不用等待进程结束就释放线程资源

* __使用pthread_join的第三个参数获取线程入口函数返回值示例__
    ```c++
    //参数结构体
    typedef struct  //线程入口函数参数结构体
    {
        long d1;
        long d2;
    }Arg;
    //入口函数
    void * th_fn(void *arg)
    {
        Arg *r = (Arg *)arg;
        return (void*)(r->d1 + r->d2);  //返回两个参数的和
    }
    //接收返回值并打印
    long *result;  //用来接收线程入口函数返回结果
    pthread_join(pid, (void**)&result);
    cout << "result: " << (long)result << endl;
    return 0;

    //示例2  返回一个结构体指针
    typedef struct  //线程入口函数参数结构体
    {
        char name[20];
        int time;
        int start;
        int end;
    }RaceArg;

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

    //使用long类型是因为其大小和指针大小一致，8个字节
    long *result1;
    long *result2;

    pthread_join(rabbit, (void **)&result1);
    pthread_join(turtle, (void **)&result2);
    cout << "rabbit run " << ((RaceArg *)result1)->end - ((RaceArg *)result1)->start << endl;
    cout << "turtle run " << ((RaceArg *)result2)->end - ((RaceArg *)result2)->start << endl;
    ```

### 线程清理和终止函数

    ```C++
    #include <pthread.h>
    void pthread_cleanup_push(void (*rtn)(void *), void *arg);
    //成功返回0，否则返回错误编号
    void pthread_cleanup_pop(int execute);
    //成对出现
    ```
* 参数
    > rtn: 清理函数指针
    > 
    > arg: 调用清理函数传递的参数
    > 
    > execute: 值1时执行线程清理函数，值0时不执行线程清理函数
* 触发线程调用清理函数的动作
    > 调用pthread_exit
    > 
    > 相应取消请求
    > 
    > 用非零execute参数调用pthread_cleanup_pop

* 代码示例
    ```c++
    #include <iostream>
    #include <pthread.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <string.h>
    using namespace std;

    //定义线程清理函数
    void clean_func(void *arg)
    {
        char *s = (char *)(arg);
        cout << "clean_func: " << s << endl;
    }
    //定义线程入口函数
    void* th_func(void *arg)
    {
        long execute = (long)arg;
        //char *s1 = "first cleanup func";
        //char *s2 = "second cleanup func";
        pthread_cleanup_push(clean_func, (char *)("first cleanup func"));
        pthread_cleanup_push(clean_func, (char *)("second cleanup func"));
        cout << "thread " << pthread_self() << " is running" << endl;
        pthread_cleanup_pop(execute);
        pthread_cleanup_pop(execute);

        return (void *)0;
    }


    int main(int argc, char const *argv[])
    {
        //创建两个线程
        int err;
        pthread_t th_1, th_2;
        err = pthread_create(&th_1, NULL, th_func, (void *)0);
        if(err == -1)
        {
            perror("th_1 create faild");
            exit(1);
        }
        pthread_join(th_1, NULL);
        cout << "th_1 finshed" << endl;
        err = pthread_create(&th_2, NULL, th_func, (void *)1);
        if(err == -1)
        {
            perror("th_2 create faild");
            exit(1);
        }
        pthread_join(th_2, NULL);
        cout << "th_2 finshed" << endl;

        return 0;
    }
    ```
### 线程的属性

* 线程的属性初始化、销毁、设置
    ```c++
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

    ```
### 线程的同步和互斥

* 线程同步
    > 是一个宏观概念，在微观上包含线程的相互排斥和线程先后执行的约束问题
    > 
    > 解决同步方式
    >> 条件变量
    >> 线程信号量
* 线程互斥
    > 线程执行的相互排斥
    > 
    > 解决互斥方式
    >> 互斥锁
    >> 读写锁
    >> 线程信号量
* 互斥锁的创建和销毁
    ```c++
    #include <pthread>
    pthread_mutex_t mutex;
    //锁
    pthread_mutex_init(&(a->mutex), NULL);
    //互斥锁初始化
    pthread_mutex_destroy(&(a->mutex));
    //销毁互斥锁
    pthread_mutex_lock(&(a->mutex));
    //上锁
    pthread_mutex_unlock(&(a->mutex));
    //解锁
    ```
* 互斥锁的属性
> 先初始化属性，再初始化锁
> pthread_mutex_t mutex;
> pthread_mutexattr_t mutexattr;
> pthread_mutexattr_init(&mutexattr);
> //中间可以对属性进行修改
> pthread_mutex_init(&mutex, &mutexattr);
> //销毁
> pthread_mutex_destroy(&mutex);
> pthread_mutexattr_destroy(&mutexattr);

* 属性创建
```c++
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

```

* 读写锁
* 创建和销毁
```c++
    pthread_rwlock_t rwlock;
    pthread_rwlock_init(&rwlock, NULL);
    pthread_rwlock_destroy(&rwlock);
```
* 加锁和解锁
```c++
    pthread_rwlock_rdlock(&rwlock);
    pthread_rwlock_wrlock(&rwlock);
    pthread_rwlock_unlock(&rwlock);
    //成功返回0，出错返回错误编号
```
* 加锁先后顺序问题
> 先读后读  成功
> 先读后写  阻塞
> 先写后读  失败
> 先写后写  失败

* 代码示例
```C++
    #include <pthread.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <iostream>
    #include <string.h>

    using namespace std;

    int main(int argc, char const *argv[])
    {
        if (argc < 3)
        {
            cout << "arg error" << "[r|w] [r|w]" << endl;
        }

        //读写锁初始化
        pthread_rwlock_t rwlock;
        pthread_rwlock_init(&rwlock, NULL);

        if(!strcmp(argv[1], "r"))
        {
            //上读锁
            if(pthread_rwlock_rdlock(&rwlock) != 0)
            {
                //上锁失败
                cout << "first lock failed" << endl;
            }
            else
            {
                cout << "first lock success" << endl;
            }
        }
        else if(!strcmp(argv[1], "w"))
        {
            //上写锁
            if(pthread_rwlock_wrlock(&rwlock) != 0)
            {
                //上锁失败
                cout << "first lock failed" << endl;
            }
            else
            {
                cout << "first lock success" << endl;
            }
        }
        else cout << "first lock failed, arg error" << endl;

        if(!strcmp(argv[2], "r"))
        {
            //上读锁
            if(pthread_rwlock_rdlock(&rwlock) != 0)
            {
                //上锁失败
                cout << "second lock failed" << endl;
            }
            else
            {
                cout << "second lock success" << endl;
            }
        }
        else if(!strcmp(argv[2], "w"))
        {
            //上写锁
            if(pthread_rwlock_wrlock(&rwlock) != 0)
            {
                //上锁失败
                cout << "second lock failed" << endl;
            }
            else
            {
                cout << "second lock success" << endl;
            }
        }
        else cout << "second lock failed, arg error" << endl;

        pthread_rwlock_unlock(&rwlock);
        pthread_rwlock_unlock(&rwlock);
        return 0;
    }
```
* 条件变量的创建的销毁
```c++
#include <pthread.h>
pthread_cond_t cond;
pthread_cond_init(&cond, NULL);
pthread_cond_destroy(&cond);
//成功返回0，失败返回错误编码
```
* 等待队列
```c++
pthread_cond_wait(&(r->cond), &(r->mutex));
//该所用于对等待队列加锁

pthread_cond_broadcast(&(r->cond));
//唤醒队列中正在等待的线程

int pthread_cond_wait(cond, mutex)
{//等待队列实现流程
    unlock(&mutex);
    lock(&mutex);
    将线程自己插入到条件变量的等待队列中
    unlock(&mutex);
    将当前线程阻塞，等待其他线程对其进行唤醒
    唤醒后，lock(&mutex);
    从等待队列中删除线程自己
}
//因此，想要加入等待队列的线程要先获取锁，然后调用wait函数，结束后释放锁
```
* 同步代码示例
```c++
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
```
### 信号量
* 信号量的创建销毁和使用
```c++
#include <semaphore.h>
sem_t s;
sem_init(&s, 0, 0); //0:是否在其他进程共享，0：sem初始值
sem_post(&s);  //增加信号量的值
sem_wait(&s);  //减少信号量的值，当信号量为0时，阻塞
sem_trywait(&s);  //同上，不会阻塞
sem_destroy(&s);
```
* 信号量代码示例
```c++
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
```
