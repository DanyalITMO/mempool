//
// Created by mugutdinov on 27.10.2021.
//

#include "gtest/gtest.h"
#include "mempool.h"


class bucket_tests : public bucket, public ::testing::Test
{
public:
    bucket_tests() : bucket(4,27)
    {
    }
};

TEST_F(bucket_tests, find_contiguous_blocks_test)
{
    EXPECT_EQ(0, find_contiguous_blocks(4));

    m_ledger[0] = static_cast<std::byte>(unsigned{0b11111111});
    EXPECT_EQ(8, find_contiguous_blocks(4));

    m_ledger[0] = static_cast<std::byte>(unsigned{0b11111000});
    EXPECT_EQ(5, find_contiguous_blocks(4));

    m_ledger[0] = static_cast<std::byte>(unsigned{0b10101011});
    EXPECT_EQ(8, find_contiguous_blocks(4));

    m_ledger[0] = static_cast<std::byte>(unsigned{0b10101011});
    m_ledger[1] = static_cast<std::byte>(unsigned{0b10100011});
    m_ledger[2] = static_cast<std::byte>(unsigned{0b10100100});
    EXPECT_EQ(22, find_contiguous_blocks(4));

    m_ledger[0] = static_cast<std::byte>(unsigned{0b11111111});
    m_ledger[1] = static_cast<std::byte>(unsigned{0b11111111});
    m_ledger[2] = static_cast<std::byte>(unsigned{0b11111111});
    m_ledger[3] = static_cast<std::byte>(unsigned{0b11000000});
    EXPECT_EQ(-1, find_contiguous_blocks(5));
}
