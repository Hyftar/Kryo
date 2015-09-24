#include "define.h"
#include "chunk.h"

KRYO_BEGIN_NAMESPACE

Chunk::Chunk()
    : BlocArray3d(CHUNK_SIZE_WIDTH, CHUNK_SIZE_HEIGHT, CHUNK_SIZE_DEPTH) { }

// TODO: renommer afin d'uniformiser la nomenclature
void Chunk::RemoveBloc(int x, int y, int z)
{
    Set(x, y, z, BTYPE_AIR);
}

// TODO: déprécier/rétirer en faveur de la fonction équivalente de la classe sous-jacente.
void Chunk::SetBloc(int x, int y, int z, BlocType type)
{
    Set(x, y, z, type);
}

// TODO: déprécier/rétirer en faveur de la fonction équivalente de la classe sous-jacente.
BlocType Chunk::GetBloc(int x, int y, int z) const
{
    return Get(x, y, z);
}

KRYO_END_NAMESPACE
