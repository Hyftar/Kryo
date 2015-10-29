#ifndef KRYO_ARRAY2D
#define KRYO_ARRAY2D

#include "global.h"
#include "define.h"

KRYO_BEGIN_NAMESPACE

#define KRYO_BLOCKARRAY2D_SIZE (m_width * m_height)
#define KRYO_COORDINATES_IDX(x, y) (x + (y * m_height))

template <class T>
class Array2d
{
public:
    Array2d(int width, int height);
    Array2d(const Array2d &source);
    ~Array2d();
    void Set(int x, int y, T type);
    void Set(int idx, T type);
    T Get(int x, int y) const;
    T Get(int idx) const;
    int GetIndexAt(int x, int y) const;
    int GetBlockCount() const;
    void Reset(T value);

private:
    void ValidateCoordinates(int x, int y) const;

private:
    int m_width, m_height;
    T* m_data;
};

template <class T>
Array2d<T>::Array2d(int width, int height)
    : m_width(width), m_height(height)
{
    assert(width > 0 && height > 0);
    m_data = new T[KRYO_BLOCKARRAY2D_SIZE];
    Reset(BTYPE_AIR);
}

template <class T>
Array2d<T>::Array2d(const Array2d& source)
    : m_width(source.m_width), m_height(source.m_height)
{
    m_data = new T[KRYO_BLOCKARRAY2D_SIZE];
    std::copy(source.m_data, source.m_data + KRYO_BLOCKARRAY2D_SIZE, m_data);
}

template <class T>
Array2d<T>::~Array2d()
{
    delete[] m_data;
}

template <class T>
void Array2d<T>::Set(int x, int y, T type)
{
    ValidateCoordinates(x, y);
    m_data[KRYO_COORDINATES_IDX(x, y)] = type;
}

template <class T>
void Array2d<T>::Set(int idx, T type)
{
    assert(idx >= 0 && idx < KRYO_BLOCKARRAY2D_SIZE);
    m_data[idx] = type;
}

template <class T>
T Array2d<T>::Get(int x, int y) const
{
    ValidateCoordinates(x, y);
    return m_data[KRYO_COORDINATES_IDX(x, y)];
}

template <class T>
T Array2d<T>::Get(int idx) const
{
    assert(idx >= 0 && idx < KRYO_BLOCKARRAY2D_SIZE);
    return m_data[idx];
}

template <class T>
int Array2d<T>::GetIndexAt(int x, int y) const
{
    ValidateCoordinates(x, y);
    return KRYO_COORDINATES_IDX(x, y);
}

template <class T>
int Array2d<T>::GetBlockCount() const
{
    return KRYO_BLOCKARRAY2D_SIZE;
}

template <class T>
void Array2d<T>::Reset(T value)
{
    std::fill(m_data, m_data + KRYO_BLOCKARRAY2D_SIZE, value);
}

template <class T>
void Array2d<T>::ValidateCoordinates(int x, int y) const
{
    assert(x >= 0 && y >= 0);
    assert(x < m_width && y < m_height);
}

#undef KRYO_BLOCKARRAY2D_SIZE
#undef KRYO_COORDINATES_IDX

KRYO_END_NAMESPACE

#endif
