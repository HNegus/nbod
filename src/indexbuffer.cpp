#include "indexbuffer.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, const unsigned int count)
    : m_count(count)
{
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof (unsigned int), data, GL_STATIC_DRAW);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
}

void IndexBuffer::UnBind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Renew(const unsigned int *data, const unsigned int count) {
    glDeleteBuffers(1, &m_renderer_id);

    m_count = count;
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof (unsigned int), data, GL_STATIC_DRAW);
}

void IndexBuffer::Update(const unsigned int *data, const unsigned int count) {

    m_count = count;

    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof (unsigned int), data, GL_DYNAMIC_DRAW);
}

void IndexBuffer::Update(const unsigned int *data) const {

    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof (unsigned int), data, GL_DYNAMIC_DRAW);
}
