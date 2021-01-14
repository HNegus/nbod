#include "renderer.hpp"

void Renderer::Draw() const {}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib,
                    const Shader& shader) const {

    va.Bind();
    ib.Bind();
    shader.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawLines(const VertexArray& va, const Shader& shader) {

    va.Bind();
    shader.Bind();
    glDrawArrays(GL_LINES, 0, 16);
}


void Renderer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}
