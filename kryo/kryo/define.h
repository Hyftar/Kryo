#ifndef KRYO_DEFINE_H
#define KRYO_DEFINE_H

enum BlockType
{
    BTYPE_AIR,
    BTYPE_DIRT,
    BTYPE_GRASS
};

#define CHUNK_SIZE_WIDTH 16
#define CHUNK_SIZE_HEIGHT 128
#define CHUNK_SIZE_DEPTH 16

#define TEXTURE_PATH "../kryo/media/textures/"
#define VIEW_DISTANCE 128

#endif
