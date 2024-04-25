#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

//信号处理函数
void func(int signum)
{
    cout << "收到了信号：" << signum << endl;
    signal(signum, SIG_DFL);  //恢复处理信号方法为默认行为
}

void func1(int signum)
{
    cout << "闹钟响了！" << endl;
    alarm(3);   //3秒后再次发送闹钟信号
}

int main(int argc, char const *argv[])
{

    signal(1, func);  //如果收到信号1 SIGHUP，就执行func函数   信号可以用编号也可以用信号名，增强代码可读性
    signal(15, func);  //如果收到信号15，就执行func函数
    signal(2, SIG_IGN);  //表示忽略信号2
    signal(1, func);
    //signal(9, func);  //代码无效，信号9不能被捕获
    //signal(9, SIG_IGN);  //代码无效，信号9不能被忽略

    alarm(3);  //闹钟信号，编号为14
    signal(14, func1);


    while (1)
    {
        cout << "执行了一次任务。" << endl;
        sleep(1);
    }
    return 0;
}
