#pragma once

#include <iostream>
#include "bucket.h"
#include "array"
#include "algorithm"

struct info
{
    std::size_t index{0};
    std::size_t waste{0};
    std::size_t block_count{0};
};

template<typename T>
struct mempool_allocator {
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    constexpr static std::size_t _buckets_count = 4;
    std::array<bucket, _buckets_count> _buckets{bucket{64, 500},
                                   bucket{128, 500},
                                   bucket{256, 500},
                                   bucket{512, 500}
    };

    template<typename U>
    struct rebind {
        using other = mempool_allocator<U>;
    };

    mempool_allocator()
    {
    }

    ~mempool_allocator() = default;

    template<typename U>
    mempool_allocator(const mempool_allocator<U>&) {
    }

    T *allocate(std::size_t elements) {

        std::size_t bytes = elements * sizeof(T);
        std::cout << "allocate: [n = " << elements << "]" << std::endl;

        std::array<info, _buckets_count> deltas;
        for(std::size_t i = 0; i < _buckets.size(); i++)
        {
            deltas[i].index = i;
            auto&& bucket = _buckets[i];
            const auto n = 1 + ((bytes - 1) / bucket.BlockSize);
            const auto storage_required = n * bucket.BlockSize;
            deltas[i].waste = storage_required - bytes;
            deltas[i].block_count = n;
        }

        std::sort(std::begin(deltas), std::end(deltas), [](const auto& arg1, const auto& arg2){
            if(arg1.waste != arg2.waste)
                return arg1.waste > arg2.waste;
            return arg1.block_count > arg2.block_count;
        });

        for(const auto& d : deltas)
        {
            if(auto ptr = _buckets[d.index].allocate(bytes); ptr != nullptr)
                return reinterpret_cast<T *>(ptr);
        }

        throw std::bad_alloc();
//        auto p = std::malloc(n * sizeof(T));
    }

    void deallocate(T *p, std::size_t n) {
        std::cout << "deallocate: [n  = " << n << "] " << std::endl;
//        std::free(p);
    }

    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        std::cout << "construct" << std::endl;
        new(p) U(std::forward<Args>(args)...);
    };

    template<typename U>
    void destroy(U *p) {
        std::cout << "destroy: " << typeid(U).name()<< std::endl;
        p->~U();
    }
};