//父子进程对文件操作相关内容

#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
using namespace std;

int g_v = 30;

int main(int argc, char const *argv[])
{
    int l_v = 30;
    static int s_v = 30;

    cout << "pid: " << getpid() << endl;

    //父进程调用
    FILE *fp = fopen("test.txt", "w");  //基于缓存功能，在堆区
    int fd = open("test_fd.txt", O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IRWXG);  //不带缓存功能，直接写入
    char s[128] = "hello fork";
    ssize_t size = strlen(s) * sizeof(char);

    fprintf(fp, "s: %s, pid: %d\n", s, getpid());   //基于缓存功能，在堆区
    write(fd, s, size);   //不带缓存功能，直接写入

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
    // cout << "g_v = " << g_v << endl;
    // cout << "l_v = " << l_v << endl;
    // cout << "s_v = " << s_v << endl;
    fprintf(fp, "pid: %d, g_v: %d, l_v: %d, s_v: %d\n", getpid(), g_v, l_v, s_v);
    
    fclose(fp);
    close(fd);
    sleep(1);
    return 0;
}
