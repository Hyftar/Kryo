#ifndef KRYO_ARRAY3D_H
#define KRYO_ARRAY3D_H

#include "global.h"
#include "define.h"

KRYO_BEGIN_NAMESPACE

#define KRYO_ARRAY3D_SIZE (m_width * m_height * m_depth)
#define KRYO_COORDINATES_IDX(x, y, z) (x + (z * m_width) + (y * m_depth * m_width))

template <class T>
class Array3d
{
public:
    Array3d(int width, int height, int depth);
    Array3d(int width, int height, int depth, T defaultVal);
    Array3d(const Array3d &source);
    ~Array3d();
    void Set(int x, int y, int z, T type);
    void Set(int idx, T type);
    T Get(int x, int y, int z) const;
    T Get(int idx) const;
    int GetIndexAt(int x, int y, int z) const;
    int GetSize() const;
    int GetWidth() const;
    int GetHeight() const;
    int GetDepth() const;
    void Reset(T value);

private:
    void ValidateCoordinates(int x, int y, int z) const;

private:
    int m_width, m_height, m_depth;
    T* m_data;
};

template <class T>
Array3d<T>::Array3d(int width, int height, int depth)
    : m_width(width), m_height(height), m_depth(depth)
{
    assert(width > 0 && height > 0 && depth > 0);
    m_data = new T[KRYO_ARRAY3D_SIZE];
    Reset(T());
}

template<class T>
Array3d<T>::Array3d(int width, int height, int depth, T defaultVal) : m_width(width), m_height(height), m_depth(depth)
{
    assert(width > 0 && height > 0 && depth > 0);
    m_data = new T[KRYO_ARRAY3D_SIZE];
    Reset(defaultVal);
}

template <class T>
Array3d<T>::Array3d(const Array3d& source)
    : m_width(source.m_width), m_height(source.m_height), m_depth(source.m_depth)
{
    m_data = new T[KRYO_ARRAY3D_SIZE];
    std::copy(source.m_data, source.m_data + KRYO_ARRAY3D_SIZE, m_data);
}

template <class T>
Array3d<T>::~Array3d()
{
    delete[] m_data;
}

template <class T>
void Array3d<T>::Set(int x, int y, int z, T type)
{
    ValidateCoordinates(x, y, z);
    m_data[KRYO_COORDINATES_IDX(x, y, z)] = type;
}

template <class T>
void Array3d<T>::Set(int idx, T type)
{
    assert(idx >= 0 && idx < KRYO_ARRAY3D_SIZE);
    m_data[idx] = type;
}

template <class T>
T Array3d<T>::Get(int x, int y, int z) const
{
    ValidateCoordinates(x, y, z);
    return m_data[KRYO_COORDINATES_IDX(x, y, z)];
}

template <class T>
T Array3d<T>::Get(int idx) const
{
    assert(idx >= 0 && idx < KRYO_ARRAY3D_SIZE);
    return m_data[idx];
}

template <class T>
int Array3d<T>::GetIndexAt(int x, int y, int z) const
{
    ValidateCoordinates(x, y, z);
    return KRYO_COORDINATES_IDX(x, y, z);
}

template <class T>
int Array3d<T>::GetSize() const
{
    return KRYO_ARRAY3D_SIZE;
}

template <class T>
int Array3d<T>::GetWidth() const
{
    return m_width;
}

template <class T>
int Array3d<T>::GetHeight() const
{
    return m_height;
}

template <class T>
int Array3d<T>::GetDepth() const
{
    return m_depth;
}

template <class T>
void Array3d<T>::Reset(T value)
{
    std::fill(m_data, m_data + KRYO_ARRAY3D_SIZE, value);
}

template <class T>
void Array3d<T>::ValidateCoordinates(int x, int y, int z) const
{
    assert(x >= 0 && y >= 0 && z >= 0);
    assert(x < m_width && y < m_height && z < m_depth);
}

#undef KRYO_ARRAY3D_SIZE
#undef KRYO_COORDINATES_IDX

KRYO_END_NAMESPACE

#endif
