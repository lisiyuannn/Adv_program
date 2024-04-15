#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__
#include <pthread.h>
typedef struct
{
    int code;
    double balance;

    //定义一把互斥锁，用来对共享资源进行加锁
    /*
    建议互斥锁用来锁定要给账户，和账户绑定在一起
    尽量不要设置成全局变量，否则可能出现一把锁去锁几百个账户
    导致并发性能降低
    */
    pthread_mutex_t mutex;
}Account;

//创建账户  返回账户结构体指针
extern Account* create_account(int code, double balance);
//销毁账户
extern void destroy_account(Account *a);
//取款
extern double withdraw(Account *a, double amt);
//存款
extern double deposit(Account *a, double amt);
//查看账户余额
extern double get_balance(Account *a);

#endif