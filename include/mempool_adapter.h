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
        void* ptr = nullptr;
        if(_mempool.allocate(ptr, elements * sizeof(T)))
            return reinterpret_cast<pointer>(ptr);

        throw std::bad_alloc();
    }

    void deallocate(pointer p, std::size_t n) {
        if(!_mempool.deallocate(p, n * sizeof(T)))
            throw std::logic_error{"free unknown memory region"};
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    };

    template<typename U>
    void destroy(U *p) {
        p->~U();
    }

    mempool<sizeof(T) * N> _mempool;
};