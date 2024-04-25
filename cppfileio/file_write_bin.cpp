#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[])
{
    //第一步，创建文件流对象
    ofstream fout;

    //打开二进制文件
    string filename = "/root/adv_program/cppfileio/test.dat";
    //在原有打开方式后追加ios::binary即可以二进制方式打开文件
    fout.open(filename, ios::binary);

    if(fout.is_open() == false)
    {
        perror("file open failed");
        exit(1);
    }

    //像文件中写入数据
    struct people{
        char name[20];
        int no;
        double balance;    
    };

    struct people sunwukong{"孙悟空", 1, 99999.99};
    fout.write((char *)&sunwukong, sizeof(people));
    struct people tangseng{"唐僧", 2, 99.99};
    fout.write((char *)&tangseng, sizeof(people));

    fout.close();
    return 0;
}
