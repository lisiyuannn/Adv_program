#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char const *argv[])
{
    //第一步，创建文件输出流对象
    ofstream fout;

    //第二步，打开文件，如果文件不存在，则创建它，如果文件已经存在，则截断其内容
    string filename = "/root/adv_program/cppfileio/test.txt";  //可以使用c风格字符串，也可以使用string
    //文件打开模式：
    /*
    ios::out 缺省值，会截断文件内容
    ios::trunc  截断文件内容
    ios::app  不截断文件内容，只在文件末尾追加文件
    */
    //fout.open(filename, 打开模式);
    fout.open(filename, ios::app);
    //判断文件打开是否成功
    //失败的情况主要有三种，目录不存在，磁盘空间已满，没有权限
    if(fout.is_open() == false)
    {
        perror("file open failed");
        exit(1);
    }

    //第三步，向文件中写入数据
    fout << "孙悟空\n";
    fout << "猪八戒\n";
    fout << "沙僧\n";
    fout << "唐僧\n";

    //第四步，关闭文件，fout对象失效前会自动调用close()
    fout.close();
    return 0;
}
