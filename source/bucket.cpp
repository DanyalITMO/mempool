#include "../include/bucket.h"
#include "../tests/utils.h"
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cmath>
#include <iostream>

bucket::bucket(std::size_t block_size, std::size_t block_count)
        : BlockSize{block_size}
        , BlockCount{block_count}
{
    if(block_count ==0)
        return;
//    assert(block_count > 0);
    assert(block_size > 0);
    const auto data_size = BlockSize * BlockCount;
    m_data = static_cast<std::byte*>(malloc(data_size));
    assert(m_data != nullptr);
    m_ledger_size = 1 + ((BlockCount - 1) / 8);
    m_ledger = static_cast<std::byte*>(std::malloc(m_ledger_size));
    assert(m_ledger != nullptr);
    std::memset(m_data, 0, data_size);
    std::memset(m_ledger, 0, m_ledger_size);
}

bucket::~bucket() {
    free(m_ledger);
    free(m_data);
}

bool bucket::belongs(void* ptr) const noexcept
{
    return (ptr >= m_data && ptr </*<=*/ m_data + (BlockCount * BlockSize));
}

void * bucket::allocate(std::size_t bytes) noexcept {
    if(BlockCount == 0)
        return nullptr;
    // Calculate the required number of blocks
    const auto n = 1 + ((bytes - 1) / BlockSize);
    const auto index = find_contiguous_blocks(n);
    if (index >= BlockCount) {
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
        std::byte filter{0b10000000};
        while (static_cast<unsigned>(filter))
        {
            if(static_cast<unsigned>(m_ledger[ledger_number] & filter) == 0)
            {
                contiguous_count++;

                if(contiguous_count == n)
                {
                    auto offset = 7 - std::log2(static_cast<unsigned>(filter));
                    auto last_block = ((ledger_number * 8) + offset + 1); // +1 потому что включительно позиция оффсета
                    if(last_block > BlockCount)
                        return -1;
                    else
                        return last_block - n; // +1 потому что иначе будет на предыдущий от первого свободного
                }
            } else
            {
                contiguous_count = 0;
            }

            filter >>= 1;
        }
    }

    return -1;
}


void bucket::set_block(std::size_t index, std::size_t n, bool inUse) noexcept
{
    std::size_t block = index / 8;
    std::size_t offset_start_block = index % 8;
    for(std::size_t i = 0; i < n; i++)
    {
        if(inUse)
        {
            m_ledger[block] |= static_cast<std::byte>(std::pow(2, 7 - offset_start_block));
        } else
        {
            m_ledger[block] &= ~static_cast<std::byte>(std::pow(2, 7 - offset_start_block));
        }

        if(offset_start_block == 7)
        {
            ++block;
            offset_start_block = 0;
        }
        else
        {
            offset_start_block++;
        }
    }
}

void bucket::set_blocks_in_use(std::size_t index, std::size_t n) noexcept
{
    set_block(index, n, true);
}

void bucket::set_blocks_free(std::size_t index, std::size_t n) noexcept
{
    set_block(index, n, false);
}
