#include "chunkbuffer.h"

KRYO_BEGIN_NAMESPACE

ChunkBuffer::ChunkBuffer(int width, int height, int xBufferLength, int yBufferLength)
    : m_width(width), m_height(height), m_xBufferLength(xBufferLength), m_yBufferLength(yBufferLength)
{
    if (width % 2 == 0) width++;
    if (height % 2 == 0) height++;
    if (xBufferLength % 2 == 0) xBufferLength++;
    if (yBufferLength % 2 == 0) yBufferLength++;

    assert(xBufferLength >= width);
    assert(yBufferLength >= height);
    m_chunks = new Chunk**[xBufferLength];
    for (int x = 0; x < xBufferLength; x++)
        m_chunks[x] = new Chunk*[yBufferLength];
}

void ChunkBuffer::LoadChunks(Chunk*** chunks)
{
    std::copy(chunks, chunks + m_xBufferLength, m_chunks);
}

void ChunkBuffer::SwapBuffer(Chunk*** chunks)
{
    std::copy(m_chunks + m_xBufferLength, m_chunks + m_xBufferLength, m_chunks);
    std::copy(chunks, chunks + m_xBufferLength, m_chunks + m_xBufferLength);
}

Chunk* ChunkBuffer::Get(int x, int y) const
{
    assert(x >= 0 && x < m_xBufferLength);
    assert(y >= 0 && y < m_yBufferLength);
    return m_chunks[x][y];
}


BlockType ChunkBuffer::GetBlock_s(int chunkX, int chunkY, int nx, int ny, int nz) const
{
    if (ny < 0 || ny >= CHUNK_SIZE_HEIGHT)
        return BTYPE_AIR;

    chunkX += (m_xBufferLength / 2) + 1;
    chunkY += (m_yBufferLength / 2) + 1;

    chunkX += floor(nx / float(CHUNK_SIZE_WIDTH));
    int x = (nx < 0 ? CHUNK_SIZE_WIDTH + nx : nx) % CHUNK_SIZE_WIDTH;

    chunkY += floor(nz / float(CHUNK_SIZE_DEPTH));
    int z = (nz < 0 ? CHUNK_SIZE_DEPTH + nz : nz) % CHUNK_SIZE_DEPTH;

    if (chunkX < 0 || chunkY < 0 || chunkX >= m_xBufferLength || chunkY >= m_yBufferLength)
        return BTYPE_AIR;

    Chunk* chunk = Get(chunkX, chunkY);

    if (chunk == nullptr)
        return BTYPE_AIR;

    return chunk->Get(x, ny, z);
}

KRYO_END_NAMESPACE
