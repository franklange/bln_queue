#pragma once

#include <bln_queue/types.hpp>

#include <cstddef>
#include <list>
#include <mutex>
#include <optional>
#include <semaphore>

namespace bln_queue {

template <typename T>
class msg_queue
{
public:
    auto put(T) -> std::size_t;
    auto get()  -> std::optional<T>;

    auto wait() -> T;
    auto wait(const timeout&) -> std::optional<T>;

private:
    auto pop() -> T;

    struct mutex
    {
        void lock()  { m_semaphore.acquire(); }
        void unlock(){ m_semaphore.release(); }

        std::binary_semaphore m_semaphore{1};
    };

    using lock = std::lock_guard<mutex>;
    using semaphore = std::counting_semaphore<>;

    mutex m_mutex;
    semaphore m_signal{0};

    std::list<T> m_queue;
};

template <typename T>
auto msg_queue<T>::put(T t) -> std::size_t
{
    const lock l{m_mutex};

    m_queue.push_back(std::move(t));
    m_signal.release();

    return m_queue.size();
}

template <typename T>
auto msg_queue<T>::get() -> std::optional<T>
{
    const lock l{m_mutex};
    return m_queue.empty() ? std::optional<T>{} : pop();
}

template <typename T>
auto msg_queue<T>::wait() -> T
{
    m_signal.acquire();

    const lock l{m_mutex};
    return pop();
}

template <typename T>
auto msg_queue<T>::wait(const timeout& t) -> std::optional<T>
{
    if (!m_signal.try_acquire_for(t))
        return {};

    const lock l{m_mutex};
    return pop();
}

template <typename T>
auto msg_queue<T>::pop() -> T
{
    const T t = std::move(m_queue.front());
    m_queue.pop_front();

    return t;
}

} // namespace bln_queue
