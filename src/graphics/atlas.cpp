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
    // Соберём все тайлы в память (чтобы потом генерировать mip-уровни per-tile)
    struct Tile { int id; std::vector<unsigned char> data; };
    std::vector<Tile> tiles;
    tiles.reserve(resources::REGISTRY.size());

    for (auto const& [id, textureName] : resources::REGISTRY)
    {
        int width, height;
        unsigned char* textureData = load_png_data(textureName.c_str(), &width, &height);
        if (!textureData) continue;

        // Копируем в буфер размера TILE_SIZE x TILE_SIZE, учитывая реальные размеры изображения
        std::vector<unsigned char> tileVec(TILE_SIZE * TILE_SIZE * 4, 0);
        int copyW = std::min(width, TILE_SIZE);
        int copyH = std::min(height, TILE_SIZE);
        for (int y = 0; y < copyH; y++) {
            for (int x = 0; x < copyW; x++) {
                int srcIdx = (y * width + x) * 4;
                int dstIdx = (y * TILE_SIZE + x) * 4;
                tileVec[dstIdx + 0] = textureData[srcIdx + 0];
                tileVec[dstIdx + 1] = textureData[srcIdx + 1];
                tileVec[dstIdx + 2] = textureData[srcIdx + 2];
                tileVec[dstIdx + 3] = textureData[srcIdx + 3];
            }
        }
        delete[] textureData;

        tiles.push_back({id, std::move(tileVec)});
    }

    // Level 0 atlas
    std::vector<unsigned char> atlasData(ATLAS_SIZE * ATLAS_SIZE * 4, 0);
    LOG_F(INFO, "Atlas: tiles to pack = %zu, atlas slots = %d", tiles.size(), TEXTURES_PER_ROW * TEXTURES_PER_ROW);
    for (const auto &t : tiles) {
        int id = t.id;
        const std::vector<unsigned char> &textureData = t.data;
        int gridX = id % TEXTURES_PER_ROW;
        int gridY = id / TEXTURES_PER_ROW;
        if (gridY >= TEXTURES_PER_ROW || gridX >= TEXTURES_PER_ROW) {
            LOG_F(WARNING, "Atlas: resource id %d does not fit into atlas (%d x %d), skipping", id, TEXTURES_PER_ROW, TEXTURES_PER_ROW);
            continue;
        }
        int pixelX = gridX * TILE_SIZE;
        int pixelY = gridY * TILE_SIZE;
        for (int y = 0; y < TILE_SIZE; y++) {
            for (int x = 0; x < TILE_SIZE; x++) {
                int srcY = y; // PNG loader already flips rows
                int srcIdx = (srcY * TILE_SIZE + x) * 4;
                int dstIdx = ((pixelY + y) * ATLAS_SIZE + (pixelX + x)) * 4;
                atlasData[dstIdx] = textureData[srcIdx];
                atlasData[dstIdx + 1] = textureData[srcIdx + 1];
                atlasData[dstIdx + 2] = textureData[srcIdx + 2];
                atlasData[dstIdx + 3] = textureData[srcIdx + 3];
            }
        }
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Настройки текстур
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Загружаем level 0
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ATLAS_SIZE, ATLAS_SIZE, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, atlasData.data());

    // Сгенерируем mip-уровни вручную per-tile, чтобы исключить смешение соседних тайлов
    int maxLevel = 0;
    int tmp = ATLAS_SIZE;
    while (tmp > 1) { tmp >>= 1; maxLevel++; }

    int uploadedMaxLevel = 0;
    for (int level = 1; level <= maxLevel; level++) {
        int tileMipSize = TILE_SIZE >> level; // integer division by 2^level
        if (tileMipSize == 0) break; // can't represent per-tile mips below 1px

        int mipSize = TEXTURES_PER_ROW * tileMipSize; // should equal ATLAS_SIZE >> level
        std::vector<unsigned char> mipData(mipSize * mipSize * 4, 0);

        int blockSize = 1 << level;

        for (const auto &t : tiles) {
            int id = t.id;
            const std::vector<unsigned char> &tileSrc = t.data; // TILE_SIZE x TILE_SIZE

            int gridX = id % TEXTURES_PER_ROW;
            int gridY = id / TEXTURES_PER_ROW;

            for (int ty = 0; ty < tileMipSize; ty++) {
                for (int tx = 0; tx < tileMipSize; tx++) {
                    // Average block in source tile
                    int srcStartX = tx * blockSize;
                    int srcStartY = ty * blockSize;
                    int srcEndX = std::min(srcStartX + blockSize, TILE_SIZE);
                    int srcEndY = std::min(srcStartY + blockSize, TILE_SIZE);

                    uint32_t sumR = 0, sumG = 0, sumB = 0, sumA = 0;
                    int count = 0;
                    for (int sy = srcStartY; sy < srcEndY; sy++) {
                        for (int sx = srcStartX; sx < srcEndX; sx++) {
                            int sIdx = (sy * TILE_SIZE + sx) * 4;
                            sumR += tileSrc[sIdx + 0];
                            sumG += tileSrc[sIdx + 1];
                            sumB += tileSrc[sIdx + 2];
                            sumA += tileSrc[sIdx + 3];
                            count++;
                        }
                    }
                    if (count == 0) count = 1;
                    unsigned char r = sumR / count;
                    unsigned char g = sumG / count;
                    unsigned char b = sumB / count;
                    unsigned char a = sumA / count;

                    int dstX = gridX * tileMipSize + tx;
                    int dstY = gridY * tileMipSize + ty;
                    if (dstX < 0 || dstX >= mipSize || dstY < 0 || dstY >= mipSize) continue;
                    int dstIdx = (dstY * mipSize + dstX) * 4;
                    mipData[dstIdx + 0] = r;
                    mipData[dstIdx + 1] = g;
                    mipData[dstIdx + 2] = b;
                    mipData[dstIdx + 3] = a;
                }
            }
        }

        glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, mipSize, mipSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, mipData.data());
        uploadedMaxLevel = level;
    }

    // Ограничим макс уровень до фактически загруженных
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, uploadedMaxLevel > 0 ? uploadedMaxLevel : 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Сохраняем текстуру атласа
    textureAtlas = new Texture(textureID, ATLAS_SIZE, ATLAS_SIZE);

    LOG_F(INFO, "Atlas built successfully!");
}