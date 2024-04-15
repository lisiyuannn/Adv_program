#include "account.h"
#include <iostream>
#include <assert.h>
#include <string.h>
#include <unistd.h>
using namespace std;


//创建账户  返回账户结构体指针
Account* create_account(int code, double balance)
{
    Account *a = (Account *)malloc(sizeof(Account));
    assert(a != NULL);  //检查条件，如果不满足，会返回诊断信息，并且停止运行
    //初始化账户
    a->code = code;
    a->balance = balance;
    //对互斥锁进行初始化
    pthread_mutex_init(&(a->mutex), NULL);
    return a;
}
//销毁账户
void destroy_account(Account *a)
{
    assert(a != NULL);
    //销毁互斥锁
    pthread_mutex_destroy(&(a->mutex));
    free(a);
}
//取款
double withdraw(Account *a, double amt)
{//取款成功，返回取款后余额
    assert(a != NULL);
    //从上锁资源开始到释放，称为临界区
    //进入函数后就开始上锁
    pthread_mutex_lock(&(a->mutex));
    //先对余额进行判断
    if(amt < 0 || amt > a->balance)
    {
        cout << "withdraw faild" << endl;
        //返回之前也要释放锁
        pthread_mutex_unlock(&(a->mutex));
        return 0.0;
    }
    //取款
    double balance = a->balance;
    sleep(1);  //模拟数据库进行操作的时间
    balance = balance - amt;
    a->balance = balance;
    //操作完毕后释放锁
    pthread_mutex_unlock(&(a->mutex));
    return a->balance;
}
//存款
double deposit(Account *a, double amt)
{//返回存款后的余额
    assert(a != NULL);
    //进入函数后就开始上锁
    pthread_mutex_lock(&(a->mutex));
    if(amt < 0)
    {
        cout << "deposit faild" << endl;
        //操作完毕后释放锁
        pthread_mutex_unlock(&(a->mutex));
        return a->balance;
    }
    double balance = a->balance;
    balance += amt;
    a->balance = balance;
    //操作完毕后释放锁
    pthread_mutex_unlock(&(a->mutex));
    return a->balance;
}
//查看账户余额
double get_balance(Account *a)
{
    assert(a != NULL);
    //进入函数后就开始上锁
    pthread_mutex_lock(&(a->mutex));
    long balance = a->balance;
    //操作完毕后释放锁
    pthread_mutex_unlock(&(a->mutex));
    return balance;
}
