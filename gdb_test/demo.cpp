#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{

    cout << "*****************" << endl;
    for (int i = 0; i < 10000; ++i)
    {
        sleep(1);
        cout << i << endl;
    }
    cout << "*****************" << endl;
    return 0;
}