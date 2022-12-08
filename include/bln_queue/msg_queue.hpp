#pragma once

#include <bln_queue/types.hpp>

#include <boost/fiber/fiber.hpp>
#include <boost/fiber/buffered_channel.hpp>

#include <cstddef>
#include <optional>
#include <utility>

namespace bln_queue {

template <typename T>
class msg_queue
{
    using status = boost::fibers::channel_op_status;
    using queue  = boost::fibers::buffered_channel<T>;

public:
    using write_type = T;
    using read_type = T;

    msg_queue(std::size_t = 128);

    template <typename W = T>
    auto put(W&&) -> bool;

    auto get() -> std::optional<T>;

    auto wait() -> T;
    auto wait(const timeout&) -> std::optional<T>;

private:
    queue m_queue;
};

template <typename T>
msg_queue<T>::msg_queue(const std::size_t s)
    : m_queue{s}
{}

template <typename T>
template <typename W>
auto msg_queue<T>::put(W&& w) -> bool
{
    return (status::success == m_queue.try_push(std::forward<W>(w)));
}

template <typename T>
auto msg_queue<T>::get() -> std::optional<T>
{
    T tmp;
    return (status::success == m_queue.try_pop(tmp))
        ? tmp
        : std::optional<T>{};
}

template <typename T>
auto msg_queue<T>::wait() -> T
{
    return m_queue.value_pop();
}

template <typename T>
auto msg_queue<T>::wait(const timeout& t) -> std::optional<T>
{
    T tmp;
    return (status::success == m_queue.pop_wait_for(tmp, t))
        ? tmp
        : std::optional<T>{};
}

} // namespace bln_queue
