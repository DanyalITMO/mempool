#include "mempool_adapter.h"
#include "gtest/gtest.h"

class mempool_tests : public ::testing::Test
{
public:
};

TEST_F(mempool_tests, createAndCheck)
{
    auto m = std::map<
            int,
            float,
            std::less<>,
            mempool_adapter<
            std::pair<
                    const int, float
            >, 500
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

TEST_F(mempool_tests, repeadetlyCreateAndCheck)
{
    auto m = std::map<
            int,
            float,
            std::less<>,
            mempool_adapter<
                    std::pair<
                            const int, float
                    >, 500
            >
    >{};

    for (int i = 0; i < 100000; ++i) {
        m[i] = static_cast<float>(i);
        EXPECT_EQ(static_cast<float>(i), m[i]);
        m.erase(i);
    }
}

TEST_F(mempool_tests, createAndModifyAndCheck)
{
    auto m = std::map<
            int,
            float,
            std::less<>,
            mempool_adapter<
                    std::pair<
                            const int, float
                    >, 500
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
