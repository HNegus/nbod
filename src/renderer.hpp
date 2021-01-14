#pragma once

#include "utils.hpp"

#include "vertexarray.hpp"
#include "indexbuffer.hpp"
#include "shader.hpp"

#include "gui.hpp"

class Renderer {
private:

public:
    Renderer() {};
    ~Renderer() {};


    void Draw() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib,
              const Shader& shader) const;
    void DrawLines(const VertexArray& va, const Shader& shader);

    void Clear() const;
};
