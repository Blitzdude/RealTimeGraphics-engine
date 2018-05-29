#pragma once
#include "util.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
private:
    GLuint m_rendererID;
};

