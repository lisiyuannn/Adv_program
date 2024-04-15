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
