#include <iostream>
#include <vector>
#include <chrono>

#include "ThreadPool.h"

int main()
{
    
    ThreadPool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                printf("%d start\n", i);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                printf("           %d end\n", i);
                return i*i;
            })
        );
    }

    // 异步访问task结果
    for(auto && result: results)
        std::cout << result.get() << "\n";
    std::cout << std::endl;
    
    return 0;
}
