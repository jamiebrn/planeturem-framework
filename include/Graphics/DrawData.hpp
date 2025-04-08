#pragma once

#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Vector.hpp"
#include "Graphics/Color.hpp"
#include "Rect.hpp"

namespace pl
{

struct DrawData
{
    Shader* shader;
    Texture* texture;
    
    Vector2f position;
    float rotation = 0.0f;
    Vector2f scale;
    Vector2f centerRatio = Vector2f(0, 0);
    Color color = Color(255, 255, 255);
    Rect<int> textureRect;

    // Use centre "ratio" in pixel coordinates
    bool useCentreAbsolute = false;
};

}