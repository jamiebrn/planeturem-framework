#pragma once

#include <extlib/gl.h>

#include <assert.h>
#include <vector>

#include "Graphics/Vertex.hpp"
#include "Graphics/RenderTarget.hpp"
#include "Graphics/Texture.hpp"
#include "Rect.hpp"

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

    void reserve(uint32_t size);
    void resize(uint32_t size);

    const std::vector<Vertex>& getVertexData() const;

    Vertex& operator[](uint32_t index);

    int size();

    void draw(RenderTarget& renderTarget, const Texture* texture) const;

private:
    std::vector<Vertex> vertices;

    // GLuint glVertexArray;
    // GLuint glVertexBuffer;

};

}