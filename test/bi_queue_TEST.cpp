#include <bln_queue/bi_queue.hpp>

#include <gtest/gtest.h>

#include <string>

namespace bln_queue::test {

struct bi_queue_test : public ::testing::Test
{
    using Queue = BiQueue<int, std::string>;

    Queue m_queue{16};
    Queue::A& m_sideA{m_queue.sideA};
    Queue::B& m_sideB{m_queue.sideB};
};

TEST_F(bi_queue_test, get)
{
    EXPECT_FALSE(m_sideA.get());
    EXPECT_FALSE(m_sideB.get());

    m_sideA.put(13);
    EXPECT_FALSE(m_sideA.get());

    const auto v = m_sideB.get();
    EXPECT_TRUE(v);
    EXPECT_EQ(13, v.value());
}

TEST_F(bi_queue_test, wait)
{
    EXPECT_FALSE(m_sideA.wait(100us));
    EXPECT_FALSE(m_sideB.wait(100us));

    m_sideA.put(15);

    EXPECT_FALSE(m_sideA.wait(100us));
    EXPECT_EQ(15, m_sideB.wait());
}

TEST_F(bi_queue_test, roundtrip)
{
    m_sideA.put(17);
    EXPECT_FALSE(m_sideA.get());
    EXPECT_EQ(17, m_sideB.wait());

    m_sideB.put("19");
    EXPECT_FALSE(m_sideB.get());
    EXPECT_EQ("19", m_sideA.wait());
}

} // namespace bln_queue::test
