#include "chunk.h"
#include "define.h"

KRYO_BEGIN_NAMESPACE

#define KRYO_CHUNK_SIZE (m_width * m_height * m_depth)

Chunk::Chunk(int x, int y, int z)
    : m_blocks(x, y, z), m_width(x), m_height(y), m_depth(z) { }

Chunk::Chunk(Chunk &source)
    : m_width(source.m_width), m_height(source.m_height), m_depth(source.m_depth),
      m_blocks(Array3d<BlockType>(source.m_blocks)) { }

Chunk::~Chunk() { }

void Chunk::Remove(int idx)
{
    m_blocks.Set(idx, BTYPE_AIR);
}

void Chunk::Remove(int x, int y, int z)
{
    m_blocks.Set(x, y, z, BTYPE_AIR);
}

void Chunk::Set(int idx, BlockType type)
{
    m_blocks.Set(idx, type);
}

void Chunk::Set(int x, int y, int z, BlockType type)
{
    m_blocks.Set(x, y, z, type);
}

BlockType Chunk::Get(int idx) const
{
    return m_blocks.Get(idx);
}

BlockType Chunk::Get(int x, int y, int z) const
{
    return m_blocks.Get(x, y, z);
}

void Chunk::Reset(BlockType type)
{
    m_blocks.Reset(type);
}

int Chunk::GetBlockCount() const
{
    return KRYO_CHUNK_SIZE;
}

#undef KRYO_CHUNK_SIZE

KRYO_END_NAMESPACE
