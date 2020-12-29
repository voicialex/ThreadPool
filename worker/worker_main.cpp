#include "BaseWorker.h"
#include "BaseJobQueue.h"

class myJob : public BaseJob
{
public:
    myJob(int i) : BaseJob(1), order(i) 
    {
        printf("myjob_%d\n", order);
    }
    ~myJob() {printf("~myjob%d\n", order);}
    void run() 
    {
        printf("***this is %d job\n", order);
        usleep(100000);
    }

private:
    int order;
};



int main()
{
    pthread_t tid = pthread_self();
    char name[32];
    pthread_getname_np(tid, name, 32);
    pid_t pid = getpid();
    printf("app: %s,  tid: %ld, pid: %d \n", name, tid, pid);
    puts("\nmain in +++\n");
    
    // 1. test for threadPool

    // ThreadPoolManager Pool(3);
    // sleep(3);
    // myJob *job;
    // for (int i = 0; i < 20; i++) {
    //     job = new myJob(i);
    //     Pool.enqueue(job);
    // }
    

    // 2. test for share pointer job


    {
        auto sp = std::make_shared<myJob>(2);
        sp->run();
    }

    puts("\nmain out +++");
    return 1;
}