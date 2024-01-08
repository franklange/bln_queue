#include <bln_queue/queue.h>

#include <gtest/gtest.h>

namespace bln_queue::test {

struct QueueTest : public ::testing::Test
{
    queue<int> m_queue;
};

TEST_F(QueueTest, get_on_empty)
{
    EXPECT_FALSE(m_queue.get());
}

TEST_F(QueueTest, timed_wait_on_empty)
{
    EXPECT_FALSE(m_queue.wait(1ms));
}

TEST_F(QueueTest, put_get)
{
    const int n{13};

    m_queue.put(n);
    const auto v = m_queue.get();

    EXPECT_TRUE(v);
    EXPECT_EQ(n, v.value());
}

TEST_F(QueueTest, put_wait)
{
    m_queue.put(19);
    const auto v = m_queue.wait();

    EXPECT_EQ(19, v);
}

TEST_F(QueueTest, put_timed_wait)
{
    m_queue.put(17);
    const auto v = m_queue.wait(1ms);

    EXPECT_TRUE(v);
    EXPECT_EQ(17, v.value());
}

} // namespace bln_queue::test
