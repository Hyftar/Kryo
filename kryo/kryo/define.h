#ifndef KRYO_DEFINE_H
#define KRYO_DEFINE_H

#include "global.h"

typedef uint8_t BlockType;
enum BLOCK_TYPE { BTYPE_AIR, BTYPE_DIRT, BTYPE_GRASS, BTYPE_STONE };

#define CHUNK_SIZE_WIDTH 16
#define CHUNK_SIZE_HEIGHT 128
#define CHUNK_SIZE_DEPTH 16
#define CHUNK_SIZE (CHUNK_SIZE_WIDTH * CHUNK_SIZE_DEPTH * CHUNK_SIZE_HEIGHT)
#define WORLD_SEED 95
#define CAMERA_SENSITIVITY 0.01f


#define GRAVACC -12
#define PLAYER_HEIGHT 1.7f
#define PLAYER_SPEED 12.5f
#define PLAYER_JUMPHEIGHT 6.f

#define MAX_SELECTION_DISTANCE 4

#define BLOCK_HEIGHT 1.f
#define BLOCK_MARGIN .25f

#define BLOCK_TYPE_MAX sizeof(BlockType) << 8
#define TEXTUREATLAS_SIZE 16

#define TEXTURE_PATH "../kryo/media/textures/"
#define SHADER_PATH "../kryo/media/shaders/"

#define CHUNK_VIEW_DISTANCE 16
#define CHUNK_BUFFER_SIZE (CHUNK_VIEW_DISTANCE * 2)

#endif
