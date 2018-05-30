#pragma once
#include "util.hpp"

class VertexBuffer
{
public:
    VertexBuffer() {};
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();
    void init(const void* data, unsigned int size);
    void bind() const;
    void unbind() const;
private:
    GLuint m_rendererID;
};

