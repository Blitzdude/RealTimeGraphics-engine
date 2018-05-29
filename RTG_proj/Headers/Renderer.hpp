#pragma once
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "util.hpp"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void setClearColor(float r, float g, float b, float a);
    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void bind() const;
    void unbind() const;
private:
    
};

