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
        cout << "I am parent process, pid is " << getpid() << ", ppid is " << getppid() << endl;
        cout << "fork return is " << pid << endl;
    }
    else
    {//子进程执行代码
        cout << "I am child process, pid is " << getpid() << ", ppid is " << getppid() << endl;
        cout << "fork return is " << pid << endl;
    }

    cout << "pid: " << getpid() << endl;
    sleep(1);

    return 0;
}
