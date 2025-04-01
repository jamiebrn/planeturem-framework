#pragma once

#include <gl.h>

#include <vector>

#include "Vertex.hpp"
#include "RenderTarget.hpp"

namespace pl
{

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void addVertex(const Vertex& vertex);
    void setVertexData(const std::vector<Vertex> vertices);
    void clear();

    int size();

    void draw(RenderTarget& renderTarget) const;

private:
    std::vector<Vertex> vertices;

    // GLuint glVertexArray;
    // GLuint glVertexBuffer;

};

}