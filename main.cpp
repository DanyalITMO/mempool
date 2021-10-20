#include <iostream>
#include <vector>
#include "container.h"

int main()
{
    TArray<int> integers(2);
    std::fill(integers.begin(), integers.end(), 3);

    for(int i = 0; i < 10; i++)
    {
        integers.push_back(5);
    }
    for (auto i : integers)
        std::cout << i << "\n";
}