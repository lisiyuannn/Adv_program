//子进程会将父进程的内存空间复制一份，他们拥有相同的虚拟地址，但各自映射到不同的物理地址上

#include <sys/types.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int g_v = 30;

int main(int argc, char const *argv[])
{
    int l_v = 30;
    static int s_v = 30;

    cout << "pid: " << getpid() << endl;

    pid_t pid;
    pid = fork();
    if(pid == -1)
    {
        perror("fail to fork");
        exit(1);
    }
    else if(pid > 0)
    {//父进程执行代码
        g_v = 40; l_v = 40; s_v = 40;
        sleep(5);
        cout << "I am parent process, pid is " << getpid() << endl;
        cout << "g_v: " << &g_v << endl;
        cout << "l_v: " << &l_v << endl;
        cout << "s_v: " << &s_v << endl;
    }
    else
    {//子进程执行代码
        g_v = 50; l_v = 50; s_v = 50;
        cout << "I am child process, pid is " << getpid() << endl;
        cout << "g_v: " << &g_v << endl;
        cout << "l_v: " << &l_v << endl;
        cout << "s_v: " << &s_v << endl;
    }

    //父子进程共同执行代码
    cout << "g_v = " << g_v << endl;
    cout << "l_v = " << l_v << endl;
    cout << "s_v = " << s_v << endl;

    return 0;
}
