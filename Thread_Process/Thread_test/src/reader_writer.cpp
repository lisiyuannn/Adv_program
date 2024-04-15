//读者和写者问题
//一个读者，一个写者

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

using namespace std;

typedef struct 
{
    int val;
    //读取
    pthread_cond_t rc;
    pthread_mutex_t rm;
    int r_wait;

    //写入
    pthread_cond_t wc;
    pthread_mutex_t wm;
    int w_wait;
}Storage;

//写数据函数
void set_data(Storage *, int);
//读数据函数
int get_data(Storage *);

//写线程函数
void * set_th(void *);
//读线程函数
void * get_th(void *);

int main(int argc, char const *argv[])
{
    int err;
    pthread_t rth, wth;

    Storage s;
    //初始化s
    s.r_wait = 0;
    s.w_wait = 0;
    pthread_cond_init(&(s.rc), NULL);
    pthread_cond_init(&(s.wc), NULL);
    pthread_mutex_init(&(s.rm), NULL);
    pthread_mutex_init(&(s.wm), NULL);

    //创建一个读者线程和写者线程
    if((err = pthread_create(&rth, NULL, get_th, &s) != 0))
    {
        perror("rth create failed");
        exit(1);
    }
    if((err = pthread_create(&wth, NULL, set_th, &s) != 0))
    {
        perror("wth create failed");
        exit(1);
    }

    pthread_join(rth, NULL);
    pthread_join(wth, NULL);

    //销毁锁和队列
    pthread_cond_destroy(&(s.rc));
    pthread_cond_destroy(&(s.wc));
    pthread_mutex_destroy(&(s.rm));
    pthread_mutex_destroy(&(s.wm));

    return 0;
}

//写数据函数
void set_data(Storage *s, int val)
{
    s->val = val;
}
//读数据函数
int get_data(Storage *s)
{
    return s->val;
}

//写线程函数
void * set_th(void * arg)
{
    Storage *s = (Storage *)arg;
    int i = 1;
    for(; i <= 100; i++)
    {
        usleep(100000);
        set_data(s, i+100);
        cout << "write: " << s->val << endl;
        //写入完成后看读线程是否已经准备好，然后通知他读
        pthread_mutex_lock(&(s->rm));  //对读者锁上锁
        while(!s->r_wait)
        {
            pthread_mutex_unlock(&(s->rm));
            usleep(1000000);
            pthread_mutex_lock(&(s->rm));
        }
        s->r_wait = 0;
        pthread_mutex_unlock(&(s->rm));
        pthread_cond_broadcast(&(s->rc));

        //将自己加入写着等待队列，等待被唤醒继续写
        pthread_mutex_lock(&(s->wm));  //对读者锁上锁
        //改变条件变量，然后将自己加入等待队列
        s->w_wait = 1;
        pthread_cond_wait(&(s->wc), &(s->wm));
        pthread_mutex_unlock(&(s->wm));
    }

    return NULL;
}
//读线程函数
void * get_th(void * arg)
{
    Storage *s = (Storage *)arg;
    int i = 1;
    for(; i <= 100; i++)
    {
        //判断写者是否已经将数据写好
        pthread_mutex_lock(&(s->rm));  //对读者锁上锁
        //改变条件变量，然后将自己加入等待队列
        s->r_wait = 1;
        pthread_cond_wait(&(s->rc), &(s->rm));
        //被唤醒后解锁
        pthread_mutex_unlock(&(s->rm));  //对读者锁解锁
        //读数据
        int value = get_data(s);
        usleep(100000);
        cout << "read: " << value << endl;
        cout << "---------------------------" << endl;

        //判断写者线程是否准备好
        pthread_mutex_lock(&(s->wm));
        while(!s->w_wait)
        {
            pthread_mutex_unlock(&(s->wm));
            usleep(1000000);
            pthread_mutex_lock(&(s->wm));
        }
        s->w_wait = 0;
        pthread_mutex_unlock(&(s->wm));

        pthread_cond_broadcast(&(s->wc));

    }
    return NULL;
}

