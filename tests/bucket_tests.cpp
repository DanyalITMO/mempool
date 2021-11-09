#include <cstring>
#include "gtest/gtest.h"
#include "bucket.h"
#include "utils.h"

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

TEST_F(bucket_tests, set_blocks_in_use_test)
{
    std::memset(m_ledger, 0, m_ledger_size);
    set_blocks_in_use(4, 2);
    EXPECT_EQ(static_cast<std::byte>(unsigned{0b00001100}),m_ledger[0]);

    std::memset(m_ledger, 0, m_ledger_size);
    set_blocks_in_use(7, 4);
    EXPECT_EQ(static_cast<std::byte>(unsigned{0b00000001}),m_ledger[0]);
    EXPECT_EQ(static_cast<std::byte>(unsigned{0b11100000}),m_ledger[1]);

    std::memset(m_ledger, 0, m_ledger_size);
    set_blocks_in_use(7, 10);
    EXPECT_EQ(static_cast<std::byte>(unsigned{0b00000001}),m_ledger[0]);
    EXPECT_EQ(static_cast<std::byte>(unsigned{0b11111111}),m_ledger[1]);
    EXPECT_EQ(static_cast<std::byte>(unsigned{0b10000000}),m_ledger[2]);
}

TEST_F(bucket_tests, set_blocks_free_test)
{
    m_ledger[0] = static_cast<std::byte>(unsigned{0b11111111});
    set_blocks_free(4, 2);
    EXPECT_EQ(static_cast<std::byte>(unsigned{0b11110011}), m_ledger[0]);

    m_ledger[0] = static_cast<std::byte>(unsigned{0b11111111});
    m_ledger[1] = static_cast<std::byte>(unsigned{0b11111111});
    m_ledger[2] = static_cast<std::byte>(unsigned{0b11111111});
    m_ledger[3] = static_cast<std::byte>(unsigned{0b11111111});
    set_blocks_free(3, 25);
    EXPECT_EQ(static_cast<std::byte>(unsigned{0b11100000}), m_ledger[0]);
    EXPECT_EQ(static_cast<std::byte>(unsigned{0b00000000}), m_ledger[1]);
    EXPECT_EQ(static_cast<std::byte>(unsigned{0b00000000}), m_ledger[2]);
    EXPECT_EQ(static_cast<std::byte>(unsigned{0b00001111}), m_ledger[3]);
}
