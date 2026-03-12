#include <math.h>
#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"
#include "chunk.h"
#include "blocks/block.h"
#include "world/generation.h"

#include "loguru.hpp"
#include "fmt/format.h"

Chunk::Chunk(int xpos, int ypos, int zpos) : x(xpos), y(ypos), z(zpos)
{
	for (unsigned int i = 0; i < CHUNK_BLOCKS; i++)
		blocksIds[i] = 1;
}

Chunk::~Chunk()
{
    delete[] blocksIds; 
}

bool Chunk::isEmpty()
{
    int id = -1;
	for (int i = 0; i < CHUNK_BLOCKS; i++){
		if (blocksIds[i] != id){
			if (id != -1)
				return false;
			else
				id = blocksIds[i];
		}
	}
	return true;
}
