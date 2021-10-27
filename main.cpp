#include <iostream>
#include <vector>
#include <map>
#include "include/container.h"
#include "include/logging_allocator.h"


extern  "C" const char* __asan_default_options() {
    return "verbosity=1:disable_coredump=0:abort_on_error=1:unmap_shadow_on_exit=1:verify_asan_link_order=1";
}

struct A{
    int a1 =1;

    A(int arg) : a1{arg}
    {
        std::cerr<<"A constructor\n";
    }
    A(const A& a)
    {
        std::cerr<<"A copy constructor\n";
        a1 = a.a1;
    }
    ~A()
    {
        std::cerr<<"A destructor\n";
    }


};
int main()
{
    A a{1};

//    std::vector<A> vec;
//    for(int i = 0; i < 10; i++)
//    {
//        std::cerr<<vec.capacity()<<std::endl;
//        vec.push_back(a);
//    }
    //    TArray<int, logging_allocator<int>> integers(2);
//    TArray<int> integers(2);
    TArray<A> integers(1);
    std::fill(integers.begin(), integers.end(), 3);

//    integers.push_back(a);

    for(int i = 0; i < 10; i++)
    {
        std::cerr<<"---"<<std::endl;
        integers.push_back(a);
    }

//    for (auto i : integers)
//        std::cout << i.a1 << "\n";

/*    auto v = std::vector<int, logging_allocator<int>>{};
    // v.reserve(5);
    for (int i = 0; i < 6; ++i) {
        std::cout << "vector size = " << v.size() << std::endl;
        v.emplace_back(i);
        std::cout << std::endl;
    }*/

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

}