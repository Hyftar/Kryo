#ifndef KRYO_CHUNK_H
#define KRYO_CHUNK_H

#include "global.h"
#include "define.h"
#include "chunkmesh.h"
#include "array3d.h"
#include "blockinfo.h"
#include "array2d.h"
#include "chunkbuffer.h"

KRYO_BEGIN_NAMESPACE

class ChunkBuffer;
class Engine;

class Chunk
{
public:
    Chunk(Engine* engine);
    Chunk(Chunk& source);
    ~Chunk();
    void Remove(int idx);
    void Remove(int x, int y, int z);
    void Set(int idx, BlockType Type);
    void Set(int x, int y, int z, BlockType type);
    void Reset(BlockType type = BTYPE_AIR);
    void Update(float absX, const float absY, const int chunkX, const int chunkY);
    void Render() const;
    BlockType Get(int idx) const;
    BlockType Get(int x, int y, int z) const;
    BlockType Get_s(int x, int y, int z) const;
    bool IsDirty() const;

private:
    void AddBlockToMesh(ChunkMesh::VertexData* vd, int& count, BlockInfo* bi,
        float absX, float absY, int chunkX,
        int chunkY, int x, int y, int z);
    void PopulateArrayTest();

private:
    Engine* m_engine;
    bool m_isDirty;
    ChunkMesh m_chunkMesh;
    Array3d<BlockType> m_blocks;
    ChunkBuffer* m_chunks;
};

KRYO_END_NAMESPACE

#endif
