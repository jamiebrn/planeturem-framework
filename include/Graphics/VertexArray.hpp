#pragma once

#include <extlib/gl.h>

#include <assert.h>
#include <vector>
#include <array>

#include "Graphics/Vertex.hpp"
#include "Graphics/PrimitiveMode.hpp"
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

    void setPrimitiveMode(PrimitiveMode primitiveMode);

    void reserve(uint32_t size);
    void resize(uint32_t size);

    const std::vector<Vertex>& getVertexData() const;

    Vertex& operator[](uint32_t index);

    int size();

    // Always call RenderTarget::draw() for correct bindings
    void draw(RenderTarget& renderTarget);

private:
    void initBuffers();

    std::vector<Vertex> vertices;

    uint32_t primitiveMode;

    static constexpr int VERTEX_BUFFER_COUNT = 3;

    static std::array<GLuint, VERTEX_BUFFER_COUNT> vertexArrays;
    static std::array<GLuint, VERTEX_BUFFER_COUNT> vertexBuffers;
    static int vertexBufferIndex;

};

}