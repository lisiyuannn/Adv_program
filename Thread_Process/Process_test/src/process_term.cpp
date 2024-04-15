//打开一个文件，看多个终止方式能否对文件写入信息，并且观察终止函数的调用顺序

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

//定义进程的终止函数
void term_fun1(void)
{
    cout << "first term function" << endl;
}
void term_fun2(void)
{
    cout << "second term function" << endl;
}
void term_fun3(void)
{
    cout << "third term function" << endl;
}


int main(int argc, char const *argv[])
{
    if(argc < 3)
    {
        cout << "usage: " << argv[0] << " file [exit|_exit|return]" << endl;
        exit(1);
    }

    //向内核登记终止函数
    atexit(term_fun1);  //先登记的后调用
    atexit(term_fun2);
    atexit(term_fun3);  //后登记的先调用

    FILE *fp = fopen(argv[1], "w");
    fprintf(fp, "hello process");  //带缓存功能的全缓存文件写入函数，会首先将内容写入缓存
    //调用return或fclose()会将清缓存，写入文件
    if(!strcmp(argv[2], "exit"))
    {
        exit(0);  //能写入，能调用
    }
    else if(!strcmp(argv[2], "_exit"))
    {
        _exit(0);  //无法写入，
    }
    else if(!strcmp(argv[2], "return"))
    {
        return 0;  //能写入，能调用
    }
    else
    {
        cout << "usage: " << argv[0] << " file [exit|_exit|return]" << endl;
    }

    return 0;
}

