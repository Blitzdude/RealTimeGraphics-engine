#pragma once

#include "util.hpp"

class IndexBuffer
{
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
    inline const unsigned int& getCount() const { return m_count; };
private:
    GLuint m_rendererID;
    unsigned int m_count;
};

