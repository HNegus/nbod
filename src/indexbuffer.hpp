#pragma once
#include "utils.hpp"

class IndexBuffer {

private:
    unsigned int m_renderer_id, m_count;

public:

    IndexBuffer() : m_renderer_id(0), m_count(0) {}
    IndexBuffer(const unsigned int* data, const unsigned int count);
    ~IndexBuffer() { glDeleteBuffers(1, &m_renderer_id); }

    void Bind() const;
    void UnBind() const;
    void Renew(const unsigned int* data, const unsigned int count);
    void Update(const unsigned int *data, const unsigned int count);
    void Update(const unsigned int* data) const;


    inline unsigned int GetCount() const { return m_count; };
};
