#include "blocarray3d.h"

KRYO_BEGIN_NAMESPACE

#define KRYO_BLOCKARRAY3D_SIZE (m_width * m_height * m_depth)
#define KRYO_COORDINATES_IDX(x, y, z) (x + (z * m_width) + (y * m_depth * m_width))

BlocArray3d::BlocArray3d(int width, int height, int depth)
    : m_width(width), m_height(height), m_depth(depth)
{
    m_blocks = new BlocType[KRYO_BLOCKARRAY3D_SIZE];
    Reset(BTYPE_AIR);
}

BlocArray3d::BlocArray3d(const BlocArray3d& source)
    : m_width(source.m_width), m_height(source.m_height), m_depth(source.m_depth)
{
    m_blocks = new BlocType[KRYO_BLOCKARRAY3D_SIZE];
    std::copy(source.m_blocks, source.m_blocks + KRYO_BLOCKARRAY3D_SIZE, m_blocks);
}

BlocArray3d::~BlocArray3d()
{
    delete[] m_blocks;
}

void BlocArray3d::Set(int x, int y, int z, BlocType type)
{
    m_blocks[KRYO_COORDINATES_IDX(x, y, z)] = type;
}

void BlocArray3d::Set(int idx, BlocType type)
{
    m_blocks[idx] = type;
}

BlocType BlocArray3d::Get(int x, int y, int z) const
{
    return m_blocks[KRYO_COORDINATES_IDX(x, y, z)];
}

BlocType BlocArray3d::Get(int idx) const
{
    return m_blocks[idx];
}

int BlocArray3d::GetIndexAt(int x, int y, int z) const
{
    return KRYO_COORDINATES_IDX(x, y, z);
}

int BlocArray3d::GetBlockCount() const
{
    return KRYO_BLOCKARRAY3D_SIZE;
}

void BlocArray3d::Reset(BlocType type)
{
    std::fill(m_blocks, m_blocks + KRYO_BLOCKARRAY3D_SIZE, type);
}

#undef KRYO_BLOCKARRAY3D_SIZE
#undef KRYO_COORDINATES_IDX

KRYO_END_NAMESPACE
