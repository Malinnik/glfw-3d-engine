#include "atlas.h"

#include "glad/glad.h"
#include "loguru.hpp"
#include "fmt/format.h"
#include <vector>

#include "loaders/image/png_loader.h"
#include "engine/resources/resource.h"
#include "files/file.h"

#define ATLAS_SIZE 256
#define TILE_SIZE 16
#define TEXTURES_PER_ROW (ATLAS_SIZE / TILE_SIZE)

Texture* Atlas::textureAtlas = nullptr;

void Atlas::build()
{
    textureAtlas = nullptr;
    std::vector<unsigned char> atlasData(ATLAS_SIZE * ATLAS_SIZE * 4, 0);

    for (auto const& [id, textureName] : resources::REGISTRY)
    {
        int width, height;
        unsigned char* textureData = load_png_data(textureName.c_str(), &width, &height);

        // Вычисляем позицию в атласе
        // id=0 -> (0, 0) - левый верхний угол
        // id=1 -> (16, 0) - правее
        // id=16 -> (0, 16) - ниже первой строки
        int gridX = id % TEXTURES_PER_ROW;
        int gridY = id / TEXTURES_PER_ROW;
        
        int pixelX = gridX * TILE_SIZE;
        int pixelY = gridY * TILE_SIZE;  // pixelY увеличивается ВНИЗ


        for (int y = 0; y < TILE_SIZE; y++) {
            for (int x = 0; x < TILE_SIZE; x++) {
                int srcY = TILE_SIZE - 1 - y;  // Инвертируем Y
                int srcIdx = (srcY * TILE_SIZE + x) * 4;
                int dstIdx = ((pixelY + y) * ATLAS_SIZE + (pixelX + x)) * 4;

                atlasData[dstIdx] = textureData[srcIdx];     // R
                atlasData[dstIdx + 1] = textureData[srcIdx + 1]; // G
                atlasData[dstIdx + 2] = textureData[srcIdx + 2]; // B
                atlasData[dstIdx + 3] = textureData[srcIdx + 3]; // A
            }
        }

        delete[] textureData;
        // LOG_F(INFO, fmt::format("RESOURCE IN ATLAS: {}: {}", key, value).c_str());
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Настройки текстур
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Загружаем данные
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ATLAS_SIZE, ATLAS_SIZE, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, atlasData.data());
    
    // Генерируем мипмапы
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Сохраняем текстуру атласа
    textureAtlas = new Texture(textureID, ATLAS_SIZE, ATLAS_SIZE);
    
    LOG_F(INFO, "Atlas built successfully!");
}