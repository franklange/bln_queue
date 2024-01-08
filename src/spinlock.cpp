#include <bln_queue/spinlock.h>

namespace bln_queue {

void spinlock::lock()
{
    while(true)
    {
        if (!m_flag.test_and_set(std::memory_order_acquire))
            break;

        while (m_flag.test(std::memory_order_relaxed))
            ;
    }
}

void spinlock::unlock()
{
    m_flag.clear(std::memory_order_release);
}

} // namespace bln_queue
