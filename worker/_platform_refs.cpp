#include <stdio.h>

int main()
{
    puts("start");

#ifdef __APPLE__
    puts("__APPLE__");
    #if TARGET_OS_MAC
    puts("TARGET_OS_MAC");
    #elif TARGET_OS_IPHONE
    puts("TARGET_OS_IPHONE")
    #elif TARGET_IPHONE_SIMULATOR
    puts("TARGET_IPHONE_SIMULATOR")
    #else
    // #error "unknown Apple platform"
    puts("unknown Apple platform");
    #endif

#elif __linux__
    puts("__linux__");

#elif __unix__
    pus("__unix__");

#elif _WIN32
    puts("_WIN32");
    
#else
# error "my bro, unknown compiler"

#endif

    puts("end");
    return 1;
}