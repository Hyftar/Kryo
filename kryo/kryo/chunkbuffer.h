#ifndef KRYO_CHUNKBUFFER_H
#define KRYO_CHUNKBUFFER_H

#include "global.h"
#include "define.h"
#include "chunkbuffer.h"
#include "chunk.h"

KRYO_BEGIN_NAMESPACE

class Chunk;

class ChunkBuffer {
public:
    ChunkBuffer(int width, int height, int xBufferLength, int yBufferLength);
    void LoadChunks(Chunk*** chunks);
    void SwapBuffer(Chunk*** chunks);
    Chunk* Get(int x, int y) const;
    BlockType GetBlock_s(int chunkX, int chunkY, int x, int y, int z) const;

protected:
    int m_width, m_height;
    int m_xBufferLength, m_yBufferLength;
    Chunk*** m_chunks;
};

KRYO_END_NAMESPACE

#endif
