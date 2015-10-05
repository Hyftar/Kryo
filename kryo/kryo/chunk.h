#ifndef KRYO_CHUNK_H
#define KRYO_CHUNK_H

#include "global.h"
#include "define.h"
#include "blockarray3d.h"

KRYO_BEGIN_NAMESPACE

class Chunk : public BlockArray3d
{
public:
    Chunk();
    ~Chunk();
    void RemoveBlock(int x, int y, int z);
    void SetBlock(int x, int y, int z, BlockType type);
    BlockType GetBlock(int x, int y, int z) const;
};

KRYO_END_NAMESPACE

#endif
