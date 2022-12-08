#include <bln_queue/data_queue.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>

namespace bln_queue::test {

using namespace ::testing;
using u16 = std::uint16_t;

struct data_queue_test : public ::testing::Test
{
    data_queue<u16, 16> m_queue;
    std::array<u16, 5> m_chunk{};
};

TEST_F(data_queue_test, get_on_empty)
{
    EXPECT_FALSE(m_queue.get());
}

TEST_F(data_queue_test, put_get)
{
    const u16 exp{13};
    m_queue.put(exp);

    const auto e = m_queue.get();

    EXPECT_TRUE(e);
    EXPECT_EQ(exp, e.value());
}

TEST_F(data_queue_test, bulk_get_empty)
{
    EXPECT_EQ(0, m_queue.get(m_chunk));
}

TEST_F(data_queue_test, bulk_get_full)
{
    const std::vector<u16> m_data{1, 2, 3, 4, 5, 6};

    for (const auto& d : m_data)
        m_queue.put(d);

    const auto n = m_queue.get(m_chunk);

    EXPECT_EQ(m_chunk.size(), n);
    EXPECT_THAT(m_chunk, ElementsAreArray(m_data.begin(), m_data.begin() + n));
}

TEST_F(data_queue_test, bulk_get_partial)
{
    const std::vector<u16> m_data{1, 2, 3};

    for (const auto& d : m_data)
        m_queue.put(d);

    const auto n = m_queue.get(m_chunk);

    EXPECT_EQ(m_data.size(), n);
    EXPECT_THAT(m_data, ElementsAreArray(m_chunk.begin(), m_chunk.begin() + n));
}

} // namespace bln_queue::test
