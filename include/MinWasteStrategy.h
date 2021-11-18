//
// Created by mugutdinov on 16.11.2021.
//

#ifndef HW2_MINWASTESTRATEGY_H
#define HW2_MINWASTESTRATEGY_H

template<int N>
class MinWasteStrategy : public ChooseBucketStrategy<N>
{
private:
    struct WasteInfo
    {
        std::size_t index{0};
        std::size_t waste{0};
        std::size_t block_count{0};
    };

public:
    MinWasteStrategy() = default;
    virtual ~MinWasteStrategy() = default;

    virtual std::array<std::size_t , N> choose(const std::array<info, N>& infos, std::size_t required_bytes) {

        std::array<WasteInfo, N> waste_infos;

        for (std::size_t i = 0; i < infos.size(); i++) {
            waste_infos[i].index = i;
            auto &&bucket = infos[i];
            const auto n = 1 + ((required_bytes - 1) / bucket.block_size);
            const auto storage_required = n * bucket.block_size;
            waste_infos[i].waste = storage_required - required_bytes;
        }

        std::sort(std::begin(waste_infos), std::end(waste_infos), [](const auto &arg1, const auto &arg2) {
            if (arg1.waste != arg2.waste)
                return arg1.waste > arg2.waste;
            return arg1.block_count > arg2.block_count;
        });

        std::array<std::size_t, N> ret;

        for (std::size_t i = 0; i < waste_infos.size(); i++)
        {
            ret[i] = waste_infos[i].index;
        }

        return ret;
    }
};

#endif //HW2_MINWASTESTRATEGY_H
