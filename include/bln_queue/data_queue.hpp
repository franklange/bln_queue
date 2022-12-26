#pragma once

#include <boost/lockfree/queue.hpp>

#include <array>
#include <cstddef>
#include <optional>

namespace bln_queue {

template <typename T, std::size_t N>
class data_queue
{
public:
    auto put(T) -> bool;
    auto get()  -> std::optional<T>;

    template <std::size_t K>
    auto get(std::array<T, K>&) -> std::size_t;

private:
    using capacity = boost::lockfree::capacity<N>;
    using queue = boost::lockfree::queue<T, capacity>;

    queue m_queue;
};

template <typename T, std::size_t N>
auto data_queue<T, N>::put(T t) -> bool
{
    return m_queue.bounded_push(std::move(t));
}

template <typename T, std::size_t N>
auto data_queue<T, N>::get() -> std::optional<T>
{
    T tmp;
    return m_queue.pop(tmp) ? tmp : std::optional<T>{};
}

template <typename T, std::size_t N>
template <std::size_t K>
auto data_queue<T, N>::get(std::array<T, K>& arr) -> std::size_t
{
    for (unsigned i = 0; i < K; ++i)
        if (!m_queue.pop(arr[i])) return i;

    return arr.size();
}

} // namespace bln_queue
