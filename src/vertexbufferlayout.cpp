#include "vertexbufferlayout.hpp"


template<typename T>
void VertexBufferLayout::Push(unsigned int count){
    // static_assert(false);
    (void) count;
}


template<>
void VertexBufferLayout::Push<float>(unsigned int count) {
    m_elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_stride += count * sizeof (unsigned int);
}


template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
    m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_stride += count * sizeof (unsigned int);
}

template<>
void VertexBufferLayout::Push<unsigned char>(unsigned int count) {
    m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_stride += count * sizeof (unsigned char);
}
