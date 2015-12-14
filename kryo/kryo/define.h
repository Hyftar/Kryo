#ifndef KRYO_DEFINE_H
#define KRYO_DEFINE_H

#include "global.h"

typedef uint8_t BlockType;
enum BLOCK_TYPE { BTYPE_AIR, BTYPE_DIRT, BTYPE_GRASS, BTYPE_STONE, BTYPE_NONE };

#define CHUNK_SIZE_WIDTH 16
#define CHUNK_SIZE_HEIGHT 128
#define CHUNK_SIZE_DEPTH 16
#define CHUNK_SIZE (CHUNK_SIZE_WIDTH * CHUNK_SIZE_DEPTH * CHUNK_SIZE_HEIGHT)
#define CAMERA_SENSITIVITY 0.01f


#define GRAVACC -9.80665
#define PLAYER_HEIGHT 1.7f
#define PLAYER_SPEED 2.f

#define MAX_SELECTION_DISTANCE 4

#define BLOCK_HEIGHT 1.f
#define BLOCK_MARGIN .25f

#define BLOCK_TYPE_MAX sizeof(BlockType) << 8
#define TEXTUREATLAS_SIZE 16

#define TEXTURE_PATH "../kryo/media/textures/"
#define SHADER_PATH "../kryo/media/shaders/"

#define CHUNK_VIEW_DISTANCE_X 16
#define CHUNK_VIEW_DISTANCE_Y 16
#define CHUNK_BUFFER_SIZE_WIDTH (CHUNK_VIEW_DISTANCE_X * 2)
#define CHUNK_BUFFER_SIZE_HEIGHT (CHUNK_VIEW_DISTANCE_Y * 2)

#endif
