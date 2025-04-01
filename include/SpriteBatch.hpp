#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>

#include "Vector.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "RenderTarget.hpp"
#include "DrawData.hpp"
#include "Rect.hpp"
#include "VertexArray.hpp"

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

    Texture* batchTexture;
    Shader* batchShader;

};

}
