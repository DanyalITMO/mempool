#ifndef HW2_MEMPOOL_H
#define HW2_MEMPOOL_H

#include <memory>
#include "bucket.h"
#include "ChooseBucketStrategy.h"
#include "MinWasteStrategy.h"

template <int BLOCK_COUNT = 500>
class mempool {
private:

public:

    mempool()
    {
        s = std::make_unique<MinWasteStrategy<_buckets_count>>();
    }

    bool allocate(void*& p, std::size_t bytes)
    {
        std::array<info, _buckets_count> infos;
        for(std::size_t i = 0; i < _buckets.size(); i++)
        {
            infos[i].index = i;
            auto&& bucket = _buckets[i];
            infos[i].block_count = bucket.BlockCount;
            infos[i].block_size = bucket.BlockSize;
        }

        auto order = s->choose(infos, bytes);

        for(const auto& d : order)
        {
            if(p = _buckets[d].allocate(bytes); p != nullptr)
            {
                return true;
            }
        }

        return false;
    }

    bool deallocate(void* p , std::size_t bytes)
    {
        auto it = std::find_if(std::begin(_buckets), std::end(_buckets),
                               [p](const auto& b){
                                   return b.belongs(p);
                               });

        if(std::end(_buckets) != it)
        {
            it->deallocate(p, bytes);
            return true;
        }

        return false;
    }



private:
    constexpr static std::size_t _buckets_count = 4;
    std::unique_ptr<ChooseBucketStrategy<_buckets_count>> s;
    std::array<bucket, _buckets_count> _buckets{bucket{64, BLOCK_COUNT},
                                                bucket{128, BLOCK_COUNT},
                                                bucket{256, BLOCK_COUNT},
                                                bucket{512, BLOCK_COUNT}


    };

};


#endif //HW2_MEMPOOL_H
