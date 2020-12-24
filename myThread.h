#include <pthread.h>
#include <stdio.h>
#include <string>
#include <unistd.h>

class myThread
{
public:
    myThread(const char* thread_name = "");
    virtual ~myThread();

    pthread_t mThread;
    int mPriority;
    std::string mThreadName;
private:
    /* data */
    static void* func(void* arg) {
        puts("work start");
        for (int i = 0; i < 10; i++) {
            printf("%d \n", i);
            usleep(100000);
        }
        puts("work done");

        return NULL;
    }
};

myThread::myThread(const char* thread_name)
        : mPriority(10)
        , mThreadName(thread_name ? thread_name : "")
{
    puts("constructor");
    pthread_attr_t attr;
    if (!pthread_attr_init(&attr)) {
        if (!pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)) {
            if (!pthread_create(&mThread, &attr, func, this)) {
                // set thread priority
                {
                    int schedPolicy;
                    struct sched_param schedParam;
                    if (!pthread_getschedparam(mThread, &schedPolicy, &schedParam)) {
                        schedParam.sched_priority = mPriority;
                        if (!pthread_setschedparam(mThread, schedPolicy, &schedParam)) {
                            puts("set priority success");
                        }
                    }
                }
                // set thread name
                {
                    // if (!pthread_setname_np(mThread, mThreadName.c_str())) { }
                    if (!pthread_setname_np(mThreadName.c_str())) {
                        puts("set name success");
                    }
                }

            }
        }

        if (pthread_attr_destroy(&attr) != 0) {
            puts("destory attribute failed");
        }
    }
}

myThread::~myThread()
{
    puts("destructor");pthread_t tid = pthread_self();
    char name[32];
    pthread_getname_np(tid, name, 32);
    printf("myThread name : %s\n", name);

    pthread_exit(NULL);
}
