#include "Thread_Pool.h"
#include <iostream>
#include <unistd.h>

void testFun(void* arg){
    std::cout << "this is testFun" << std::endl;
    std::cout << pthread_self() << ": " << *(long *)arg << std::endl;
}


int main(){
    ThreadPool *pool = new ThreadPool(4, 100);

    std::cout << "线程池初始化成功" << std::endl;
    long i = 0;
    for (i = 0; i < 20; ++i) {
        usleep(200000);
        pool->pushJob(testFun, &i, sizeof(&i));
    }
    sleep(5);
    //释放线程池内存
    std::cout << "线程池释放内存" << std::endl;
    delete(pool);
    sleep(5);
    return 0;
}
