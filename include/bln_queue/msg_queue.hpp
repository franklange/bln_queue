#pragma once

#include <bln_queue/spinlock.hpp>
#include <bln_queue/types.hpp>

#include <cstddef>
#include <list>
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

    using lock = std::lock_guard<spinlock>;
    using semaphore = std::counting_semaphore<>;

    spinlock  m_mutex{};
    semaphore m_semaphore{0};

    std::list<T> m_queue{};
};

template <typename T>
auto msg_queue<T>::put(T t) -> std::size_t
{
    const lock l{m_mutex};

    m_queue.push_back(std::move(t));
    m_semaphore.release();

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
    m_semaphore.acquire();

    const lock l{m_mutex};
    return pop();
}

template <typename T>
auto msg_queue<T>::wait(const timeout& t) -> std::optional<T>
{
    if (!m_semaphore.try_acquire_for(t))
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
