#ifndef HW2_CHOOSEBUCKETSTRATEGY_H
#define HW2_CHOOSEBUCKETSTRATEGY_H

struct info
{
    std::size_t index{0};
    std::size_t block_count{0};
    std::size_t block_size{0};
};

template<int N>
class ChooseBucketStrategy
{
public:
    virtual ~ChooseBucketStrategy() = default;

    virtual std::array<std::size_t, N> choose(const std::array<info, N>& infos, std::size_t required_bytes) = 0;
};

#endif //HW2_CHOOSEBUCKETSTRATEGY_H
