#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
using namespace std;

void out_status(int);

int main(int argc, char const *argv[])
{
    int status;
    pid_t pid;
    //正常终止
    if(pid = fork() < 0)
    {
        perror("for error");
        exit(1);
    }
    else if(pid == 0)
    {//子进程代码
        cout << "pid is " << getpid() << ", ppid is " << getppid() << endl;
        exit(3);  //返回非零状态码
    }

    //父进程阻塞，等待子进程结束并回收
    sleep(1);
    wait(&status);
    
    out_status(status);  //输出状态信息
    cout << "-----------------------" << endl;

    //非正常终止
    if(pid = fork() < 0)
    {
        perror("for error");
        exit(1);
    }
    else if(pid == 0)
    {//子进程代码
        cout << "pid is " << getpid() << ", ppid is " << getppid() << endl;
        int i = 3, j = 0;
        int k = i/j;
        cout << "k= " << k << endl;
    }

    //父进程阻塞，等待子进程结束并回收
    sleep(1);
    wait(&status);
    
    out_status(status);  //输出状态信息
    cout << "-----------------------" << endl;

    //暂停后终止
    if(pid = fork() < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if(pid == 0)
    {//子进程代码
        cout << "pid is " << getpid() << ", ppid is " << getppid() << endl;
        system("pause");
    }

    //父进程阻塞，等待子进程结束并回收
    //waitpid(pid, &status, WUNTRACED);
    sleep(1);
    do
    {
        pid = waitpid(pid, &status, WNOHANG|WUNTRACED);  //等待子进程结束后返回的pid不为0时结束
        if(pid == 0) sleep(1);
    }while(pid == 0);
    out_status(status);  //输出状态信息
    cout << "-----------------------" << endl;
    return 0;
}

//状态码不可以直接输出
//需要先对其终止类型进行判断
void out_status(int status)
{
    if(WIFEXITED(status))
    {//正常终止
        cout << "normal exit: " << WEXITSTATUS(status) << endl;
    }
    else if(WIFSIGNALED(status))
    {//非正常终止
        cout << "abnormal exit: " << WTERMSIG(status) << endl;
    }
    else if(WIFSTOPPED(status))
    {//是否暂停过
        cout << "stopped sig" << WSTOPSIG(status) << endl;
    }
    else
    {
        cout << "unknown sig" << endl;
    }
}
