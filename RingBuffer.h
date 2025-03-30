#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <mutex>
#include <thread>
#include <chrono>

template <typename T>
class RingBuffer
{
public:
    static RingBuffer &GetInstance()
    {
        static RingBuffer<T> instance;
        return instance;
    }
    ~RingBuffer()
    {
        if (m_inited)
        {
            delete[] m_buffer;
            m_inited = false;
        }
    }

    void CreateRingBuffer(size_t size)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_inited)
        {
            printf("RingBuffer already inited\n");
            return;
        }
        m_buffer = new T[size];
        m_size = size;
        m_readIndex.store(0, std::memory_order_relaxed);
        m_writeIndex.store(0, std::memory_order_relaxed);
        m_inited = true;

        printf("RingBuffer inited\n");
        return;
    }

    bool push(const T &data)
    {
        if (!m_inited)
        {
            printf("RingBuffer not inited\n");
            return false;
        }
        if (full())
        {
            printf("RingBuffer full\n");
            return false;
        }
        m_buffer[m_writeIndex.load(std::memory_order_relaxed)] = data;
        m_writeIndex.store((m_writeIndex.load(std::memory_order_relaxed) + 1) % m_size, std::memory_order_release);
        return true;
    }

    bool pop(T &data)
    {
        if (!m_inited)
        {
            printf("RingBuffer not inited\n");
            return false;
        }
        if (empty())
        {
            printf("RingBuffer empty\n");
            return false;
        }
        data = m_buffer[m_readIndex.load(std::memory_order_relaxed)];
        m_readIndex.store((m_readIndex.load(std::memory_order_relaxed) + 1) % m_size, std::memory_order_release);
        return true;
    }

    bool empty()
    {
        if (m_readIndex.load(std::memory_order_relaxed) == m_writeIndex.load(std::memory_order_acquire))
        {
            return true;
        }

        return false;
    }

    bool full()
    {
        if ((m_writeIndex.load(std::memory_order_relaxed) + 1) % m_size == m_readIndex.load(std::memory_order_acquire))
        {
            return true;
        }
        return false;
    }

private:
    RingBuffer() = default;
    RingBuffer(const RingBuffer &) = delete;

    RingBuffer &operator=(const RingBuffer &) = delete;

    std::atomic<size_t> m_readIndex;
    std::atomic<size_t> m_writeIndex;
    size_t m_size;
    T *m_buffer;
    bool m_inited;
    std::mutex m_mutex;
};
