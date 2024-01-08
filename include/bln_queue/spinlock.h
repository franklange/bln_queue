#pragma once

#include <atomic>

namespace bln_queue {

class spinlock
{
public:
    void lock();
    void unlock();

private:
    std::atomic_flag m_flag = ATOMIC_FLAG_INIT;
};

} // namespace bln_queue
