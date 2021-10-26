//
// Created by mugutdinov on 26.10.2021.
//

#include "mempool.h"
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cmath>

bucket::bucket(std::size_t block_size, std::size_t block_count)
        : BlockSize{block_size}
        , BlockCount{block_count}
{
    assert(block_count > 0);
    assert(block_size > 0);
    const auto data_size = BlockSize * BlockCount;
    m_data = static_cast<std::byte*>(malloc(data_size));
    assert(m_data != nullptr);
    const auto ledger_size = 1 + ((BlockCount - 1) / 8);
    m_ledger = static_cast<std::byte*>(std::malloc(ledger_size));
    assert(m_ledger != nullptr);
    std::memset(m_data, 0, data_size);
    std::memset(m_ledger, 0, ledger_size);
}
bucket::~bucket() {
    free(m_ledger);
    free(m_data);
}

void * bucket::allocate(std::size_t bytes) noexcept {
    // Calculate the required number of blocks
    const auto n = 1 + ((bytes - 1) / BlockSize);
    const auto index = find_contiguous_blocks(n);
    if (index == BlockCount) {
        return nullptr;
    }
    set_blocks_in_use(index, n);
    return m_data + (index * BlockSize);
}

void bucket::deallocate(void * ptr, std::size_t bytes) noexcept {
    const auto p = static_cast<const std::byte *>(ptr);
    const std::size_t dist = static_cast<std::size_t>(p - m_data);
    // Calculate block index from pointer distance
    const auto index = dist / BlockSize;
    // Calculate the required number of blocks
    const auto n = 1 + ((bytes - 1) / BlockSize);
    // Update the ledger
    set_blocks_free(index, n);
}

std::size_t bucket::find_contiguous_blocks(std::size_t n) const noexcept
{
    const auto ledger_size = 1 + ((BlockCount - 1) / 8);

    std::size_t contiguous_count = 0;

    for(std::size_t ledger_number = 0; ledger_number < ledger_size; ledger_number++)
    {
        std::byte filter{0b1000000};
        while (static_cast<unsigned>(filter))
        {
            if(static_cast<unsigned>(m_ledger[ledger_number] & filter))
            {
                filter >>= 1;
                contiguous_count++;
            } else
            {
                contiguous_count = 0;
            }

            if(contiguous_count == n)
            {
                auto offset = std::log2(static_cast<unsigned>(filter));
                return (ledger_number * 8) + offset;
            }
        }
    }

    return -1;
}

void bucket::set_blocks_in_use(std::size_t index, std::size_t n) noexcept
{
    std::size_t block = index / 8;
    std::size_t offset_start_block = index % 8;
    for(std::size_t i = 0; i < n; i++)
    {
        m_ledger[block] |= static_cast<std::byte>(std::pow(2, offset_start_block - 1));
        offset_start_block--;
        if(offset_start_block == 0)
        {
            ++block;
            offset_start_block = 7;
        }
    }
}

void bucket::set_blocks_free(std::size_t index, std::size_t n) noexcept
{

}
