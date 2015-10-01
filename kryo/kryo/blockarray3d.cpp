#include "blockarray3d.h"

KRYO_BEGIN_NAMESPACE

#define KRYO_BlockArray3d_SIZE (m_width * m_height * m_depth)
#define KRYO_COORDINATES_IDX(x, y, z) (x + (z * m_width) + (y * m_depth * m_width))

BlockArray3d::BlockArray3d(int width, int height, int depth)
    : m_width(width), m_height(height), m_depth(depth)
{
    assert(width > 0 && height > 0 && depth > 0);
    m_blocks = new BlockType[KRYO_BlockArray3d_SIZE];
    Reset(BTYPE_AIR);
}

BlockArray3d::BlockArray3d(const BlockArray3d& source)
    : m_width(source.m_width), m_height(source.m_height), m_depth(source.m_depth)
{
    m_blocks = new BlockType[KRYO_BlockArray3d_SIZE];
    std::copy(source.m_blocks, source.m_blocks + KRYO_BlockArray3d_SIZE, m_blocks);
}

BlockArray3d::~BlockArray3d()
{
    delete[] m_blocks;
}

void BlockArray3d::Set(int x, int y, int z, BlockType type)
{
    ValidateCoordinates(x, y, z);
    m_blocks[KRYO_COORDINATES_IDX(x, y, z)] = type;
}

void BlockArray3d::Set(int idx, BlockType type)
{
    assert(idx >= 0 && idx < KRYO_BlockArray3d_SIZE);
    m_blocks[idx] = type;
}

BlockType BlockArray3d::Get(int x, int y, int z) const
{
    ValidateCoordinates(x, y, z);
    return m_blocks[KRYO_COORDINATES_IDX(x, y, z)];
}

BlockType BlockArray3d::Get(int idx) const
{
    assert(idx >= 0 && idx < KRYO_BlockArray3d_SIZE);
    return m_blocks[idx];
}

int BlockArray3d::GetIndexAt(int x, int y, int z) const
{
    ValidateCoordinates(x, y, z);
    return KRYO_COORDINATES_IDX(x, y, z);
}

int BlockArray3d::GetBlockCount() const
{
    return KRYO_BlockArray3d_SIZE;
}

void BlockArray3d::Reset(BlockType type)
{
    std::fill(m_blocks, m_blocks + KRYO_BlockArray3d_SIZE, type);
}

void BlockArray3d::ValidateCoordinates(int x, int y, int z) const
{
    assert(x >= 0 && y >= 0 && z >= 0);
    assert(x < m_width && y < m_height && z < m_depth);
}

#undef KRYO_BlockArray3d_SIZE
#undef KRYO_COORDINATES_IDX

KRYO_END_NAMESPACE
