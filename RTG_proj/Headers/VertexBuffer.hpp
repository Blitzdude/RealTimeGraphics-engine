#pragma once
#include "util.hpp"

class VertexBuffer
{
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
private:
    GLuint m_rendererID;
};

