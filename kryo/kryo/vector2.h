#ifndef KRYO_VECTOR2_H
#define KRYO_VECTOR2_H

#include "global.h"
#include <cmath>

KRYO_BEGIN_NAMESPACE

template <class T>
class Vector2
{
public:
    Vector2(const T& nx = T(), const T& ny = T());
    ~Vector2();

    T Length() const;
    T Dot(const Vector2<T>& v) const;
    T Cross(const Vector2<T>& v) const;

    void Normalize();
    void Zero();

    Vector2<T> operator+(const Vector2<T>& v) const;
    Vector2<T> operator-(const Vector2<T>& v) const;
    Vector2<T> operator*(const Vector2<T>& v) const;
    Vector2<T> operator/(const Vector2<T>& v) const;

    Vector2<T> operator-() const;

    Vector2<T> operator+(const T& v) const;
    Vector2<T> operator-(const T& v) const;
    Vector2<T> operator*(const T& v) const;
    Vector2<T> operator/(const T& v) const;

    Vector2<T>& operator=(const Vector2<T>& v);
    Vector2<T>& operator=(const T& v);

    Vector2<T>& operator+=(const Vector2<T>& v);
    Vector2<T>& operator-=(const Vector2<T>& v);
    Vector2<T>& operator*=(const Vector2<T>& v);
    Vector2<T>& operator/=(const Vector2<T>& v);
    Vector2<T>& operator+=(const T& v);
    Vector2<T>& operator-=(const T& v);
    Vector2<T>& operator*=(const T& v);
    Vector2<T>& operator/=(const T& v);

    bool operator==(const Vector2<T>& v) const;
    bool operator!=(const Vector2<T>& v) const;

public:
    T x, y;
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;

template<class T>
inline std::ostream& operator<<(std::ostream& out, const Vector2<T>& v)
{
    out << "[" << v.x << ", " << v.y << "]";
    return out;
}

template<class T>
Vector2<T>::Vector2(const T& nx, const T& ny) : x(nx), y(ny) { }

template<class T>
Vector2<T>::~Vector2() { }

template<class T>
T Vector2<T>::Length() const
{
    return sqrt(x * x + y * y);
}

template<class T>
T Vector2<T>::Dot(const Vector2<T>& v) const
{
    return (x * v.x) + (y * v.y);
}

template<class T>
void Vector2<T>::Normalize()
{
    T n = Length();

    if (n != 0)
    {
        x /= n;
        y /= n;
    }
}

template<class T>
void Vector2<T>::Zero()
{
    x = y = 0;
}

template<class T>
T Vector2<T>::Cross(const Vector2<T>& v) const
{
    return (x * v.y) - (y * v.x);
}

template<class T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& v) const
{
    return Vector2<T>(x + v.x, y + v.y);
}

template<class T>
Vector2<T> Vector2<T>::operator+(const T& v) const
{
    return Vector2<T>(x + v, y + v);
}

template<class T>
Vector2<T> Vector2<T>::operator-(const Vector2<T>& v) const
{
    return Vector2<T>(x - v.x, y - v.y);
}

template<class T>
Vector2<T> Vector2<T>::operator*(const Vector2<T>& v) const
{
    return Cross(v);
}

template<class T>
Vector2<T> Vector2<T>::operator/(const Vector2<T>& v) const
{
    return Vector2<T>(x / v.x, y / v.y);
}

template<class T>
Vector2<T> Vector2<T>::operator-() const
{
    return Vector2<T>(-x, -y);
}

template<class T>
Vector2<T> Vector2<T>::operator-(const T& v) const
{
    return Vector2<T>(x - v, y - v);
}

template<class T>
Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& v)
{
    return (*this = *this + v);
}

template<class T>
Vector2<T>& Vector2<T>::operator+=(const T& v)
{
    return (*this = *this + v);
}

template<class T>
Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& v)
{
    return (*this = *this - v);
}

template<class T>
Vector2<T>& Vector2<T>::operator-=(const T& v)
{
    return (*this = *this - v);
}

template<class T>
Vector2<T> Vector2<T>::operator*(const T& v) const
{
    return Vector2<T>(x * v, y * v);
}

template<class T>
Vector2<T> Vector2<T>::operator/(const T& v) const
{
    return Vector2<T>(x / v, y / v);
}

template<class T>
Vector2<T>& Vector2<T>::operator=(const Vector2<T>& v)
{
    x = v.x;
    y = v.y;
    return *this;
}

template<class T>
Vector2<T>& Vector2<T>::operator=(const T& v)
{
    x = v;
    y = v;
    return *this;
}

template<class T>
Vector2<T>& Vector2<T>::operator*=(const Vector2<T>& v)
{
    return (*this = *this * v);
}

template<class T>
Vector2<T>& Vector2<T>::operator/=(const Vector2<T>& v)
{
    return (*this = *this / v);
}

template<class T>
Vector2<T>& Vector2<T>::operator*=(const T& v)
{
    return (*this = *this * v);
}

template<class T>
Vector2<T>& Vector2<T>::operator/=(const T& v)
{
    return (*this = *this / v);
}

template<class T>
bool Vector2<T>::operator==(const Vector2<T>& v) const
{
    return (x == v.x && y == v.y);
}

template<class T>
bool Vector2<T>::operator!=(const Vector2<T>& v) const
{
    return !(*this == v);
}

KRYO_END_NAMESPACE

#endif
