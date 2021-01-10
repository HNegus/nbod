#include "vertexbuffer.hpp"
#include "renderer.hpp"

VertexBuffer::VertexBuffer(const void* data, int size)
    : m_size(size)
{
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
}

void VertexBuffer::UnBind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Renew(const void* data, const int size)  {

    glDeleteBuffers(1, &m_renderer_id);

    m_size = size;
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ARRAY_BUFFER, m_size, data, GL_STATIC_DRAW);
}

void VertexBuffer::Update(const void* data, const int size)  {

    m_size = size;

    Bind();
    glBufferData(GL_ARRAY_BUFFER, m_size, data, GL_DYNAMIC_DRAW);
}

void VertexBuffer::Update(const void* data) const {

    Bind();
    glBufferData(GL_ARRAY_BUFFER, m_size, data, GL_DYNAMIC_DRAW);
}
