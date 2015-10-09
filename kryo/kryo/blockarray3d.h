#ifndef KRYO_BLOCKARRAY3D
#define KRYO_BLOCKARRAY3D

#include "global.h"
#include "define.h"

KRYO_BEGIN_NAMESPACE

class BlockArray3d
{
public:
    BlockArray3d(int width, int height, int depth);
    BlockArray3d(const BlockArray3d &source);
    ~BlockArray3d();
    void Set(int x, int y, int z, BlockType type);
    void Set(int idx, BlockType type);
    BlockType Get(int x, int y, int z) const;
    BlockType Get(int idx) const;
    int GetIndexAt(int x, int y, int z) const;
    int GetBlockCount() const;
    void Reset(BlockType type);

private:
    void ValidateCoordinates(int x, int y, int z) const;

private:
    int m_width, m_height, m_depth;
    BlockType* m_blocks;
};

KRYO_END_NAMESPACE

#endif
