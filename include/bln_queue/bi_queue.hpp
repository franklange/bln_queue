#pragma once

#include <bln_queue/msg_queue.hpp>

#include <cstddef>
#include <optional>
#include <utility>

namespace bln_queue {

template <typename W, typename R>
class bi_queue
{
public:
    using write_type = W;
    using read_type = R;

    bi_queue(msg_queue<W>&, msg_queue<R>&);

    template <typename U = W>
    auto put(U&&) -> bool;

    auto get() -> std::optional<R>;

    auto wait() -> R;
    auto wait(const timeout&) -> std::optional<R>;

private:
    msg_queue<W>& m_write;
    msg_queue<R>& m_read;
};

template <typename W, typename R = W>
class BiQueue
{
public:
    using A = bi_queue<W, R>;
    using B = bi_queue<R, W>;

    BiQueue(std::size_t = 128);

    A sideA{m_qa, m_qb};
    B sideB{m_qb, m_qa};

private:
    msg_queue<W> m_qa;
    msg_queue<R> m_qb;
};

template <typename W, typename R>
BiQueue<W, R>::BiQueue(const std::size_t s)
    : m_qa{s}
    , m_qb{s}
{}

template <typename W, typename R>
bi_queue<W, R>::bi_queue(msg_queue<W>& w, msg_queue<R>& r)
    : m_write{w}
    , m_read{r}
{}

template <typename W, typename R>
template <typename U>
auto bi_queue<W, R>::put(U&& u) -> bool
{
    return m_write.put(std::forward<U>(u));
}

template <typename W, typename R>
auto bi_queue<W, R>::get() -> std::optional<R>
{
    return m_read.get();
}

template <typename W, typename R>
auto bi_queue<W, R>::wait() -> R
{
    return m_read.wait();
}

template <typename W, typename R>
auto bi_queue<W, R>::wait(const timeout& t) -> std::optional<R>
{
    return m_read.wait(t);
}

} // namespace bln_queue
