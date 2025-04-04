#pragma once

#include <gl.h>

#include <vector>

#include "Vertex.hpp"
#include "Rect.hpp"
#include "RenderTarget.hpp"

namespace pl
{

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void addVertex(const Vertex& vertex);
    void addQuad(const Rect<float>& quad, const Color& color, const Rect<float>& textureUV);
    void setVertexData(const std::vector<Vertex> vertices);
    void appendVertexArray(const VertexArray& vertexArray);
    void clear();

    const std::vector<Vertex>& getVertexData() const;

    int size();

    void draw(RenderTarget& renderTarget) const;

private:
    std::vector<Vertex> vertices;

    // GLuint glVertexArray;
    // GLuint glVertexBuffer;

};

}