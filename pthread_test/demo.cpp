#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

// int main(int argc, char const *argv[])
// {
//     cout << "程序开始：" << endl;
//     cout << "*****************" << endl;

//     int tag = 10000;
//     string str = "孙悟空";

//     pid_t id = fork();  //创建子进程，该行后的代码父进程和子进程都会执行
    
//     if(id > 0)   //父进程的返回值为子进程pid
//     {//父进程会执行这段代码
//         sleep(1);
//         cout << "这是父进程。。。" << endl;
//         cout << "变量地址为：" << &tag << endl;   //虚拟地址一样，但是物理地址并不在一块
//         cout << tag << '\t' << str << endl;
//     }
//     else
//     {//子进程会执行这段代码
//         tag = 1; str = "唐僧";   //子进程会将父进程的内存复制一份，虽然地址看起来一样，但是子进程对数据的修改不会影响父进程
//         cout << "这是子进程。。。" << endl;
//         cout << "变量地址为：" << &tag << endl;
//         cout << tag << '\t' << str << endl;
//     }

//     cout << "程序结束" << endl << "*****************" << endl;

//     return 0;
// }

void func(int signum)
{
    int sts;   //将该变量的地址作为参数传入wait函数，用于存放进程退出的信息
    pid_t pid = wait(&sts);  //父进程通过wait()函数等待子进程结束，在结束之前，父进程将会被阻塞
    cout << "子进程已终止，编号为：" << pid << endl;
    //sleep(100);
    if (WIFEXITED(sts))  //正常终止，该函数返回真
    {//正常退出
        cout << "正常退出，退出状态是：" << WEXITSTATUS(sts) << endl;  //获取终止状态
    }
    else
    {//异常退出
        cout << "异常退出,退出状态是；" << WEXITSTATUS(sts) << endl;
        cout << "终止他的信号是；" << WTERMSIG(sts) << endl;
    }
}

int main(int argc, char const *argv[])
{
    pid_t pid = fork();  //创建子进程

    signal(SIGCHLD, func);  //捕获子进程退出的信号

    if(pid >0)
    {//父进程执行
        while(1)
        {//若父进程比子进程先结束，则子进程被进程1接管
            cout << "父进程开始执行" << endl;  
            //cout << "父进程结束" << endl;  
            //return 0;
            sleep(1);
        }
        
    }
    else
    {//子进程执行
        cout << "子进程开始执行" << endl;
        int *p = 0; *p = 10;  //访问空指针，异常退出
        //exit(0);  //子进程在这里结束，没有进行资源回收，成为僵尸进程
        cout << "子进程结束" << endl;
    }
    return 0;
}

