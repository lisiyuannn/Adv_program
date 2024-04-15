//   ```C++
//     #include <pthread.h>
//     void pthread_cleanup_push(void (*rtn)(void *), void *arg);
//     //成功返回0，否则返回错误编号
//     void pthread_cleanup_pop(int execute);
//     //成对出现
//     ```
// * 参数
//     > rtn: 清理函数指针
//     > 
//     > arg: 调用清理函数传递的参数
//     > 
//     > execute: 值1时执行线程清理函数，值0时不执行线程清理函数
// * 触发线程调用清理函数的动作
//     > 调用pthread_exit
//     > 
//     > 相应取消请求
//     > 
//     > 用非零execute参数调用pthread_cleanup_pop

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
