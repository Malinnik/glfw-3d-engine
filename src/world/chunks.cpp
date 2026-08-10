#include "chunks.h"
#include "world/chunk.h"
#include "world/generation.h"
#include "blocks/block.h"

Chunks::Chunks(int w, int h, int d, int ox, int oy, int oz) : w(w), h(h), d(d), ox(ox), oy(oy), oz(oz)
{
    volume = w*h*d;
	chunks = new Chunk*[volume];
	chunksSecond = new Chunk*[volume];

	meshes = new Mesh*[volume];
	meshesSecond = new Mesh*[volume];

	for (size_t i = 0; i < volume; i++){
		chunks[i] = nullptr;
		chunksSecond[i] = nullptr;
		meshes[i] = nullptr;
		meshesSecond[i] = nullptr;
	}
}

Chunks::~Chunks()
{
    for (size_t i = 0; i < volume; i++)
	{
        delete chunks[i];  
		delete meshes[i];
		delete meshesSecond[i];

	}
    delete[] chunks;
	delete[] meshes;
	delete[] meshesSecond;
}

blocks::Block* Chunks::get(int x, int y, int z)
{
    x -= ox * CHUNK_W;
	y -= oy * CHUNK_H;
	z -= oz * CHUNK_D;
	int cx = x / CHUNK_W;
	int cy = y / CHUNK_H;
	int cz = z / CHUNK_D;
	if (x < 0) cx--;
	if (y < 0) cy--;
	if (z < 0) cz--;
	if (cx < 0 || cy < 0 || cz < 0 || cx >= w || cy >= h || cz >= d)
		return nullptr;
	Chunk* chunk = chunks[(cy * d + cz) * w + cx];
	if (chunk == nullptr)
		return nullptr;
	int lx = x - cx * CHUNK_W;
	int ly = y - cy * CHUNK_H;
	int lz = z - cz * CHUNK_D;
    return blocks::get(chunk->blocksIds[(ly * CHUNK_D + lz) * CHUNK_W + lx]);
}

Chunk* Chunks::getChunk(int x, int y, int z){
	x -= ox;
	y -= oy;
	z -= oz;
	if (x < 0 || y < 0 || z < 0 || x >= w || y >= h || z >= d)
		return nullptr;
	return chunks[(y * d + z) * w + x];
}

int Chunks::getChunkIndex(int x, int y, int z)
{
	int cx = x - ox;
	int cy = y - oy;
	int cz = z - oz;
	if (cx < 0 || cy < 0 || cz < 0 || cx >= w || cy >= h || cz >= d) return -1;
    return (cy * d + cz) * w + cx;
}

Chunk* Chunks::getChunkByBlock(int x, int y, int z)
{
    x -= ox * CHUNK_W;
	y -= oy * CHUNK_H;
	z -= oz * CHUNK_D;
	int cx = x / CHUNK_W;
	int cy = y / CHUNK_H;
	int cz = z / CHUNK_D;
	if (x < 0) cx--;
	if (y < 0) cy--;
	if (z < 0) cz--;
	if (cx < 0 || cy < 0 || cz < 0 || cx >= w || cy >= h || cz >= d)
		return nullptr;
	return chunks[(cy * d + cz) * w + cx];
}

void Chunks::set(int x, int y, int z, int id){
	x -= ox * CHUNK_W;
	y -= oy * CHUNK_H;
	z -= oz * CHUNK_D;
	int cx = x / CHUNK_W;
	int cy = y / CHUNK_H;
	int cz = z / CHUNK_D;
	if (x < 0) cx--;
	if (y < 0) cy--;
	if (z < 0) cz--;
	if (cx < 0 || cy < 0 || cz < 0 || cx >= w || cy >= h || cz >= d)
		return;
	Chunk* chunk = chunks[(cy * d + cz) * w + cx];
	if (chunk == nullptr)
		return;
	int lx = x - cx * CHUNK_W;
	int ly = y - cy * CHUNK_H;
	int lz = z - cz * CHUNK_D;
	chunk->blocksIds[(ly * CHUNK_D + lz) * CHUNK_W + lx] = id;
	chunk->modified = true;

	if (lx == 0 && (chunk = getChunk(cx+ox-1, cy+oy, cz+oz))) chunk->modified = true;
	if (ly == 0 && (chunk = getChunk(cx+ox, cy+oy-1, cz+oz))) chunk->modified = true;
	if (lz == 0 && (chunk = getChunk(cx+ox, cy+oy, cz+oz-1))) chunk->modified = true;

	if (lx == CHUNK_W-1 && (chunk = getChunk(cx+ox+1, cy+oy, cz+oz))) chunk->modified = true;
	if (ly == CHUNK_H-1 && (chunk = getChunk(cx+ox, cy+oy+1, cz+oz))) chunk->modified = true;
	if (lz == CHUNK_D-1 && (chunk = getChunk(cx+ox, cy+oy, cz+oz+1))) chunk->modified = true;
}

blocks::Block* Chunks::rayCast(vec3 a, vec3 dir, float maxDist, vec3& end, vec3& norm, vec3& iend) {
	float px = a.x;
	float py = a.y;
	float pz = a.z;

	float dx = dir.x;
	float dy = dir.y;
	float dz = dir.z;

	float t = 0.0f;
	int ix = floor(px);
	int iy = floor(py);
	int iz = floor(pz);

	float stepx = (dx > 0.0f) ? 1.0f : -1.0f;
	float stepy = (dy > 0.0f) ? 1.0f : -1.0f;
	float stepz = (dz > 0.0f) ? 1.0f : -1.0f;

	float infinity = std::numeric_limits<float>::infinity();

	float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
	float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
	float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);

	float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
	float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
	float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);

	float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
	float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
	float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;

	int steppedIndex = -1;

	while (t <= maxDist){
		blocks::Block* block = get(ix, iy, iz);
		if (block == nullptr || block->id){
			end.x = px + t * dx;
			end.y = py + t * dy;
			end.z = pz + t * dz;

			iend.x = ix;
			iend.y = iy;
			iend.z = iz;

			norm.x = norm.y = norm.z = 0.0f;
			if (steppedIndex == 0) norm.x = -stepx;
			if (steppedIndex == 1) norm.y = -stepy;
			if (steppedIndex == 2) norm.z = -stepz;
			return block;
		}
		if (txMax < tyMax) {
			if (txMax < tzMax) {
				ix += stepx;
				t = txMax;
				txMax += txDelta;
				steppedIndex = 0;
			} else {
				iz += stepz;
				t = tzMax;
				tzMax += tzDelta;
				steppedIndex = 2;
			}
		} else {
			if (tyMax < tzMax) {
				iy += stepy;
				t = tyMax;
				tyMax += tyDelta;
				steppedIndex = 1;
			} else {
				iz += stepz;
				t = tzMax;
				tzMax += tzDelta;
				steppedIndex = 2;
			}
		}
	}
	iend.x = ix;
	iend.y = iy;
	iend.z = iz;

	end.x = px + t * dx;
	end.y = py + t * dy;
	end.z = pz + t * dz;
	norm.x = norm.y = norm.z = 0.0f;
	return nullptr;
}

void Chunks::setCenter(int x, int y, int z)
{
	int cx = x / CHUNK_W;
	int cy = y / CHUNK_H;
	int cz = z / CHUNK_D;
	cx -= ox;
	cy -= oy;
	cz -= oz;
	if (x < 0) cx--;
	if (y < 0) cy--;
	if (z < 0) cz--;
	// Use same center calculation as loadVisible/buildMeshes
	cx -= (int)((w - 1) * 0.5f);
	cy -= (int)((h - 1) * 0.5f);
	cz -= (int)((d - 1) * 0.5f);
	if (cx != 0 || cy != 0 || cz != 0)
		translate(cx,cy,cz);
}

void Chunks::translate(int dx, int dy, int dz)
{
	for (unsigned int i = 0; i < volume; i++)
	{
		chunksSecond[i] = nullptr;
		meshesSecond[i] = nullptr;
	}

	for (unsigned int y = 0; y < h; y++){
		for (unsigned int z = 0; z < d; z++){
			for (unsigned int x = 0; x < w; x++){
				Chunk* chunk = chunks[(y * d + z) * w + x];
				int nx = x - dx;
				int ny = y - dy;
				int nz = z - dz;
				if (chunk == nullptr)
					continue;
				Mesh* mesh = meshes[(y * d + z) * w + x];
				if (nx < 0 || ny < 0 || nz < 0 || nx >= w || ny >= h || nz >= d){
					if (worldFiles) {
						worldFiles->put((const char*)chunk->blocksIds, chunk->x, chunk->y, chunk->z);
					}
					delete chunk;
					delete mesh;
					continue;
				}
				meshesSecond[(ny * d + nz) * w + nx] = mesh;
				chunksSecond[(ny * d + nz) * w + nx] = chunk;
			}
		}
	}
	Chunk** ctemp = chunks;
	chunks = chunksSecond;
	chunksSecond = ctemp;

	Mesh** mtemp = meshes;
	meshes = meshesSecond;
	meshesSecond = mtemp;

	ox += dx;
	oy += dy;
	oz += dz;
}

bool Chunks::loadVisible(WorldFiles *worldFiles)
{
    // 1. Собираем все пустые слоты с их квадратом расстояния до центра
    std::vector<std::pair<int, int>> emptySlots; // pair(расстояние^2, индекс)
    float centerX = (w - 1) * 0.5f;
    float centerY = (h - 1) * 0.5f;
    float centerZ = (d - 1) * 0.5f;

    for (unsigned int y = 0; y < h; y++) {
        for (unsigned int z = 0; z < d; z++) {
            for (unsigned int x = 0; x < w; x++) {
                int index = (y * d + z) * w + x;
                if (chunks[index] != nullptr) continue;

                float lx = x - centerX;
                float ly = y - centerY;
                float lz = z - centerZ;
                int distanceSq = (int)(lx * lx + ly * ly + lz * lz);
                emptySlots.emplace_back(distanceSq, index);
            }
        }
    }

    if (emptySlots.empty())
        return false;

    // 2. Сортируем по возрастанию расстояния (ближайшие первыми)
    std::sort(emptySlots.begin(), emptySlots.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });

    // 3. Загружаем до MAX_LOADS_PER_FRAME чанков
    const int MAX_LOADS_PER_FRAME = 3;
    int loaded = 0;
    for (const auto& [dist, idx] : emptySlots) {
        if (loaded >= MAX_LOADS_PER_FRAME)
            break;

        if (chunks[idx] != nullptr)
            continue; // на всякий случай повторная проверка

        // Локальные координаты слота
        int nx = idx % w;
        int nz = (idx / w) % d;
        int ny = idx / (w * d);

        // Мировые координаты чанка
        int chunkX = nx + ox;
        int chunkY = ny + oy;
        int chunkZ = nz + oz;

        // Создаём пустой чанк и сразу занимаем слот
        Chunk* chunk = new Chunk(chunkX, chunkY, chunkZ);
        chunks[idx] = chunk;

        // Отправляем запрос на генерацию/загрузку
        if (generator) {
            generator->requestChunk(chunkX, chunkY, chunkZ);
        }
        loaded++;
    }

    return loaded > 0;
}

bool Chunks::_buildMeshes(BlockRenderer* renderer) {
	bool builtAny = false;

	for (unsigned int y = 0; y < h; y++){
		for (unsigned int z = 0; z < d; z++){
			for (unsigned int x = 0; x < w; x++){
				int index = (y * d + z) * w + x;
				Chunk* chunk = chunks[index];
				if (chunk == nullptr)
					continue;

				Mesh* mesh = meshes[index];
				if (mesh != nullptr && !chunk->modified)
					continue;

				Chunk* closes[27];

				if (mesh == nullptr || chunk->modified){
					if (mesh != nullptr)
						delete mesh;
					if (chunk->isEmpty()){
						meshes[index] = nullptr;
						chunk->modified = false;
						continue;
					}
					chunk->modified = false;
					for (int i = 0; i < 27; i++)
						closes[i] = nullptr;
					for (size_t j = 0; j < volume; j++){
						Chunk* other = chunks[j];
						if (other == nullptr)
							continue;

						int ox = other->x - chunk->x;
						int oy = other->y - chunk->y;
						int oz = other->z - chunk->z;

						if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
							continue;

						ox += 1;
						oy += 1;
						oz += 1;
						closes[(oy * 3 + oz) * 3 + ox] = other;
					}
					mesh = renderer->render(chunk, (const Chunk**)closes);
					meshes[index] = mesh;
					builtAny = true;
				}
			}
		}
	}
	return builtAny;
}

void Chunks::update()
{
	if (!generator) return;

    std::vector<ChunkResult> results;
    generator->fetchReadyChunks(results);  // забираем всё накопившееся

    for (auto& result : results) {
        int x = result.task.x;
        int y = result.task.y;
        int z = result.task.z;

        // Ищем индекс чанка по мировым координатам
        int idx = getChunkIndex(x, y, z);   // метод, который переводит мировые координаты чанка в индекс массива
        if (idx == -1 || chunks[idx] == nullptr)
            continue;   // чанк мог быть уже выгружен (например, при перемещении мира)

        // Копируем сгенерированные блоки в наш чанк-заглушку
        memcpy(chunks[idx]->blocksIds, result.blocks, sizeof(result.blocks));
        chunks[idx]->modified = true;

        // Теперь помечаем соседние чанки (если они существуют), чтобы обновились стыки
        if (Chunk* adj = getChunk(x-1, y, z)) adj->modified = true;
        if (Chunk* adj = getChunk(x+1, y, z)) adj->modified = true;
        if (Chunk* adj = getChunk(x, y-1, z)) adj->modified = true;
        if (Chunk* adj = getChunk(x, y+1, z)) adj->modified = true;
        if (Chunk* adj = getChunk(x, y, z-1)) adj->modified = true;
        if (Chunk* adj = getChunk(x, y, z+1)) adj->modified = true;
    }
}
