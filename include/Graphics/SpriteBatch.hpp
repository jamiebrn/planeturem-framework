#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>

#include "Vector.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/RenderTarget.hpp"
#include "Graphics/DrawData.hpp"
#include "Rect.hpp"
#include "Graphics/VertexArray.hpp"

namespace pl
{

class SpriteBatch
{
public:
    SpriteBatch();

    void beginDrawing();

    void draw(RenderTarget& renderTarget, const DrawData& drawData);

    void endDrawing(RenderTarget& renderTarget);

private:
    void drawVertexArray(RenderTarget& renderTarget);

    void resetBatchValues();

private:
    VertexArray vertexArray;

    const Texture* batchTexture;
    const Shader* batchShader;

};

}
