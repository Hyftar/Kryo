#include "chunk.h"

KRYO_BEGIN_NAMESPACE

Chunk::Chunk()
    : BlockArray3d(CHUNK_SIZE_WIDTH, CHUNK_SIZE_HEIGHT, CHUNK_SIZE_DEPTH) { }

Chunk::~Chunk() { }

// TODO: renommer afin d'uniformiser la nomenclature
void Chunk::RemoveBlock(int x, int y, int z)
{
    Set(x, y, z, BTYPE_AIR);
}

// TODO: déprécier/rétirer en faveur de la fonction équivalente de la classe sous-jacente.
void Chunk::SetBlock(int x, int y, int z, BlockType type)
{
    Set(x, y, z, type);
}

// TODO: déprécier/rétirer en faveur de la fonction équivalente de la classe sous-jacente.
BlockType Chunk::GetBlock(int x, int y, int z) const
{
    return Get(x, y, z);
}

KRYO_END_NAMESPACE
