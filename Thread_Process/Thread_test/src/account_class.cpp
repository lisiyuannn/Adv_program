#include "account_class.h"
#include <iostream>
#include <assert.h>
#include <string.h>
#include <unistd.h>
using namespace std;




//构造函数
Account::Account(int code, double balance)
{
    Account *a = (Account *)malloc(sizeof(Account));
    assert(a != NULL);  //检查条件，如果不满足，会返回诊断信息，并且停止运行
    //初始化账户
    a->code = code;
    a->balance = balance;
}

//析构函数
Account::~Account()
{
}

//取款
double Account::withdraw(Account *a, double amt)
{//取款成功，返回取款后余额
    assert(a != NULL);
    //先对余额进行判断
    if(amt < 0 || amt > a->balance)
    {
        cout << "withdraw faild" << endl;
        return 0.0;
    }
    //取款
    double balance = a->balance;
    sleep(1);  //模拟数据库进行操作的时间
    balance -= balance - amt;
    a->balance = balance;
    return a->balance;
}
//存款
double Account::deposit(Account *a, double amt)
{//返回存款后的余额
    assert(a != NULL);
    if(amt < 0)
    {
        cout << "deposit faild" << endl;
        return a->balance;
    }
    double balance = a->balance;
    sleep(1);  //模拟数据库进行操作的时间
    balance += amt;
    a->balance = balance;
    return a->balance;
}
//查看账户余额
double Account::get_banlance(Account *a)
{
    assert(a != NULL);
    return a->balance;
}
