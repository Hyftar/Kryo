#ifndef KRYO_VECTOR_H
#define KRYO_VECTOR_H

#include "global.h"
#include <cmath>

KRYO_BEGIN_NAMESPACE

template <class T>
class Vector3
{
public:
    Vector3(const T& nx = T(), const T& ny = T(), const T& nz = T());
    ~Vector3();

    T Length() const;
    T Dot(const Vector3<T>& v) const;

    void Normalize();
    void Zero();

    Vector3<T> Cross(const Vector3<T>& v) const;

    Vector3<T> operator+(const Vector3<T>& v) const;
    Vector3<T> operator-(const Vector3<T>& v) const;
    Vector3<T> operator*(const Vector3<T>& v) const;
    Vector3<T> operator/(const Vector3<T>& v) const;

    Vector3<T> operator-() const;

    Vector3<T> operator+(const T& v) const;
    Vector3<T> operator-(const T& v) const;
    Vector3<T> operator*(const T& v) const;
    Vector3<T> operator/(const T& v) const;

    Vector3<T>& operator=(const Vector3<T>& v);
    Vector3<T>& operator=(const T& v);

    Vector3<T>& operator+=(const Vector3<T>& v);
    Vector3<T>& operator-=(const Vector3<T>& v);
    Vector3<T>& operator*=(const Vector3<T>& v);
    Vector3<T>& operator/=(const Vector3<T>& v);
    Vector3<T>& operator+=(const T& v);
    Vector3<T>& operator-=(const T& v);
    Vector3<T>& operator*=(const T& v);
    Vector3<T>& operator/=(const T& v);

    bool operator==(const Vector3<T>& v) const;
    bool operator!=(const Vector3<T>& v) const;
public:
    T x, y, z;
};

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

template<class T>
inline std::ostream& operator<<(std::ostream& out, const Vector3<T>& v)
{
    out << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return out;
}

template<class T>
Vector3<T>::Vector3(const T& nx, const T& ny, const T& nz) : x(nx), y(ny), z(nz) { }

template<class T>
Vector3<T>::~Vector3() { }

template<class T>
T Vector3<T>::Length() const
{
    return sqrt(x * x + y * y + z * z);
}

template<class T>
T Vector3<T>::Dot(const Vector3<T>& v) const
{
    return (x * v.x) + (y * v.y) + (z * v.z);
}

template<class T>
void Vector3<T>::Normalize()
{
    T n = Length();

    if (n != 0)
    {
        x /= n;
        y /= n;
        z /= n;
    }
}

template<class T>
void Vector3<T>::Zero()
{
    x = y = z = 0;
}

template<class T>
Vector3<T> Vector3<T>::Cross(const Vector3<T>& v) const
{
    return Vector3<T>(y * v.z - v.y * z,
                      z * v.x - v.z * x,
                      x * v.y - v.x * y);
}

template<class T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& v) const
{
    return Vector3<T>(x + v.x, y + v.y, z + v.z);
}

template<class T>
Vector3<T> Vector3<T>::operator+(const T& v) const
{
    return Vector3<T>(x + v, y + v, z + v);
}

template<class T>
Vector3<T> Vector3<T>::operator-(const Vector3<T>& v) const
{
    return Vector3<T>(x - v.x, y - v.y, z - v.z);
}

template<class T>
Vector3<T> Vector3<T>::operator*(const Vector3<T>& v) const
{
    return Cross(v);
}

template<class T>
Vector3<T> Vector3<T>::operator/(const Vector3<T>& v) const
{
    return Vector3<T>(x / v.x, y / v.y, z / v.z);
}

template<class T>
Vector3<T> Vector3<T>::operator-() const
{
    return Vector3<T>(-x, -y, -z);
}

template<class T>
Vector3<T> Vector3<T>::operator-(const T& v) const
{
    return Vector3<T>(x - v, y - v, z - v);
}

template<class T>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& v)
{
    return (*this = *this + v);
}

template<class T>
Vector3<T>& Vector3<T>::operator+=(const T& v)
{
    return (*this = *this + v);
}

template<class T>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& v)
{
    return (*this = *this - v);
}

template<class T>
Vector3<T>& Vector3<T>::operator-=(const T& v)
{
    return (*this = *this - v);
}

template<class T>
Vector3<T> Vector3<T>::operator*(const T& v) const
{
    return Vector3<T>(x * v, y * v, z * v);
}

template<class T>
Vector3<T> Vector3<T>::operator/(const T& v) const
{
    return Vector3<T>(x / v, y / v, z / v);
}

template<class T>
Vector3<T>& Vector3<T>::operator=(const Vector3<T>& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

template<class T>
Vector3<T>& Vector3<T>::operator=(const T& v)
{
    x = v;
    y = v;
    z = v;
    return *this;
}

template<class T>
Vector3<T>& Vector3<T>::operator*=(const Vector3<T>& v)
{
    return (*this = *this * v);
}

template<class T>
Vector3<T>& Vector3<T>::operator/=(const Vector3<T>& v)
{
    return (*this = *this / v);
}

template<class T>
Vector3<T>& Vector3<T>::operator*=(const T& v)
{
    return (*this = *this * v);
}

template<class T>
Vector3<T>& Vector3<T>::operator/=(const T& v)
{
    return (*this = *this / v);
}

template<class T>
bool Vector3<T>::operator==(const Vector3<T>& v) const
{
    return (x == v.x && y == v.y && z == v.z);
}

template<class T>
bool Vector3<T>::operator!=(const Vector3<T>& v) const
{
    return !(*this == v);
}

KRYO_END_NAMESPACE

#endif
