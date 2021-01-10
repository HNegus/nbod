#include "vertexarray.hpp"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_renderer_id);
    glBindVertexArray(m_renderer_id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_renderer_id);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout layout) {
    vb.Bind();
    auto elements = layout.GetElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++) {
        auto element = elements[i];
        glVertexAttribPointer(i, element.count,
                              element.type,
                              element.normalized,
                              layout.GetStride(),
                              (const void*) offset);
        glEnableVertexAttribArray(i);
        offset += element.count * sizeof (element.type);
    }

}

void VertexArray::Bind() const {
    glBindVertexArray(m_renderer_id);
}

void VertexArray::UnBind() const {
    glBindVertexArray(0);
}


void VertexArray::Renew(const VertexBuffer& vb, const VertexBufferLayout layout) {
    glDeleteVertexArrays(1, &m_renderer_id);
    glGenVertexArrays(1, &m_renderer_id);
    glBindVertexArray(m_renderer_id);
    AddBuffer(vb, layout);
}
