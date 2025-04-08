#include "Graphics/SpriteBatch.hpp"

pl::SpriteBatch::SpriteBatch()
{
    resetBatchValues();
}

void pl::SpriteBatch::beginDrawing()
{
    resetBatchValues();
}

void pl::SpriteBatch::draw(RenderTarget& window, const DrawData& drawData)
{
    if (batchTexture && batchShader)
    {
        if (batchTexture != drawData.texture || batchShader != drawData.shader)
        {
            // End current batch and start new
            endDrawing(window);
            batchTexture = drawData.texture;
            batchShader = drawData.shader;
        }
    }
    else
    {
        batchTexture = drawData.texture;
        batchShader = drawData.shader;

        if (vertexArray.size() > 0)
        {
            printf("WARNING: SpriteBatch batch type has been reset mid-batch. Resetting data\n");
            vertexArray.clear();
        }
    }

    Vertex vertices[4];
    
    Vector2f size;
    size.x = drawData.textureRect.width * drawData.scale.x;
    size.y = drawData.textureRect.height * drawData.scale.y;

    float centreRatioX = drawData.centerRatio.x;
    float centreRatioY = drawData.centerRatio.y;

    if (drawData.useCentreAbsolute)
    {
        centreRatioX /= drawData.textureRect.width;
        centreRatioY /= drawData.textureRect.height;
    }
    
    if (drawData.rotation == 0)
    {
        // Simple case, no rotation
        // Separate from rotation calculation in order to save performance
        Vector2f topLeft;
        topLeft.x = drawData.position.x - (size.x * centreRatioX);
        topLeft.y = drawData.position.y - (size.y * centreRatioY);

        vertices[0].position = topLeft;
        vertices[1].position = topLeft + Vector2f(size.x, 0);
        vertices[2].position = topLeft + Vector2f(size.x, size.y);
        vertices[3].position = topLeft + Vector2f(0, size.y);
    }
    else
    {
        // Apply rotation
        float angleRadians = M_PI * drawData.rotation / 180.0f;

        float nX = -size.x * centreRatioX;
        float pX = (1.0f - centreRatioX) * size.x;
        float nY = -size.y * centreRatioY;
        float pY = (1.0f - centreRatioY) * size.y;

        vertices[0].position = Vector2f(nX, nY).rotate(angleRadians) + drawData.position;
        vertices[1].position = Vector2f(pX, nY).rotate(angleRadians) + drawData.position;
        vertices[2].position = Vector2f(pX, pY).rotate(angleRadians) + drawData.position;
        vertices[3].position = Vector2f(nX, pY).rotate(angleRadians) + drawData.position;
    }

    // Set UV coords
    vertices[0].textureUV = static_cast<Vector2f>(drawData.textureRect.getPosition());
    vertices[1].textureUV = vertices[0].textureUV + Vector2f(drawData.textureRect.width, 0);
    vertices[2].textureUV = vertices[0].textureUV + Vector2f(drawData.textureRect.width, drawData.textureRect.height);
    vertices[3].textureUV = vertices[0].textureUV + Vector2f(0, drawData.textureRect.height);

    Color colorNormalised = drawData.colour.normalise();
    for (int i = 0; i < 4; i++)
    {
        vertices[i].color = colorNormalised;
    }
    
    vertexArray.addVertex(vertices[0]);
    vertexArray.addVertex(vertices[1]);
    vertexArray.addVertex(vertices[2]);
    vertexArray.addVertex(vertices[0]);
    vertexArray.addVertex(vertices[2]);
    vertexArray.addVertex(vertices[3]);
}

void pl::SpriteBatch::endDrawing(RenderTarget& window)
{
    drawVertexArray(window);
    resetBatchValues();
}

void pl::SpriteBatch::drawVertexArray(RenderTarget& window)
{
    if (!batchTexture || !batchShader)
    {
        return;
    }

    window.draw(vertexArray, *batchShader, batchTexture, BlendMode::Alpha);
}

void pl::SpriteBatch::resetBatchValues()
{
    vertexArray.clear();
    batchTexture = nullptr;
    batchShader = nullptr;
}