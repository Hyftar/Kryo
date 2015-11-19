#include "chunkbuffer.h"

KRYO_BEGIN_NAMESPACE

ChunkBuffer::ChunkBuffer(int width, int height, int xBufferLength, int yBufferLength)
    : m_width(width), m_height(height), m_xBufferLength(xBufferLength), m_yBufferLength(yBufferLength)
{
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

KRYO_END_NAMESPACE
