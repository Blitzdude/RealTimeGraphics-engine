#pragma once

#include "util.hpp"

class IndexBuffer
{
public:
    IndexBuffer() {};
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void init(const unsigned int* data, unsigned int count);
    void bind() const;
    void unbind() const;
    inline const unsigned int& getCount() const { return m_count; };
private:
    GLuint m_rendererID;
    unsigned int m_count;
};

