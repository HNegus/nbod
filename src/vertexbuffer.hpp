#pragma once
#include "utils.hpp"

class VertexBuffer {

private:
    unsigned int m_renderer_id, m_size;

public:
    VertexBuffer();
    VertexBuffer(const void* data, int size);
    ~VertexBuffer() { glDeleteBuffers(1, &m_renderer_id); }

    void Bind() const;
    void UnBind() const;
    void Renew(const void* data, const int size);
    void Update(const void* data, const int size);
    void Update(const void* data) const;

    float GetData();

};
