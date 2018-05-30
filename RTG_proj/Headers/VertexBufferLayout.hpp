#pragma once
#include "util.hpp"

#include <vector>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        assert(false);
        return 0;
    }
};

class VertexBufferLayout // Must be expanded for each type we support
{
public:
    VertexBufferLayout();
    ~VertexBufferLayout();

    template<typename T>
    void push(unsigned int count)
    {
        static_assert(false);
    }

    template<>
    void push<float>(unsigned int count)
    {
        m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
        m_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
    }

    template<>
    void push<unsigned int>(unsigned int count)
    {
        m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void push<unsigned char>(unsigned int count)
    {
        m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });       
        m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
    }

    template<>
    void push<glm::vec2>(unsigned int count) // TODO: Mod to use float push
    {
        m_elements.push_back({ GL_FLOAT, count * 2, GL_TRUE });
        m_stride += count * 2 * VertexBufferElement::getSizeOfType(GL_FLOAT);
    }

    template<>
    void push<glm::vec3>(unsigned int count) // TODO: Mod to use float push
    {
        m_elements.push_back({ GL_FLOAT, count * 3, GL_FALSE });
        m_stride += count * 3 * VertexBufferElement::getSizeOfType(GL_FLOAT);
    }

    inline const std::vector<VertexBufferElement>& getElements() const { return m_elements; };
    inline unsigned int getStride() const { return m_stride; }

private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride;
};

