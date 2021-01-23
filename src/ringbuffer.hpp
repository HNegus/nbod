#pragma once

#include <vector>

template<class T>
class RingBuffer {

private:
    std::vector<T> m_data;
    const unsigned int m_size;

public:

    RingBuffer() : m_size(0) {};
    RingBuffer(unsigned int size) : m_size(size) {};
    ~RingBuffer() {};

    void Add(T val) {
        m_data.push_back(val);
        if (m_data.size() > m_size)
            m_data.erase(m_data.begin());
    };

    std::vector<T> GetAll() const { return m_data; };
};
