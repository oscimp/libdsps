#ifndef QUEUE_H
#define QUEUE_H

#include <cassert>

#include <algorithm>
#include <cmath>
#include <memory>

class Queue {
public:
    /// \brief Constructor
    Queue()
    : m_capacity(InitialCapacity)
    , m_size(0)
    , m_head(0)
    , m_tail(0)
    {
        m_data = m_allocator.allocate(m_capacity);
    }

    /// \brief Destructor
    ~Queue() {
        m_allocator.deallocate(m_data, m_capacity);
    }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    Queue(Queue&&) = default;
    Queue& operator=(Queue&&) = default;

    /// \brief Check if the queue is empty
    ///
    /// \return True if the queue is empty
    bool isEmpty() const {
        return m_size == 0;
    }

    /// \brief Get the size of queue in bytes
    ///
    /// \return The number of bytes stored in the queue
    std::size_t size() const {
        return m_size;
    }


    /// \brief Send raw data in the queue
    ///
    /// \param raw Pointor of raw data
    /// \param size Number of bytes to push
    void push(const void *raw, std::size_t size) {
        // We grow the queue if needed
        while (m_size + size >= m_capacity) {
            grow();
        }

        // Check if the capacity is correct
        assert(m_size + size < m_capacity);

        // Push data only to the end
        if (m_tail + size <= m_capacity) {
            std::copy_n(static_cast<const uint8_t*>(raw), size, m_data + m_tail);
            m_tail += size;

            // If the end is reached
            if (m_tail == m_capacity) {
                m_tail = 0;
            }
        }
        // Push data to the end and to the begin
        else {
            std::size_t first = m_capacity - m_tail;
            std::size_t second = size - first;

            std::copy_n(static_cast<const uint8_t*>(raw), first, m_data + m_tail);
            std::copy_n(static_cast<const uint8_t*>(raw) + first, second, m_data);

            m_tail = second;
            assert(m_tail < m_capacity);
            assert(m_tail < m_head);
        }

        // Update the size and check sanity
        m_size += size;
        assert(invariant());
    }

    /// \brief Get the data to send into raw pointor
    ///
    /// \param raw Pointor to send data
    /// \param size Number of bytes to send
    void pop(void *raw, std::size_t size) {
        // Check if he data is avaible
        assert(size <= m_size);

        // If we read the data in one read
        if (m_head + size <= m_capacity) {
            std::copy_n(m_data + m_head, size, static_cast<uint8_t*>(raw));
            m_head += size;

            // If the end is reached
            if (m_head == m_capacity) {
                m_head = 0;
            }
        }
        // If we read the data in two reads
        else {
            std::size_t first = m_capacity - m_head;
            std::size_t second = size - first;

            std::copy_n(m_data + m_head, first, static_cast<uint8_t*>(raw));
            std::copy_n(m_data, second, static_cast<uint8_t*>(raw) + first);

            m_head = second;
            assert(m_head < m_capacity);
            assert(m_head <= m_tail);
        }

        // Update the size and check sanity
        m_size -= size;
        assert(invariant());

        // Shrink
        while (m_size < std::floor(ShrinkLimit * m_capacity) && m_capacity > InitialCapacity) {
            shrink();
        }
    }

private:
    void grow() {
        std::size_t capacity = m_capacity * GrowLimit;
        auto data = m_allocator.allocate(capacity);

        if (m_size > 0) {
            if (m_head < m_tail) {
                std::copy_n(m_data + m_head, m_size, data);
            }
            else {
                std::size_t first = m_capacity - m_head;
                std::size_t second = m_size - first;
                std::copy_n(m_data + m_head, first, data);
                std::copy_n(m_data, second, data + first);
            }
        }

        m_head = 0;
        m_tail = m_size;

        m_allocator.deallocate(m_data, m_capacity);
        m_data = data;
        m_capacity = capacity;
        assert(invariant());
    }

    void shrink() {
        std::size_t capacity = std::floor(ShrinkLimit * m_capacity);
        auto data = m_allocator.allocate(capacity);

        if (m_size > 0) {
            if (m_head < m_tail) {
                std::copy_n(m_data + m_head, m_size, data);
            }
            else {
                std::size_t first = m_capacity - m_head;
                std::size_t second = m_size - first;
                std::copy_n(m_data + m_head, first, data);
                std::copy_n(m_data, second, data + first);
            }
        }

        m_head = 0;
        m_tail = m_size;

        m_allocator.deallocate(m_data, m_capacity);
        m_data = data;
        m_capacity = capacity;
        assert(invariant());
    }

    bool invariant() const {
        return m_size == computedSize();
    }

    std::size_t computedSize() const {
        if (m_head <= m_tail) {
            return m_tail - m_head;
        }

        return m_capacity - m_head + m_tail;
    }

private:
    static constexpr std::size_t InitialCapacity = sizeof(double) * 4097;
    static constexpr double GrowLimit = 1.10;
    static constexpr double ShrinkLimit = 0.04;

    std::allocator<uint8_t> m_allocator;
    std::size_t m_capacity;
    std::size_t m_size;
    std::size_t m_head;
    std::size_t m_tail;
    uint8_t *m_data;
};

#endif // QUEUE_H
