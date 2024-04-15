#include "account.h"
#include <iostream>
#include <assert.h>
#include <string.h>
#include <unistd.h>
using namespace std;

//定义账户操作者结构体
typedef struct
{
    char name[20];
    Account *account;
    double amt;
}OperArg;

//定义线程入口函数
//取款线程
void * withdraw_fn(void *arg)
{//输入为操作者结构体  name, Account *, amt
    OperArg * oa = (OperArg *)arg;
    double balance = withdraw(oa->account, oa->amt);
    cout << "balance: " << balance << endl;
    cout << oa->name << " withdraw finshed" << endl;
    return (void *)0;
}
//存款线程
void * deposit_fn(void *arg)
{
    OperArg * oa = (OperArg *)arg;
    double balance = deposit(oa->account, oa->amt);
    cout << "balance: " << balance << endl;
    cout << oa->name << " deposit finshed" << endl;
    return (void *)0;
}
//检查银行账户的线程运行函数
void * check_fn(void *arg)
{
    return (void *)0;
}

int main(int argc, char const *argv[])
{
    int err;
    pthread_t boy, girl;
    Account *a = create_account(100001, 10000);

    OperArg o1, o2;
    strcpy(o1.name, "boy");
    o1.account = a;
    o1.amt = 10000;    
    
    strcpy(o2.name, "girl");
    o2.account = a;
    o2.amt = 10000;    
    
    //启动两个线程同时去操作同一个银行账户
    if((err = pthread_create(&boy, NULL, withdraw_fn, (void *)&o1)) != 0)
    {
        perror("pthread_create faild");
        exit(1);
    }
    if((err = pthread_create(&girl, NULL, withdraw_fn, (void *)&o2)) != 0)
    {
        perror("pthread_create faild");
        exit(1);
    }

    pthread_join(boy, NULL);
    pthread_join(girl, NULL);

    //主控线程查看账户余额
    cout << "account balance: " << get_balance(a) << endl;
    destroy_account(a);
    cout << "finshed" << endl;
    
    return 0;
}


