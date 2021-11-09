#include "mempool_allocator.h"
#include "gtest/gtest.h"

class mempool_tests : public ::testing::Test
{
public:
};

TEST_F(mempool_tests, test1)
{
    auto m = std::map<
            int,
            float,
            std::less<>,
            mempool_allocator<
            std::pair<
                    const int, float
            >
    >
             >{};

    std::size_t n = 100;
    for (int i = 0; i < n; ++i) {
        m[i] = static_cast<float>(i * i);
    }

    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(static_cast<float>(i * i), m[i]);
    }
}

TEST_F(mempool_tests, test2)
{
    auto m = std::map<
            int,
            float,
            std::less<>,
            mempool_allocator<
                    std::pair<
                            const int, float
                    >
            >
    >{};

    std::size_t n = 100;
    for (int i = 0; i < n; ++i) {
        m[i] = static_cast<float>(i);
    }

    for(int i = 80; i < 90; i++)
    {
        m.erase(i);
    }

    for (int i = 120; i < 150; ++i) {
        m[i] = static_cast<float>(i);
    }

    for (int i = 0; i < 80; ++i) {
        EXPECT_EQ(static_cast<float>(i), m[i]);
    }
    for (int i = 90; i < 100; ++i) {
        EXPECT_EQ(static_cast<float>(i), m[i]);
    }
    for (int i = 120; i < 150; ++i) {
        EXPECT_EQ(static_cast<float>(i), m[i]);
    }
}
