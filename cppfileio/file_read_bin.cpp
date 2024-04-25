#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[])
{
    //第一步，创建文件流对象
    ifstream fin;

    //打开二进制文件
    string filename = "/root/adv_program/cppfileio/test.dat";
    //在原有打开方式后追加ios::binary即可以二进制方式打开文件
    fin.open(filename, ios::binary);

    if(fin.is_open() == false)
    {
        perror("file open failed");
        exit(1);
    }
    cout << "file open success" << endl;
    //读取文件，二进制以数据类型的方式组织数据，没有换行的说法，怎么写进去就怎么读出来
    struct people{
        char name[20];
        int no;
        double balance;    
    };
    struct people p1;
    while(fin.read((char *)&p1, sizeof(people)))
    {
        cout << p1.name << endl;
        cout << p1.no << endl;
        cout << p1.balance << endl;
    }
    

    fin.close();
    return 0;
}
