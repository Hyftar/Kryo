// TODO: Renommer 'BlocType' en 'BlockType'
#ifndef KRYO_DEFINE_H
#define KRYO_DEFINE_H

enum BlocType
{
    BTYPE_AIR,
    BTYPE_DIRT,
    BTYPE_GRASS
};

// Les macros ont été renommées dans l'optique d'améliorer la sémantique
//#define CHUNK_SIZE_X 16
//#define CHUNK_SIZE_Y 128
//#define CHUNK_SIZE_Z 16

#define CHUNK_SIZE_WIDTH 16
#define CHUNK_SIZE_HEIGHT 128
#define CHUNK_SIZE_DEPTH 16

#endif
