#include "Graphics/VertexArray.hpp"

GLuint pl::VertexArray::vertexArray = 0;
GLuint pl::VertexArray::vertexBuffer = 0;

pl::VertexArray::VertexArray()
{
    primitiveMode = GL_TRIANGLES;
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
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(Vector2f)));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(Vector2f) + sizeof(Color)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
}

void pl::VertexArray::draw(RenderTarget& renderTarget, const Texture* texture)
{
    std::vector<Vertex> transformedVertices = vertices;
    
    float halfTargetWidth = renderTarget.getWidth() / 2.0f;
    float halfTargetHeight = renderTarget.getHeight() / 2.0f;
    
    for (Vertex& vertex : transformedVertices)
    {
        vertex.position.x = (vertex.position.x - halfTargetWidth) / halfTargetWidth;
        vertex.position.y = -(vertex.position.y - halfTargetHeight) / halfTargetHeight;
        if (texture)
        {
            vertex.textureUV.x /= texture->getWidth();
            vertex.textureUV.y /= texture->getHeight();
        }
        vertex.color = vertex.color.normalise();
    }
    
    if (vertexArray == 0 || vertexBuffer == 0)
    {
        initBuffers();
    }
    
    glBindVertexArray(vertexArray);
    
    glBufferData(GL_ARRAY_BUFFER, transformedVertices.size() * sizeof(Vertex), transformedVertices.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(primitiveMode, 0, transformedVertices.size());
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}