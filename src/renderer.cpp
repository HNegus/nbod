#include "renderer.hpp"

void Renderer::Draw() const {}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib,
                    const Shader& shader) const {

    va.Bind();
    ib.Bind();
    shader.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawLineStrip(const VertexArray& va, const unsigned int count,
                         const Shader& shader) const {

    va.Bind();
    shader.Bind();
    glDrawArrays(GL_LINES, 0, count);
}


void Renderer::DrawLineStrip(const VertexArray& va, const IndexBuffer& ib,
                         const Shader& shader) const {


    va.Bind();
    ib.Bind();
    shader.Bind();
    glDrawElements(GL_LINE_STRIP, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

}

void Renderer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}
