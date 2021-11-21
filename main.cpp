#include <iostream>
#include <vector>
#include <map>
#include "include/container.h"
#include "include/logging_allocator.h"
#include "include/mempool_adapter.h"

/**NOTE  основная идея и часть кода взята отсюда https://youtu.be/l14Zkx5OXr4*/

extern  "C" const char* __asan_default_options() {
    return "verbosity=1:disable_coredump=0:abort_on_error=1:unmap_shadow_on_exit=1:verify_asan_link_order=1";
}

int fact(int n){

    return (n==0) || (n==1) ? 1 : n* fact(n-1);
}

int main()
{
    //std::map std::allocator
    {
        auto m = std::map<int, int>{};

        for (int i = 0; i < 10; ++i) {
            m[i] = fact(i);
        }

        for (const auto &it : m) {
            std::cout << it.first << " " << it.second << std::endl;
        }
    }

    //std::map mempool_adapter
    {
        auto m = std::map<
                int,
                int,
                std::less<>,
                mempool_adapter<std::pair<int, int>, 10>
        >{};

        for (int i = 0; i < 10; ++i) {
            m[i] = static_cast<float>(fact(i));
        }

        for (const auto &it : m) {
            std::cout << it.first << " " << it.second << std::endl;
        }
    }

    //TArray std::allocator
    {
        TArray<int> integers(10);

        for (int i = 0; i < 10; ++i) {
            integers.push_back(fact(i));
        }

        for (const auto &it : integers) {
            std::cout << it << std::endl;
        }
    }

    //TArray mempool_adapter
    {
        TArray<int, mempool_adapter<int, 10>> integers(10);

        for (int i = 0; i < 10; ++i) {
            integers.push_back(fact(i));
        }

        for (const auto &it : integers) {
            std::cout << it << std::endl;
        }
    }


}