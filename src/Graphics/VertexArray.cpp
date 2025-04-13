#include "Graphics/VertexArray.hpp"

GLuint pl::VertexArray::vertexArray = 0;
std::array<GLuint, pl::VertexArray::VERTEX_BUFFER_COUNT> pl::VertexArray::vertexBuffers;
int pl::VertexArray::vertexBufferIndex = 0;

pl::VertexArray::VertexArray()
{
    primitiveMode = GL_TRIANGLES;
}

pl::VertexArray::~VertexArray()
{
}

void pl::VertexArray::addVertex(const Vertex& vertex)
{
    vertices.push_back(vertex);
}

void pl::VertexArray::addQuad(const Rect<float>& quad, const Color& color, const Rect<float>& textureUV)
{
    addVertex(Vertex(quad.getPosition(), color, textureUV.getPosition()));
    addVertex(Vertex(quad.getPosition() + Vector2f(quad.width, 0), color, textureUV.getPosition() + Vector2f(textureUV.width, 0)));
    addVertex(Vertex(quad.getPosition() + Vector2f(0, quad.height), color, textureUV.getPosition() + Vector2f(0, textureUV.height)));
    addVertex(Vertex(quad.getPosition() + Vector2f(quad.width, 0), color, textureUV.getPosition() + Vector2f(textureUV.width, 0)));
    addVertex(Vertex(quad.getPosition() + quad.getSize(), color, textureUV.getPosition() + textureUV.getSize()));
    addVertex(Vertex(quad.getPosition() + Vector2f(0, quad.height), color, textureUV.getPosition() + Vector2f(0, textureUV.height)));
}

void pl::VertexArray::setVertexData(const std::vector<Vertex> vertices)
{
    this->vertices = vertices;
}

void pl::VertexArray::appendVertexArray(const VertexArray& vertexArray)
{
    vertices.insert(vertices.end(), vertexArray.getVertexData().begin(), vertexArray.getVertexData().end());
}

void pl::VertexArray::clear()
{
    vertices.clear();
}

void pl::VertexArray::setPrimitiveMode(PrimitiveMode primitiveMode)
{
    switch (primitiveMode)
    {
        case PrimitiveMode::Triangles:
        {
            this->primitiveMode = GL_TRIANGLES;
            break;
        }
        case PrimitiveMode::Points:
        {
            this->primitiveMode = GL_POINTS;
            break;
        }
        case PrimitiveMode::Lines:
        {
            this->primitiveMode = GL_LINES;
            break;
        }
    }
}

void pl::VertexArray::reserve(uint32_t size)
{
    vertices.reserve(size);
}

void pl::VertexArray::resize(uint32_t size)
{
    vertices.resize(size);
}

const std::vector<pl::Vertex>& pl::VertexArray::getVertexData() const
{
    return vertices;
}

pl::Vertex& pl::VertexArray::operator[](uint32_t index)
{
    assert(index < vertices.size());
    return vertices[index];
}

int pl::VertexArray::size()
{
    return vertices.size();
}

void pl::VertexArray::initBuffers()
{
    glGenVertexArrays(1, &vertexArray);

    glBindVertexArray(vertexArray);
    
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(Vector2f)));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(Vector2f) + sizeof(Color)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (int i = 0; i < vertexBuffers.size(); i++)
    {
        glGenBuffers(1, &vertexBuffers[i]);
    }
}

void pl::VertexArray::draw(RenderTarget& renderTarget, const Texture* texture)
{
    if (vertexArray == 0)
    {
        initBuffers();
    }
    
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[vertexBufferIndex]);
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(primitiveMode, 0, vertices.size());

    vertexBufferIndex = (vertexBufferIndex + 1) % vertexBuffers.size();
}