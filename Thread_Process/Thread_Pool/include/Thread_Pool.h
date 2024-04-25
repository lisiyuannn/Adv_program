#include <pthread.h>
#include <list>


class ThreadPool{
private:
	struct NWORKER{  //即工作的线程
		pthread_t threadid;  //线程id
		bool terminate;  //线程结束标志  1结束
		bool isWorking;  //线程工作标志  1正在工作
		ThreadPool *pool;  //隶属线程池
	};

	struct NJOB{  //需要执行的任务结构体
		void (* func)(void *arg);  //任务函数
		void * user_data;  //任务参数
	};

public:
	//线程初始化
	ThreadPool(int numsWorkers, int maxJobs);
	//销毁线程池
	~ThreadPool();
	//面向用户的添加任务
	bool pushJob(void (*func)(void *), void *arg, int len);

private:
	//向线程中添加任务，内部实现，用户使用pushJob()添加任务
	bool addJob(NJOB *job);
	//回调函数
	static void *recall(void *arg);  //线程回调函数
	void threadLoop(void *arg);  //线程循环，不断检查是否有任务需要做

private:
	std::list<NJOB*> jobs_list;  //任务队列
	NWORKER *workers;  //线程数组结构体
	int max_jobs;  //队列中最大任务数
	int sum_thread;  //worker总数
	int free_thread;  //空闲worker总数
	pthread_cond_t jobs_cond;  //线程等待条件变量
	pthread_mutex_t jobs_mutex;  //队列互斥锁		
};
