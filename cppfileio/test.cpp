#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
using namespace std;

int main(int argc, char const *argv[])
{
    ofstream fout("/root/adv_program/cppfileio/test.txt", ios::out);
    if(fout.is_open() == false)
    {
        perror("file open failed");
        exit(1);
    }

    //fout << "abcde\n";
    for (int i = 0; i < 1000; i++)
    {
        fout << "*******************************************************" << endl;
        //fout.flush();
        usleep(100000);
    }
    fout.close();
    return 0;
}

// int main(int argc, char const *argv[])
// {
//     ifstream fin;
//     fin.open("/root/adv_program/cppfileio/test.txt");
//     if(fin.is_open() == false)
//     {
//         perror("file open failed");
//         exit(1);
//     }
//     string buffer;
//     while(fin >> buffer)
//     {
//         cout << buffer << endl;
//         cout << buffer.length() << endl;
//     }
//     return 0;
// }

