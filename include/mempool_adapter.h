#pragma once

#include <iostream>
#include <algorithm>

#include "bucket.h"
#include "array"
#include "mempool.h"

template<typename T, int N>
struct mempool_adapter {
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = mempool_adapter<U, sizeof(U) * N>;
    };

    mempool_adapter() = default;

    ~mempool_adapter() = default;

    T *allocate(std::size_t elements) {

//        std::cout << "allocate: [n = " << elements << "]" << std::endl;
        void* ptr = nullptr;
        if(_mempool.allocate(ptr, elements * sizeof(T)))
            return reinterpret_cast<pointer>(ptr);

        throw std::bad_alloc();
    }

    void deallocate(pointer p, std::size_t n) {
//        std::cout << "deallocate: [n  = " << n << "] " << std::endl;
        if(!_mempool.deallocate(p, n * sizeof(T)))
            throw std::logic_error{"free unknown memory region"};
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
//        std::cout << "construct" << std::endl;
        new(p) U(std::forward<Args>(args)...);
    };

    template<typename U>
    void destroy(U *p) {
//        std::cout << "destroy" << std::endl;
        p->~U();
    }

    mempool<sizeof(T) * N> _mempool;
};