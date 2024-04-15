//在父进程收到终止信号时，跳转到父进程的终止函数，可以在该函数内部回收资源
//并且通知所有子进程结束，防止产生孤儿进程

#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

//该代码测试父进程和子进程收到信号的终止情况
//父进程终止时应通知所有子进程全部终止，子进程终止时可以只有自己终止

void FathEXIT(int);  //父进程终止函数
void ChldEXIT(int);  //子进程终止函数

int main(int argc, char const *argv[])
{
    //忽略全部信号，不希望被打扰
    for (int i = 1; i <= 64; ++i)
    {
        signal(i, SIG_IGN);  //忽略i信号
    }

    //设置信号，在shell状态下可用“kill pid”或“ctrl + c”终止进程
    //不要使用信号9强行终止，因为该信号不能被捕获和忽略
    signal(SIGTERM, FathEXIT);  //信号15，终止信号，转到函数FathEXIT
    signal(SIGINT, FathEXIT);  //信号2，键盘中断信号，转到函数FathEXIT

    while(1)
    {
        if (fork() > 0)  //每隔5秒创建一个子进程
        {
            sleep(5); continue;
        }
        else
        {//子进程执行
            signal(SIGTERM, ChldEXIT);  //收到中断信号后跳转至ChldEXIT函数
            signal(SIGINT, SIG_IGN);  //子进程不需要捕获键盘中断信号

            while(1)  //子进程在这里会被阻塞，如果不阻塞，子进程将进入外层while循环接着创建子进程
            {
                sleep(3);
                cout << "子进程" << getpid() << "正在运行" << endl;
            }
        }
    }
    return 0;
}

void ChldEXIT(int signum)
{
    //防止信号处理函数在执行过程中再次被信号中断
    signal (SIGINT, SIG_IGN); signal(SIGTERM, SIG_IGN);

    cout << "子进程" << getpid() << "已经结束。" << endl;

    //添加释放子进程资源的代码
    exit(0);
}

void FathEXIT(int signum)
{
    //防止信号处理函数在执行过程中再次被信号中断
    signal (SIGINT, SIG_IGN); signal(SIGTERM, SIG_IGN);

    cout << "父进程" << getpid() << "已经结束。" << endl;

    kill(0, SIGTERM);  //向全部子进程发出信号，让他们退出

    //添加释放资源代码
    
    exit(0);
}
