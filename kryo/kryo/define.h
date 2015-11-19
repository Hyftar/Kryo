#ifndef KRYO_DEFINE_H
#define KRYO_DEFINE_H

#include "global.h"

typedef uint8_t BlockType;
enum BLOCK_TYPE { BTYPE_AIR, BTYPE_DIRT, BTYPE_GRASS };

#define CHUNK_SIZE_WIDTH 16
#define CHUNK_SIZE_HEIGHT 128
#define CHUNK_SIZE_DEPTH 16
#define CAMERA_SENSITIVITY 0.01f
#define BLOCK_TYPE_MAX sizeof(BlockType) << 8
#define TEXTUREATLAS_SIZE 16

#define TEXTURE_PATH "../kryo/media/textures/"
#define SHADER_PATH "../kryo/media/shaders/"
#define VIEW_DISTANCE 128
#define CHUNK_VIEW_DISTANCE_X 8
#define CHUNK_VIEW_DISTANCE_Y 8

#endif
