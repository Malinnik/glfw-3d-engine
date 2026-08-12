#include "WorldFiles.h"
#include "file.h"
#include "../world/chunk.h"
#include <cassert>
#include <fstream>
#include <loguru.hpp>

// Размер данных чанка в байтах (CHUNK_BLOCKS элементов по 4 байта каждый)
static const unsigned int CHUNK_DATA_SIZE = CHUNK_BLOCKS * sizeof(unsigned int);

// ===== RLE Компрессия =====

static unsigned int compressRLE(const char* src, unsigned int srcSize, char* dst) {
    assert(src != nullptr && dst != nullptr && srcSize > 0);

    unsigned int dstPos = 0;
    unsigned int srcPos = 0;

    while (srcPos < srcSize) {
        char value = src[srcPos];
        unsigned int count = 1;

        while (srcPos + count < srcSize && src[srcPos + count] == value && count < 255) {
            count++;
        }

        dst[dstPos++] = (char)count;
        dst[dstPos++] = value;
        srcPos += count;
    }

    return dstPos;
}

static void decompressRLE(const char* src, unsigned int srcSize, char* dst, unsigned int dstSize) {
    assert(src != nullptr && dst != nullptr);

    unsigned int srcPos = 0;
    unsigned int dstPos = 0;

    while (srcPos < srcSize && dstPos < dstSize) {
        unsigned char count = (unsigned char)src[srcPos++];
        char value = src[srcPos++];

        for (unsigned int i = 0; i < count && dstPos < dstSize; i++) {
            dst[dstPos++] = value;
        }
    }
}

// ===== Вспомогательные функции для работы с байтами =====

static int bytes2Int(const unsigned char* src, unsigned int offset) {
    return (src[offset] << 24) | (src[offset + 1] << 16)
         | (src[offset + 2] << 8) | (src[offset + 3]);
}

static void int2Bytes(int value, char* dest, unsigned int offset) {
    dest[offset] = (char)((value >> 24) & 0xFF);
    dest[offset + 1] = (char)((value >> 16) & 0xFF);
    dest[offset + 2] = (char)((value >> 8) & 0xFF);
    dest[offset + 3] = (char)(value & 0xFF);
}

// ===== Конструктор и деструктор =====

WorldFiles::WorldFiles(std::string directory, size_t mainBufferCapacity)
    : directory(directory) {
    mainBuffer = new char[mainBufferCapacity];
    LOG_F(INFO, "WorldFiles initialized: directory='%s', buffer=%zu MB",
          directory.c_str(), mainBufferCapacity / (1024 * 1024));
}

WorldFiles::~WorldFiles() {
    delete[] mainBuffer;

    for (auto& pair : regions) {
        char** region = pair.second;
        if (region == nullptr)
            continue;

        for (unsigned int i = 0; i < REGION_VOL; i++) {
            if (region[i] != nullptr)
                delete[] region[i];
        }
        delete[] region;
    }
    LOG_F(INFO, "WorldFiles destroyed");
}

// ===== Вспомогательные функции =====

void WorldFiles::coordsToRegion(int& regX, int& regY, int& regZ,
                                int chunkX, int chunkY, int chunkZ) {
    regX = chunkX >> REGION_SIZE_BIT;
    regY = chunkY >> REGION_SIZE_BIT;
    regZ = chunkZ >> REGION_SIZE_BIT;
}

void WorldFiles::coordsToLocal(int& locX, int& locY, int& locZ,
                               int chunkX, int chunkY, int chunkZ,
                               int regX, int regY, int regZ) {
    locX = chunkX - (regX << REGION_SIZE_BIT);
    locY = chunkY - (regY << REGION_SIZE_BIT);
    locZ = chunkZ - (regZ << REGION_SIZE_BIT);
}

int WorldFiles::getChunkIndex(int locX, int locY, int locZ) {
    assert(locX >= 0 && locX < REGION_SIZE);
    assert(locY >= 0 && locY < REGION_SIZE);
    assert(locZ >= 0 && locZ < REGION_SIZE);

    return locY * REGION_SIZE * REGION_SIZE + locZ * REGION_SIZE + locX;
}

std::string WorldFiles::getRegionFile(int x, int y, int z) {
    return directory + "/" + std::to_string(x) + "_" + std::to_string(y)
         + "_" + std::to_string(z) + ".bin";
}

// ===== Основные методы =====

void WorldFiles::put(const char* chunkData, int x, int y, int z) {
    assert(chunkData != nullptr);

    int regX, regY, regZ;
    int locX, locY, locZ;

    coordsToRegion(regX, regY, regZ, x, y, z);
    coordsToLocal(locX, locY, locZ, x, y, z, regX, regY, regZ);

    RegionCoords regionKey(regX, regY, regZ);

    // Получить или создать регион
    char** region = regions[regionKey];
    if (region == nullptr) {
        region = new char*[REGION_VOL];
        for (unsigned int i = 0; i < REGION_VOL; i++)
            region[i] = nullptr;
        regions[regionKey] = region;
    }

    // Получить или создать чанк
    int chunkIndex = getChunkIndex(locX, locY, locZ);
    char* targetChunk = region[chunkIndex];
    if (targetChunk == nullptr) {
        targetChunk = new char[CHUNK_DATA_SIZE];
        region[chunkIndex] = targetChunk;
    }

    // Скопировать данные (CHUNK_DATA_SIZE байт = CHUNK_BLOCKS * 4)
    memcpy(targetChunk, chunkData, CHUNK_DATA_SIZE);
}

bool WorldFiles::getChunk(int x, int y, int z, char* out) {
    assert(out != nullptr);

    int regX, regY, regZ;
    int locX, locY, locZ;

    coordsToRegion(regX, regY, regZ, x, y, z);
    coordsToLocal(locX, locY, locZ, x, y, z, regX, regY, regZ);

    RegionCoords regionKey(regX, regY, regZ);
    int chunkIndex = getChunkIndex(locX, locY, locZ);

    // Проверить если регион загружен в памяти
    auto it = regions.find(regionKey);
    if (it != regions.end() && it->second != nullptr) {
        char* chunk = it->second[chunkIndex];
        if (chunk != nullptr) {
            memcpy(out, chunk, CHUNK_DATA_SIZE);
            LOG_F(1, "Chunk (%d,%d,%d) loaded from memory (region %d,%d,%d)",
                  x, y, z, regX, regY, regZ);
            return true;
        }
    }

    // Попытаться прочитать с диска
    bool loaded = readChunk(x, y, z, out);
    if (loaded) {
        LOG_F(1, "Chunk (%d,%d,%d) loaded from disk (region %d,%d,%d)",
              x, y, z, regX, regY, regZ);
    }
    return loaded;
}

bool WorldFiles::readChunk(int x, int y, int z, char* out) {
    int regX, regY, regZ, locX, locY, locZ;
    coordsToRegion(regX, regY, regZ, x, y, z);
    coordsToLocal(locX, locY, locZ, x, y, z, regX, regY, regZ);
    RegionCoords regionKey(regX, regY, regZ);
    int chunkIndex = getChunkIndex(locX, locY, locZ);

    // Получаем открытый файл региона (или открываем новый)
    std::ifstream* stream = nullptr;
    {
        std::lock_guard<std::mutex> lock(m_fileMutex);
        auto it = m_openFiles.find(regionKey);
        if (it == m_openFiles.end()) {
            std::string filename = getRegionFile(regX, regY, regZ);
            auto newStream = std::make_unique<std::ifstream>(filename, std::ios::binary);
            if (!newStream->is_open()) {
                return false;
            }
            auto [iter, _] = m_openFiles.emplace(regionKey, std::move(*newStream));
            stream = &iter->second;
        } else {
            stream = &it->second;
        }
    }

    std::ifstream& input = *stream;

    // Читаем offset из таблицы смещений
    uint32_t offset;
    input.seekg(chunkIndex * 4);
    input.read(reinterpret_cast<char*>(&offset), 4);
    if (input.fail()) return false;

    offset = bytes2Int(reinterpret_cast<const unsigned char*>(&offset), 0);
    if (offset == 0) return false;   // чанк отсутствует на диске

    // Переходим к смещению и читаем сжатые данные
    input.seekg(offset);
    uint32_t compressedSize;
    input.read(reinterpret_cast<char*>(&compressedSize), 4);
    if (input.fail()) return false;

    compressedSize = bytes2Int(reinterpret_cast<const unsigned char*>(&compressedSize), 0);
    if (compressedSize == 0) return false;

    input.read(mainBuffer, compressedSize);
    if (input.fail()) return false;

    // Распаковываем
    decompressRLE(mainBuffer, compressedSize, out, CHUNK_DATA_SIZE);

    // Сохраняем чанк в памяти, чтобы следующий запрос не обращался к диску
    char** region = regions[regionKey];
    if (region == nullptr) {
        region = new char*[REGION_VOL];
        for (unsigned int i = 0; i < REGION_VOL; i++) region[i] = nullptr;
        regions[regionKey] = region;
    }
    char* chunkCopy = new char[CHUNK_DATA_SIZE];
    memcpy(chunkCopy, out, CHUNK_DATA_SIZE);
    region[chunkIndex] = chunkCopy;

    return true;
}

void WorldFiles::write() {
    // Закрываем все открытые на чтение файлы, чтобы их можно было перезаписать
    {
        std::lock_guard<std::mutex> lock(m_fileMutex);
        m_openFiles.clear();
    }
    try {
        std::filesystem::create_directories(directory);
    } catch (const std::exception& e) {
        LOG_F(ERROR, "Failed to create directory '%s': %s", directory.c_str(), e.what());
        return;
    }

    int successCount = 0;
    int failCount = 0;

    for (auto& pair : regions) {
        if (pair.second == nullptr)
            continue;

        RegionCoords coords = pair.first;
        unsigned int size = writeRegion(mainBuffer, coords.x, coords.y, coords.z, pair.second);

        std::string filename = getRegionFile(coords.x, coords.y, coords.z);
        std::ofstream output(filename, std::ios::binary);

        if (!output.is_open()) {
            LOG_F(WARNING, "Failed to open file for writing: %s", filename.c_str());
            failCount++;
            continue;
        }

        output.write(mainBuffer, size);

        if (output.fail()) {
            LOG_F(WARNING, "Error writing file: %s", filename.c_str());
            failCount++;
        } else {
            successCount++;
        }

        output.close();
    }

    LOG_F(INFO, "Regions saved: %d success, %d failed", successCount, failCount);
}

unsigned int WorldFiles::writeRegion(char* out, int x, int y, int z, char** region) {
    unsigned int offset = REGION_VOL * 4;

    // Инициализировать таблицу смещений нулями
    for (unsigned int i = 0; i < offset; i++)
        out[i] = 0;

    char* compressed = new char[CHUNK_DATA_SIZE * 2];

    for (int i = 0; i < REGION_VOL; i++) {
        char* chunk = region[i];

        // Если чанк не загружен в памяти, попытаться прочитать с диска
        if (chunk == nullptr) {
            int locX = i % REGION_SIZE;
            int locY = i / (REGION_SIZE * REGION_SIZE);
            int locZ = (i / REGION_SIZE) % REGION_SIZE;

            int chunkX = locX + (x << REGION_SIZE_BIT);
            int chunkY = locY + (y << REGION_SIZE_BIT);
            int chunkZ = locZ + (z << REGION_SIZE_BIT);

            chunk = new char[CHUNK_DATA_SIZE];
            if (readChunk(chunkX, chunkY, chunkZ, chunk)) {
                region[i] = chunk;
            } else {
                delete[] chunk;
                chunk = nullptr;
            }
        }

        // Записать чанк или выставить нулевое смещение
        if (chunk == nullptr) {
            int2Bytes(0, out, i * 4);
        } else {
            int2Bytes(offset, out, i * 4);

            unsigned int compressedSize = compressRLE(chunk, CHUNK_DATA_SIZE, compressed);
            int2Bytes(compressedSize, out, offset);
            offset += 4;

            for (unsigned int j = 0; j < compressedSize; j++)
                out[offset++] = compressed[j];
        }
    }

    delete[] compressed;
    return offset;
}
