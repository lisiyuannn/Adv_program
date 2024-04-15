#include <sys/types.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
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
        int i = 0;
        for(i; i < 10; ++i)
        {
            cout << "I am parent process, pid is " << getpid() << endl;
            sleep(1);
        }
    }
    else
    {//子进程执行代码
        int i = 0;
        for(i; i < 10; ++i)
        {
            cout << "I am child process, pid is " << getpid() << endl;
            sleep(1);
        }
        
    }
    return 0;
}
