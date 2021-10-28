#include "utils.h"
#include <iostream>
#include <bitset>

void print_bin(unsigned a)
{
    std::bitset<8> x(a);
    std::cerr << x << '\n';
}
