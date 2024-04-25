#include "Thread_Pool.h"
#include <iostream>
#include <string.h>


//创建线程时的回调函数
void * ThreadPool::recall(void *arg)
{
    std::cout << "this is recall func" << std::endl;
    NWORKER *worker = (NWORKER *)arg;
    worker->pool->threadLoop(arg);
    return NULL;
}

void ThreadPool::threadLoop(void *arg)
{//线程在用完后不会销毁，而是放回等待队列，阻塞等待是否有任务需要执行
 //如果有任务，则取出这个job，并将job从任务队列中删除，并且执行job中的func函数
 //如果没有任务，调用条件变量pthread_cond_wait函数，等待job将其唤醒
    std::cout << "this is threadLoop func" << std::endl;
    NWORKER *worker = (NWORKER *)arg;
    while (1)
    {//线程只有两个状态，执行/等待
        //先检查线程是否要结束
        if(worker->terminate)
        {
            //pthread_mutex_unlock(&jobs_mutex);
            break;
        }
        //对任务队列进行操作时需要先加锁
        pthread_mutex_lock(&jobs_mutex);
        while(jobs_list.empty())
        {//当任务队列为空时检查线程是否要结束
            if(worker->terminate) 
            {
                pthread_mutex_unlock(&jobs_mutex);
                break;
            }
            //进入等待队列直到被唤醒
            pthread_cond_wait(&jobs_cond, &jobs_mutex); 
        }
        //检查线程是否要结束
        if(worker->terminate)
        {
            pthread_mutex_unlock(&jobs_mutex);
            break;
        }
        //从任务队列中获取任务，并且将任务从任务队列中移除
        struct NJOB *job = jobs_list.front();
        jobs_list.pop_front();
        //获取到任务后释放锁
        pthread_mutex_unlock(&jobs_mutex);

        //空闲线程数减一
        free_thread--;
        worker->isWorking = true;

        //跳转到任务结构题中的函数，表示执行任务
        job->func(job->user_data);
        //执行完成后开始循环等待
        worker->isWorking = false;
        free_thread++;
        //释放任务结构体
        free(job->user_data);
        free(job);
    }
    //如果跳出循环，则证明worker->terminate为1，需要销毁该线程
    delete(worker);  //释放内存
    std::cout << "线程" << pthread_self() << "退出" << std::endl;
    pthread_exit(NULL);  //退出线程
}

//添加任务
bool ThreadPool::addJob(NJOB *job)
{
    //对任务队列的操作需要先上锁
    pthread_mutex_lock(&jobs_mutex);
    //判断队列里的任务是否超过数量上限
    if(jobs_list.size() >= max_jobs)
    {//当任务存满了
        //释放锁，发出错误信息
        pthread_mutex_unlock(&jobs_mutex);
        std::cout << "jobs_list is full" << std::endl;
        return false;
    }
    //向任务队列中添加任务
    jobs_list.push_back(job);
    //唤醒休眠的线程起来工作
    pthread_cond_broadcast(&jobs_cond); //广播方式唤醒
    pthread_mutex_unlock(&jobs_mutex);
    return true;
}

//面向用户添加任务
//参数为：回调函数，参数结构体指针，参数内存大小
bool ThreadPool::pushJob(void (*func)(void *), void *arg, int len)
{
    //堆区创建任务结构体
    struct NJOB *job = (struct NJOB *)malloc(sizeof(struct NJOB));
    if(job == NULL)
    {
        perror("job create failed");
        return false;
    }

    //初始化结构体
    memset(job, 0, sizeof(struct NJOB));

    job->user_data = malloc(len);
    memcpy(job->user_data, arg, len);
    job->func = func;
    addJob(job);  //调用内部添加任务函数

    return true;
}

//构造函数
//线程池线程总数，最大任务总数
ThreadPool::ThreadPool(int numsWorkers, int maxJobs):sum_thread(numsWorkers), max_jobs(maxJobs), free_thread(numsWorkers)
{
    if(numsWorkers < 1 || maxJobs < 1)
    {
        std::cout << "threadpool arg err" << std::endl;
        exit(1);
    }
    //初始化条件队列和锁
    if(pthread_cond_init(&jobs_cond, NULL) != 0)
    {
        perror("cond init err");
        exit(1);
    }
    if(pthread_mutex_init(&jobs_mutex, NULL) != 0)
    {
        perror("mutex init err");
        exit(1);
    }
    //初始化workers
    workers = new NWORKER[numsWorkers];
    if(workers == NULL)
    {
        perror("NWORKER[numsWorkers] create failed");
        exit(1);
    }
    //初始化每个worker
    for(int i = 0; i < numsWorkers; ++i)
    {
        workers[i].pool = this;  //所属线程就是该类本身
        //创建线程
        int err;
        if((err = pthread_create(&(workers[i].threadid), NULL, recall, &workers[i])) != 0)
        {
            perror("thread create failed");
            exit(1);
        }
        else{
            std::cout << "线程" << i << "创建成功" << std::endl;
        }
        //设置线程分离属性
        if(pthread_detach(workers[i].threadid))
        {
            perror("thread detach failed");
            exit(1);
        }
        workers[i].isWorking = 0;
        workers[i].terminate = 0;
    }
}

//析构函数
ThreadPool::~ThreadPool()
{
    for(int i = 0; i < sum_thread; ++i)
    {
        workers[i].terminate = 1;  //设置线程退出标志
    }
    //对线程队列上锁后唤起全部线程
    pthread_mutex_lock(&jobs_mutex);
    pthread_cond_broadcast(&jobs_cond);
    pthread_mutex_unlock(&jobs_mutex);
}
