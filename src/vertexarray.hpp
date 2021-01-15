#pragma once

#include "utils.hpp"
#include "vertexbuffer.hpp"
#include "vertexbufferlayout.hpp"

class VertexArray {

private:
    unsigned int m_renderer_id;

public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout layout);
    void AddBuffer(const VertexBuffer& vb, const unsigned int index, const VertexBufferLayout layout);

    void Bind() const;
    void UnBind() const;
    void Renew(const VertexBuffer& vb, const VertexBufferLayout layout);
    void Update(const VertexBuffer& vb, const VertexBufferLayout layout);
    void Update(const VertexBuffer& vb) const;

};
