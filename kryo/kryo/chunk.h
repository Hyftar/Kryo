#ifndef KRYO_CHUNK_H
#define KRYO_CHUNK_H

#include "global.h"
#include "define.h"
#include "chunkmesh.h"
#include "array3d.h"
#include "blockinfo.h"

KRYO_BEGIN_NAMESPACE

class Engine;

class Chunk
{
public:
    Chunk(int x = CHUNK_SIZE_WIDTH, int y = CHUNK_SIZE_HEIGHT, int z = CHUNK_SIZE_DEPTH);
    Chunk(Chunk& source);
    ~Chunk();
    int GetBlockCount() const;
    void Remove(int idx);
    void Remove(int x, int y, int z);
    void Set(int idx, BlockType type);
    void Set(int x, int y, int z, BlockType type);
    void Reset(BlockType type = BTYPE_AIR);
    void Update(Engine* engine);
    void Render() const;
    BlockType Get(int idx) const;
    BlockType Get(int x, int y, int z) const;
    bool IsDirty() const;

private:
    void ValidateCoordinates(int idx) const;
    void ValidateCoordinates(int x, int y, int z) const;
    void AddBlockToMesh(ChunkMesh::VertexData* vd, int& count, BlockInfo bi, int x, int y, int z);
    void PopulateArrayTest();

private:
    bool m_isDirty;
    int m_width, m_height, m_depth;
    ChunkMesh m_chunkMesh;
    Array3d<BlockType> m_blocks;
};

KRYO_END_NAMESPACE

#endif
