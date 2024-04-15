#pragma once

class Account
{
public:
    int code;
    Account (int , double);
    ~Account ();

    //取款
    double withdraw(Account *, double);
    //存款
    double deposit(Account *, double);
    //查看账户余额
    double get_banlance(Account *);

private:
    double balance;
};
