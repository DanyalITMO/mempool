//
// Created by mugutdinov on 26.10.2021.
//

#ifndef HW2_MEMPOOL_H
#define HW2_MEMPOOL_H

#include <cstddef>

class bucket{
public:
    const std::size_t BlockSize;
    const std::size_t BlockCount;

    bucket(std::size_t block_size, std::size_t  block_count);
    ~bucket();

    bool belongs(void* ptr) const noexcept;

    [[nodiscard]] void * allocate(std::size_t bytes) noexcept;
    void deallocate(void * ptr, std::size_t bytes) noexcept;

private:
    std::size_t find_contiguous_blocks(std::size_t n) const noexcept;
    void set_blocks_in_use(std::size_t index, std::size_t n) noexcept;
    void set_blocks_free(std::size_t index, std::size_t n) noexcept;

    std::byte* m_data{nullptr};
    std::byte* m_ledger{nullptr};
};

class mempool {

};


#endif //HW2_MEMPOOL_H
