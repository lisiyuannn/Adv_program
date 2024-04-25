#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char const *argv[])
{
    //第一步，创建文件输出流对象
    ifstream fin;

    //第二步，打开文件，如果文件不存在，则会失败
    string filename = "/root/adv_program/cppfileio/test.txt";  //可以使用c风格字符串，也可以使用string
    //文件打开模式：
    /*
    ios::in 缺省值，会截断文件内容
    */
    //fout.open(filename, 打开模式);
    fin.open(filename, ios::in);
    //判断文件打开是否成功
    //失败的情况主要有三种，目录不存在，文件不存在，没有权限
    if(fin.is_open() == false)
    {
        perror("file open failed");
        exit(1);
    }

    //第三步，读取文件中的内容
    //文本文件一般以行的方式组织数据，一行一行读
    //定义一个buff用于存放读出的数据
    // string buffer;
    // while(getline(fin, buffer))
    // {
    //     cout << buffer << endl;
    // }

    //方法2
    // char buffer[101];  //c风格字符串
    // while(fin.getline(buffer, 100))
    // {
    //     cout << buffer << endl;
    // }

    //方法3
    string buffer;
    while(fin >> buffer)
    {
        cout << buffer << endl;
    }
    


    //第四步，关闭文件，fout对象失效前会自动调用close()
    fin.close();
    return 0;
}
