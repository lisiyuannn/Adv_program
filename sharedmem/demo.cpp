//共享内存  只能使用内置的数据类型，因为其他数据类型例如STL会自动扩展内存
//shell中执行 ipcs -m 可以查看当前共享内存
//ipcrm -m 编号 可以手动删除共享内存
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
using namespace std;

struct people
{
    int no;  //编号
    char name[51];  //姓名
};

int main(int argc, char const *argv[])
{
     //设置主程序参数
     if(argc != 3)
     {
        cout << "参数错误！！！" << endl;
        return -1;
     }

     //创建共享内存
     //参数1为键值，16进制，2为内存大小，3为访问权限和如果内存不存在需要创建
     int shmid = shmget(0x1000, sizeof(people), 0640|IPC_CREAT);
     if (shmid == -1)
     {//创建失败
        cout << "共享内存创建失败" << endl;
        return -1;
     }

     cout << "shmid = " << shmid << endl;

     //将共享内存连接到当前进程的地址空间
     //shmat函数参数为地址编号，0，0   成功会返回首地址，失败返回(void *)-1
     people *ptr = (people *)shmat(shmid, 0, 0);
     if(ptr == (void *)-1)
     {
        cout << "链接失败！！！" << endl;
     }

     //使用共享内存，对内存进行读写
     cout << "原值：no = " << ptr->no << '\t' << "name = " << ptr->name << endl;
     ptr->no = atoi(argv[1]);  //字符串转整形
     strcpy(ptr->name, argv[2]);
     cout << "修改后：no = " << ptr->no << '\t' << "name = " << ptr->name << endl;

     //使用完后将内存分离，参数为内存首地址
     shmdt(ptr);

     //删除共享内存，参数为内存编号
     if(shmctl(shmid, IPC_RMID, 0) == -1)
     {
        cout << "删除失败！！！" << endl;
        return -1;
     }

    return 0;
}
