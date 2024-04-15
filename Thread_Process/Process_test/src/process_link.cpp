//进程链

#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
using namespace std;

int main(int argc, char const *argv[])
{
    int counter;
    if(argc < 2)
    {
        counter = 2;
    }
    else
    {
        counter = atoi(argv[1]);
    }

    int i = 1;
    pid_t pid;
    for(i; i < counter; ++i)
    {
        //需要保证父进程在fork一次之后下次循环不可以继续fork
        pid = fork();
        if(pid == -1)
        {
            perror("fail to fork");
            exit(1);
        }
        // else if(pid > 0)
        // {//父进程代码
        //     break; //父进程退出循环，子进程继续fork
        // }
        else if(pid == 0)
        {
            break;  //进程扇，子进程退出循环，父进程继续fork
        }
        
    }

    cout << "pid is " << getpid() << ", ppid is " << getppid() << endl;
    sleep(3);

    return 0;
}
