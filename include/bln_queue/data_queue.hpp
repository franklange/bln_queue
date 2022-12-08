#pragma once

#include <boost/lockfree/queue.hpp>

#include <array>
#include <cstddef>
#include <optional>
#include <utility>

namespace bln_queue {

template <typename T, std::size_t N>
class data_queue
{
    using queue = boost::lockfree::queue<T, boost::lockfree::capacity<N>>;

public:
    using write_type = T;
    using read_type = T;

    template <typename W = T>
    auto put(W&&) -> bool;

    auto get() -> std::optional<T>;

    template <std::size_t K>
    auto get(std::array<T, K>&) -> std::size_t;

private:
    queue m_queue;
};

template <typename T, std::size_t N>
template <typename W>
auto data_queue<T, N>::put(W&& w) -> bool
{
    return m_queue.bounded_push(std::forward<W>(w));
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
