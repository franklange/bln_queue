#include <bln_queue/msg_queue.hpp>

#include <gtest/gtest.h>

namespace bln_queue::test {

struct msg_queue_test : public ::testing::Test
{
    msg_queue<int> m_queue;
};

TEST_F(msg_queue_test, get_on_empty)
{
    EXPECT_FALSE(m_queue.get());
}

TEST_F(msg_queue_test, timed_wait_on_empty)
{
    EXPECT_FALSE(m_queue.wait(100us));
}

TEST_F(msg_queue_test, put_get)
{
    const int n{13};

    m_queue.put(n);
    const auto v = m_queue.get();

    EXPECT_TRUE(v);
    EXPECT_EQ(n, v.value());
}

TEST_F(msg_queue_test, put_wait)
{
    m_queue.put(19);
    const auto v = m_queue.wait();

    EXPECT_EQ(19, v);
}

TEST_F(msg_queue_test, put_timed_wait)
{
    m_queue.put(17);
    const auto v = m_queue.wait(100us);

    EXPECT_TRUE(v);
    EXPECT_EQ(17, v.value());
}

} // namespace bln_queue::test
