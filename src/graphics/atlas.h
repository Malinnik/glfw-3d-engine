#pragma once

#include "graphics/texture.h"

class Atlas
{
    public:
        static Texture* textureAtlas;
        static void build();
};