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
        printf("                     +++ this is %d job\n", order);
        usleep(500000);
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

    ThreadPoolManager Pool(3);
    sleep(1);

    std::shared_ptr<BaseJob> job[10];
    for (int i = 0; i < 10; i++) {
        // myJob* job = new myJob(i);
        job[i] = std::make_shared<myJob>(i);
        Pool.enqueue(job[i]);
    }

    sleep(10);

    puts("\nmain out +++");
    return 1;
}