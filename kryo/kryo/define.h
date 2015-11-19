#ifndef KRYO_DEFINE_H
#define KRYO_DEFINE_H

#include "global.h"

typedef uint8_t BlockType;
enum BLOCK_TYPE { BTYPE_AIR, BTYPE_DIRT, BTYPE_GRASS };

#define CHUNK_SIZE_WIDTH 16
#define CHUNK_SIZE_HEIGHT 128
#define CHUNK_SIZE_DEPTH 16
#define CAMERA_SENSITIVITY 0.01f

#define GRAVACC -9.80665
#define PLAYER_HEIGHT 1.7f
#define PLAYER_SPEED 2.f

#define BLOCK_HEIGHT 1.f
#define BLOCK_MARGIN .25f

#define TEXTURE_PATH "../kryo/media/textures/"
#define SHADER_PATH "../kryo/media/shaders/"
#define TEXTUREMAP_SIZE 4
#define VIEW_DISTANCE 128

#endif
