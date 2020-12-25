#ifndef __WORKER_H
#define __WORKER_H

#include "BaseThread.h"

class BaseWorker : public BaseThread
{
public:
    BaseWorker(const char* thread_name = "");
    virtual ~BaseWorker();

    void run() 
    {
        
        for (int i = 0; i < 10; i++) {
            printf("%d \n", i);
            usleep(100000);
        }

        puts("hi, I'm gnu");
    }
};
 
BaseWorker::BaseWorker(const char* thread_name)
    : BaseThread(thread_name)
{
    puts("BaseWorker");
}

BaseWorker::~BaseWorker()
{
    puts("~BaseWorker");
}



#endif // __WORKER_H