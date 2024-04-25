#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

int main(int argc, char const *argv[])
{
    int ret = system("tree /root/adv_program");
    cout << "ret = " << ret << endl;  //system的返回值，如果执行命令不存在，则返回非零
                                      //如果执行程序成功，并且执行程序的终止状态是0，返回0
                                      //如果执行程序成功，并且执行程序的终止状态非零，返回非零

    perror("system");
    return 0;
}
