#include "VertexArray.hpp"

pl::VertexArray::VertexArray()
{
    glGenVertexArrays(1, &glVertexArray);
    glGenBuffers(1, &glVertexBuffer);

    glBindVertexArray(glVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, glVertexBuffer);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(Vector2f)));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(Vector2f) + sizeof(Color)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

pl::VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &glVertexArray);
    glDeleteBuffers(1, &glVertexBuffer);
}

void pl::VertexArray::addVertex(const Vertex& vertex)
{
    vertices.push_back(vertex);
}

void pl::VertexArray::setVertexData(const std::vector<Vertex> vertices)
{
    this->vertices = vertices;
}

void pl::VertexArray::clear()
{
    vertices.clear();
}

int pl::VertexArray::size()
{
    return vertices.size();
}

void pl::VertexArray::draw(RenderTarget& renderTarget) const
{
    std::vector<Vertex> transformedVertices = vertices;

    float halfTargetWidth = renderTarget.getWidth() / 2.0f;
    float halfTargetHeight = renderTarget.getHeight() / 2.0f;

    for (Vertex& vertex : transformedVertices)
    {
        vertex.position.x = (vertex.position.x - halfTargetWidth) / halfTargetWidth;
        vertex.position.y = -(vertex.position.y - halfTargetHeight) / halfTargetHeight;
    }

    glBindBuffer(GL_ARRAY_BUFFER, glVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, transformedVertices.size() * sizeof(Vertex), transformedVertices.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(glVertexArray);

    glDrawArrays(GL_TRIANGLES, 0, transformedVertices.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}