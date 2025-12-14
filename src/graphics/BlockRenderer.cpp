#include "BlockRenderer.h"
#include "engine/resources/ResourceLocation.h"

// vert coords and texture coords
#define vertex_SIZE (3+2+1)

int chunk_attrs[] = {3,2,1,0};

BlockRenderer::BlockRenderer(size_t capacity) : capacity(capacity)
{
    buffer = new float[capacity * vertex_SIZE * 6];

}

BlockRenderer::~BlockRenderer()
{
    delete[] buffer;
}

std::tuple<int, int, int, int> BlockRenderer::calculateUV(resources::ResourceLocation* location)
{
	unsigned int id = location->getID();
	float uvsize = 1.0f/16.0f;
	float u1 = (id % 16) * uvsize;
	float v1 = 1-((1 + id / 16) * uvsize);
	float u2 = u1 + uvsize;
	float v2 = v1 + uvsize;

	return {u1, v1, u2, v2};
}


Mesh *BlockRenderer::render(Chunk *chunk, const Chunk** chunks, bool ambientOcclusion)
{
	float aoFactor = 0.15f;
    size_t index = 0;
	for (int y = 0; y < CHUNK_H; y++){
		for (int z = 0; z < CHUNK_D; z++){
			for (int x = 0; x < CHUNK_W; x++){
				unsigned int id = chunk->blocksIds[(y * CHUNK_D + z) * CHUNK_W + x];
				blocks::Block* block = blocks::get(id);
				// unsigned int id = blk.id;
				if (id == 0){
					continue;
				}
				float l;
				float a,b,c,d,e,f,g,h;
				a = b = c = d = e = f = g = h = 0.0f;
				
                if (!isBlocked(x,y+1,z)){
					l = 1.0f;
					auto [u1, v1, u2, v2] = calculateUV(block->textures.up);

					if (ambientOcclusion){
						a = isBlocked(x+1,y+1,z)*aoFactor;
						b = isBlocked(x,y+1,z+1)*aoFactor;
						c = isBlocked(x-1,y+1,z)*aoFactor;
						d = isBlocked(x,y+1,z-1)*aoFactor;

						e = isBlocked(x-1,y+1,z-1)*aoFactor;
						f = isBlocked(x-1,y+1,z+1)*aoFactor;
						g = isBlocked(x+1,y+1,z+1)*aoFactor;
						h = isBlocked(x+1,y+1,z-1)*aoFactor;
					}

					vertex(&index, x - 0.5f, y + 0.5f, z - 0.5f, u2,v1, l*(1.0f-c-d-e));
					vertex(&index, x - 0.5f, y + 0.5f, z + 0.5f, u2,v2, l*(1.0f-c-b-f));
					vertex(&index, x + 0.5f, y + 0.5f, z + 0.5f, u1,v2, l*(1.0f-a-b-g));

					vertex(&index, x - 0.5f, y + 0.5f, z - 0.5f, u2,v1, l*(1.0f-c-d-e));
					vertex(&index, x + 0.5f, y + 0.5f, z + 0.5f, u1,v2, l*(1.0f-a-b-g));
					vertex(&index, x + 0.5f, y + 0.5f, z - 0.5f, u1,v1, l*(1.0f-a-d-h));
				}
				if (!isBlocked(x,y-1,z)){
					l = 0.75f;
					auto [u1, v1, u2, v2] = calculateUV(block->textures.down);

					if (ambientOcclusion){
						a = isBlocked(x+1,y-1,z)*aoFactor;
						b = isBlocked(x,y-1,z+1)*aoFactor;
						c = isBlocked(x-1,y-1,z)*aoFactor;
						d = isBlocked(x,y-1,z-1)*aoFactor;

						e = isBlocked(x-1,y-1,z-1)*aoFactor;
						f = isBlocked(x-1,y-1,z+1)*aoFactor;
						g = isBlocked(x+1,y-1,z+1)*aoFactor;
						h = isBlocked(x+1,y-1,z-1)*aoFactor;
					}

					vertex(&index, x - 0.5f, y - 0.5f, z - 0.5f, u1,v1, l*(1.0f-c-d-e));
					vertex(&index, x + 0.5f, y - 0.5f, z + 0.5f, u2,v2, l*(1.0f-a-b-g));
					vertex(&index, x - 0.5f, y - 0.5f, z + 0.5f, u1,v2, l*(1.0f-c-b-f));

					vertex(&index, x - 0.5f, y - 0.5f, z - 0.5f, u1,v1, l*(1.0f-c-d-e));
					vertex(&index, x + 0.5f, y - 0.5f, z - 0.5f, u2,v1, l*(1.0f-a-d-h));
					vertex(&index, x + 0.5f, y - 0.5f, z + 0.5f, u2,v2, l*(1.0f-a-b-g));
				}

				if (!isBlocked(x+1,y,z)){
					l = 0.95f;
					auto [u1, v1, u2, v2] = calculateUV(block->textures.right);

					if (ambientOcclusion){
						a = isBlocked(x+1,y+1,z)*aoFactor;
						b = isBlocked(x+1,y,  z+1)*aoFactor;
						c = isBlocked(x+1,y-1,z)*aoFactor;
						d = isBlocked(x+1,y,  z-1)*aoFactor;

						e = isBlocked(x+1,y-1,z-1)*aoFactor;
						f = isBlocked(x+1,y-1,z+1)*aoFactor;
						g = isBlocked(x+1,y+1,z+1)*aoFactor;
						h = isBlocked(x+1,y+1,z-1)*aoFactor;
					}

					vertex(&index, x + 0.5f, y - 0.5f, z - 0.5f, u2,v1, l*(1.0f-c-d-e));
					vertex(&index, x + 0.5f, y + 0.5f, z - 0.5f, u2,v2, l*(1.0f-d-a-h));
					vertex(&index, x + 0.5f, y + 0.5f, z + 0.5f, u1,v2, l*(1.0f-a-b-g));

					vertex(&index, x + 0.5f, y - 0.5f, z - 0.5f, u2,v1, l*(1.0f-c-d-e));
					vertex(&index, x + 0.5f, y + 0.5f, z + 0.5f, u1,v2, l*(1.0f-a-b-g));
					vertex(&index, x + 0.5f, y - 0.5f, z + 0.5f, u1,v1, l*(1.0f-b-c-f));
				}
				if (!isBlocked(x-1,y,z)){
					l = 0.85f;
					auto [u1, v1, u2, v2] = calculateUV(block->textures.left);

					if (ambientOcclusion){
						a = isBlocked(x-1,y+1,z)*aoFactor;
						b = isBlocked(x-1,y,  z+1)*aoFactor;
						c = isBlocked(x-1,y-1,z)*aoFactor;
						d = isBlocked(x-1,y,  z-1)*aoFactor;

						e = isBlocked(x-1,y-1,z-1)*aoFactor;
						f = isBlocked(x-1,y-1,z+1)*aoFactor;
						g = isBlocked(x-1,y+1,z+1)*aoFactor;
						h = isBlocked(x-1,y+1,z-1)*aoFactor;
					}

					vertex(&index, x - 0.5f, y - 0.5f, z - 0.5f, u1,v1, l*(1.0f-c-d-e));
					vertex(&index, x - 0.5f, y + 0.5f, z + 0.5f, u2,v2, l*(1.0f-a-b-g));
					vertex(&index, x - 0.5f, y + 0.5f, z - 0.5f, u1,v2, l*(1.0f-d-a-h));

					vertex(&index, x - 0.5f, y - 0.5f, z - 0.5f, u1,v1, l*(1.0f-c-d-e));
					vertex(&index, x - 0.5f, y - 0.5f, z + 0.5f, u2,v1, l*(1.0f-b-c-f));
					vertex(&index, x - 0.5f, y + 0.5f, z + 0.5f, u2,v2, l*(1.0f-a-b-g));
				}

				if (!isBlocked(x,y,z+1)){
					l = 0.9f;
					auto [u1, v1, u2, v2] = calculateUV(block->textures.front);

					if (ambientOcclusion){
						a = isBlocked(x,  y+1,z+1)*aoFactor;
						b = isBlocked(x+1,y,  z+1)*aoFactor;
						c = isBlocked(x,  y-1,z+1)*aoFactor;
						d = isBlocked(x-1,y,  z+1)*aoFactor;

						e = isBlocked(x-1,y-1,z+1)*aoFactor;
						f = isBlocked(x+1,y-1,z+1)*aoFactor;
						g = isBlocked(x+1,y+1,z+1)*aoFactor;
						h = isBlocked(x-1,y+1,z+1)*aoFactor;
					}

					vertex(&index, x - 0.5f, y - 0.5f, z + 0.5f, u1,v1, l*(1.0f-c-d-e));
					vertex(&index, x + 0.5f, y + 0.5f, z + 0.5f, u2,v2, l*(1.0f-a-b-g));
					vertex(&index, x - 0.5f, y + 0.5f, z + 0.5f, u1,v2, l*(1.0f-a-d-h));

					vertex(&index, x - 0.5f, y - 0.5f, z + 0.5f, u1,v1, l*(1.0f-c-d-e));
					vertex(&index, x + 0.5f, y - 0.5f, z + 0.5f, u2,v1, l*(1.0f-b-c-f));
					vertex(&index, x + 0.5f, y + 0.5f, z + 0.5f, u2,v2, l*(1.0f-a-b-g));
				}
				if (!isBlocked(x,y,z-1)){
					l = 0.8f;
					auto [u1, v1, u2, v2] = calculateUV(block->textures.back);

					if (ambientOcclusion){
						a = isBlocked(x,  y+1,z-1)*aoFactor;
						b = isBlocked(x+1,y,  z-1)*aoFactor;
						c = isBlocked(x,  y-1,z-1)*aoFactor;
						d = isBlocked(x-1,y,  z-1)*aoFactor;

						e = isBlocked(x-1,y-1,z-1)*aoFactor;
						f = isBlocked(x+1,y-1,z-1)*aoFactor;
						g = isBlocked(x+1,y+1,z-1)*aoFactor;
						h = isBlocked(x-1,y+1,z-1)*aoFactor;
					}

					vertex(&index, x - 0.5f, y - 0.5f, z - 0.5f, u2,v1, l*(1.0f-c-d-e));
					vertex(&index, x - 0.5f, y + 0.5f, z - 0.5f, u2,v2, l*(1.0f-a-d-h));
					vertex(&index, x + 0.5f, y + 0.5f, z - 0.5f, u1,v2, l*(1.0f-a-b-g));

					vertex(&index, x - 0.5f, y - 0.5f, z - 0.5f, u2,v1, l*(1.0f-c-d-e));
					vertex(&index, x + 0.5f, y + 0.5f, z - 0.5f, u1,v2, l*(1.0f-a-b-g));
					vertex(&index, x + 0.5f, y - 0.5f, z - 0.5f, u1,v1, l*(1.0f-b-c-f));
				}
			}
		}
	}

    return new Mesh(buffer, index / vertex_SIZE, chunk_attrs);
}

int BlockRenderer::cdiv(int x, int a)
{
    if (x < 0)
		return (x / a) - 1;
	return x / a;
}

int BlockRenderer::localNeg(int x, int size)
{
    if (x < 0)
		return size + x;
	return x;
}

int BlockRenderer::local(int x, int size)
{
    if (x >= size)
		return x - size;
	return localNeg(x, size);
}

Chunk *BlockRenderer::getChunk(int x, int y, int z)
{
	if (!currentChunks)
		return nullptr;
    int cx = cdiv(x, CHUNK_W);
    int cy = cdiv(y, CHUNK_H);
    int cz = cdiv(z, CHUNK_D);
    
    int index = ((cy + 1) * 3 + (cz + 1)) * 3 + (cx + 1);
    return (Chunk*)currentChunks[index]; 
}

bool BlockRenderer::isChunk(int x, int y, int z)
{
    return getChunk(x, y, z) != nullptr;
}

blocks::Block* BlockRenderer::block(int x, int y, int z)
{
    return blocks::get(getChunk(x, y, z)->blocksIds[local(y, CHUNK_H) * CHUNK_D + local(z, CHUNK_D) * CHUNK_W + local(x, CHUNK_W)]);
}

bool BlockRenderer::isBlocked(int x, int y, int z)
{
    Chunk* chunk = getChunk(x, y, z);
	if (!chunk)
		return true;
	if (block(x, y, z)->id != 0)
		return true;
	return false;
}

void BlockRenderer::vertex(size_t* index, float x, float y, float z, float u, float v, float l)
{
	buffer[*index+0] = x;
	buffer[*index+1] = y;
	buffer[*index+2] = z;
	buffer[*index+3] = u;
	buffer[*index+4] = v;
	buffer[*index+5] = l;
	*index += vertex_SIZE;
}
