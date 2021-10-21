#include <iostream>
#include <vector>
#include <map>
#include "container.h"
#include "logging_allocator.h"

int main()
{
//    TArray<int, logging_allocator<int>> integers(2);
    TArray<int> integers(2);
    std::fill(integers.begin(), integers.end(), 3);

    for(int i = 0; i < 10; i++)
    {
        integers.push_back(5);
    }
    for (auto i : integers)
        std::cout << i << "\n";

/*    auto v = std::vector<int, logging_allocator<int>>{};
    // v.reserve(5);
    for (int i = 0; i < 6; ++i) {
        std::cout << "vector size = " << v.size() << std::endl;
        v.emplace_back(i);
        std::cout << std::endl;
    }*/
/*
    auto m = std::map<
            int,
            float,
            std::less<int>,
            logging_allocator<
                    std::pair<
                            const int, float
                    >
            >
    >{};

    for (int i = 0; i < 1; ++i) {
        m[i] = static_cast<float>(i);
    }
*/
}