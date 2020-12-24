#include "myThread.h"

int main(){

    pthread_t tid = pthread_self();
    char name[32];
    pthread_getname_np(tid, name, 32);
    printf("main name : %s\n", name);

    myThread("hello_sebas");

    return 1;
}