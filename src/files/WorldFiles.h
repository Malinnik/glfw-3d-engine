#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>
#include <filesystem>

/**
 * Структура для координат региона (3D)
 * Регионы группируют чанки для оптимизации сохранения и загрузки
 */
struct RegionCoords {
    int x, y, z;

    RegionCoords() = default;
    RegionCoords(int x, int y, int z) : x(x), y(y), z(z) {}

    bool operator==(const RegionCoords& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

/**
 * Хеш-функция для использования RegionCoords в unordered_map
 */
namespace std {
    template <>
    struct hash<RegionCoords> {
        size_t operator()(const RegionCoords& c) const {
            return hash<int>()(c.x) ^ (hash<int>()(c.y) << 1) ^ (hash<int>()(c.z) << 2);
        }
    };
}

/**
 * Система сохранения и загрузки мира
 * Поддерживает бесконечный мир в трех измерениях
 * Использует систему регионов (32×32×32 чанки в каждом регионе)
 */
class WorldFiles {
public:
    WorldFiles(std::string directory, size_t mainBufferCapacity);
    ~WorldFiles();

    /**
     * Сохранить чанк в памяти
     * @param chunkData данные чанка размером CHUNK_BLOCKS
     * @param x, y, z координаты чанка в мире
     */
    void put(const char* chunkData, int x, int y, int z);

    /**
     * Загрузить чанк из памяти или диска
     * @param x, y, z координаты чанка
     * @param out буфер размером CHUNK_BLOCKS для вывода данных
     * @return true если чанк успешно загружен
     */
    bool getChunk(int x, int y, int z, char* out);

    /**
     * Записать все измененные регионы на диск
     */
    void write();

private:
    std::string directory;
    char* mainBuffer;  // Буфер для компрессии/декомпрессии

    // Размер региона в чанках (2^5 = 32)
    static constexpr int REGION_SIZE_BIT = 5;
    static constexpr int REGION_SIZE = 1 << REGION_SIZE_BIT;  // 32
    static constexpr int REGION_VOL = REGION_SIZE * REGION_SIZE * REGION_SIZE;  // 32768

    // Карта: RegionCoords -> массив чанков в этом регионе
    std::unordered_map<RegionCoords, char**> regions;

    /**
     * Получить путь к файлу региона
     */
    std::string getRegionFile(int x, int y, int z);

    /**
     * Прочитать чанк с диска
     */
    bool readChunk(int x, int y, int z, char* out);

    /**
     * Записать регион на диск
     */
    unsigned int writeRegion(char* out, int x, int y, int z, char** region);

    /**
     * Преобразовать ключ в 3D координаты региона
     */
    void coordsToRegion(int& regX, int& regY, int& regZ, int chunkX, int chunkY, int chunkZ);

    /**
     * Преобразовать глобальные координаты в локальные координаты внутри региона
     */
    void coordsToLocal(int& locX, int& locY, int& locZ, int chunkX, int chunkY, int chunkZ,
                       int regX, int regY, int regZ);

    /**
     * Получить индекс чанка в массиве региона (3D -> 1D)
     */
    int getChunkIndex(int locX, int locY, int locZ);
};
