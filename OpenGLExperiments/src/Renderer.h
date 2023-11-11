#pragma once

#include <GL/glew.h>

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

class Renderer {
public:
    Renderer() {};
    ~Renderer() {};

    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};