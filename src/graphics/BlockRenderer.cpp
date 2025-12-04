#include "BlockRenderer.h"
#include "blocks/block.h"

// vert coords and texture coords
#define VERTEX_SIZE (3+2+1)

#define IS_IN_CHUNK(x,y,z) ((x) >= 0 && (x) < CHUNK_W && (y) >= 0 && (y) < CHUNK_H && (z) >= 0 && (z) < CHUNK_D)
#define BLOCK(x,y,z) (chunk->blocks[((y) * CHUNK_D + (z)) * CHUNK_W + (x)])
#define IS_BLOCKED(x,y,z) ((IS_IN_CHUNK(x, y, z)) && BLOCK(x, y, z).id)

#define VERTEX(index, x,y,z, u,v, l) buffer[index+0] = (x);\
                                     buffer[index+1] = (y);\
                                     buffer[index+2] = (z);\
                                     buffer[index+3] = (u);\
                                     buffer[index+4] = (v);\
                                     buffer[index+5] = (l);\
                                     index += VERTEX_SIZE

int chunk_attrs[] = {3,2,1,0};

BlockRenderer::BlockRenderer(size_t capacity) : capacity(capacity)
{
    buffer = new float[capacity * VERTEX_SIZE * 6];

}

BlockRenderer::~BlockRenderer()
{
    delete[] buffer;
}

Mesh *BlockRenderer::render(Chunk *chunk)
{
    size_t index = 0;
	for (int y = 0; y < CHUNK_H; y++){
		for (int z = 0; z < CHUNK_D; z++){
			for (int x = 0; x < CHUNK_W; x++){
				block blk = chunk->blocks[(y * CHUNK_D + z) * CHUNK_W + x];
				unsigned int id = blk.id;

				if (!id){
					continue;
				}
				float l;
				float uvsize = 1.0f/16.0f;
				float u = ((id-1) % 16) * uvsize;
				float v = 1-((1 + (id-1) / 16) * uvsize);

				if (!IS_BLOCKED(x,y+1,z)){
					l = 1.0f;
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u,v, l);
				}
				if (!IS_BLOCKED(x,y-1,z)){
					l = 0.75f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
				}

				if (!IS_BLOCKED(x+1,y,z)){
					l = 0.95f;
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v+uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u,v, l);
				}
				if (!IS_BLOCKED(x-1,y,z)){
					l = 0.85f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u,v+uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
				}

				if (!IS_BLOCKED(x,y,z+1)){
					l = 0.9f;
					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u,v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u,v, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
				}
				if (!IS_BLOCKED(x,y,z-1)){
					l = 0.8f;
					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v+uvsize, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u,v+uvsize, l);

					VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u,v+uvsize, l);
					VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u,v, l);
				}
			}
		}
	}

    return new Mesh(buffer, index / VERTEX_SIZE, chunk_attrs);
}
