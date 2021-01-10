#pragma once
#include "utils.hpp"

struct VertexBufferElement {
    unsigned int type, count;
    unsigned char normalized;
};

class VertexBufferLayout {

private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride;

public:

    VertexBufferLayout() : m_stride(0) {}
    ~VertexBufferLayout() {}

    template<typename T>
    void Push(unsigned int count);

    template<>
    void Push<float>(unsigned int count);

    template<>
    void Push<unsigned int>(unsigned int count);

    template<>
    void Push<unsigned char>(unsigned int count);

    inline unsigned int GetStride() const { return m_stride; }
    inline std::vector<VertexBufferElement> GetElements() const { return m_elements; }
};
