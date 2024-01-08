#pragma once

#include <bln_queue/spinlock.h>

#include <chrono>
#include <cstddef>
#include <queue>
#include <optional>
#include <semaphore>

namespace bln_queue {

using namespace std::chrono_literals;

template <typename T>
class queue
{
public:
    using timeout = std::chrono::milliseconds;

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

    std::queue<T> m_queue{};
};

template <typename T>
auto queue<T>::put(T t) -> std::size_t
{
    const lock l{m_mutex};

    m_queue.push(std::move(t));
    m_semaphore.release();

    return m_queue.size();
}

template <typename T>
auto queue<T>::get() -> std::optional<T>
{
    const lock l{m_mutex};
    return m_queue.empty() ? std::optional<T>{} : pop();
}

template <typename T>
auto queue<T>::wait() -> T
{
    m_semaphore.acquire();
    return pop();
}

template <typename T>
auto queue<T>::wait(const timeout& t) -> std::optional<T>
{
    return m_semaphore.try_acquire_for(t) ? pop() : std::optional<T>{};
}

template <typename T>
auto queue<T>::pop() -> T
{
    const lock l{m_mutex};

    T t = std::move(m_queue.front());
    m_queue.pop();

    return t;
}

} // namespace bln_queue
