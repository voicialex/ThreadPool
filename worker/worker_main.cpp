#include "BaseWorker.h"

int main()
{
    pthread_t tid = pthread_self();
    char name[32];
    pthread_getname_np(tid, name, 32);
    pid_t pid = getpid();
    printf("main name: %s,  tid: %ld, pid: %d \n", name, tid, pid);

    
    puts("main in +++");

    // BaseThread *th = new BaseWorker("this_is_alex");
    // th->start();
    
    BaseWorker th("work_test");
    th.start();

    char th_name[32];
    pthread_getname_np(th.mThreadId, th_name, 32);
    printf("mThreadId : %ld, BaseThread name : %s\n", th.mThreadId, th_name);
    
    puts("main out +++");
    return 1;
}