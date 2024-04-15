/*
#include <unistd.h>
    #include <sys/types.h>
    pid_t getpid(void);
    //获取当前进程ID
    uid_t getuid(void);
    //获取当前进程的实际用户ID
    uid_t geteuid(void);
    //获取当前进程的有效用户ID
    gid_t getgid(void);
    //获得当前进程的用户组ID
    pid_t getppid(void);
    //获得当前进程的父进程ID
    pid_t getpgrp(void);
    //获得当前进程所在的进程组ID
    pid_t getpgid(void);
    //获得进程ID为pid的进程所在的进程组ID
*/

#include <unistd.h>
#include <stdio.h>
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    cout << "pid: " << getpid() << endl;
    cout << "ppid: " << getppid() << endl;  //获得当前进程的父进程ID
    cout << "uid: " << getuid() << endl;  //获取当前进程的实际用户ID
    cout << "euid: " << geteuid() << endl;  //获取当前进程的有效用户ID
    cout << "user gid: " << getgid() << endl;  //获得当前进程的  用户组ID
    cout << "gid: " << getpgrp() << endl;  //获得当前进程所在的  进程组ID
    cout << "pgid: " << getpgid(getpid()) << endl;  //获得进程ID为pid的进程所在的  进程组ID
    cout << "ppgid: " << getpgid(getppid()) << endl;  //获得进程的父进程进程所在的 进程组ID
    return 0;
}
