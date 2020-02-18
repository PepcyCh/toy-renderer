#pragma once

#include "defines.h"

#include <cmath>
#include <iostream>

namespace toy_renderer {

template <typename T>
class Vector3;
template <typename T>
class Point2;
template <typename T>
class Point3;

// Vector2
template <typename T>
class Vector2 {
  public:
    Vector2() : x(0), y(0) {}
    Vector2(T x, T y) : x(x), y(y) {}
    explicit Vector2(T x) : x(x), y(x) {}
    explicit Vector2(const Vector3<T> &v) : x(v.x), y(v.y) {}
    explicit Vector2(const Point2<T> &p) : x(p.x), y(p.y) {}
    explicit Vector2(const Point3<T> &p) : x(p.x), y(p.y) {}

    T &operator[](int i) {
        return i == 0 ? x : y;
    }
    T operator[](int i) const {
        return i == 0 ? x : y;
    }

    bool operator==(const Vector2<T> &v) const {
        return x == v.x && y == v.y;
    }
    bool operator!=(const Vector2<T> &v) const {
        return x != v.x || y != v.y;
    }

    Vector2<T> operator+(const Vector2<T> &v) const {
        return Vector2<T>(x + v.x, y + v.y);
    }
    Vector2<T> &operator+=(const Vector2<T> &v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2<T> operator-(const Vector2<T> &v) const {
        return Vector2<T>(x - v.x, y - v.y);
    }
    Vector2<T> &operator-=(const Vector2<T> &v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    template <typename U>
    Vector2<T> operator*(U s) const {
        return Vector2<T>(x * s, y * s);
    }
    template <typename U>
    Vector2<T> &operator*=(U s) {
        x *= s;
        y *= s;
        return *this;
    }

    template <typename U>
    Vector2<T> operator/(U s) const {
        Float inv = Float(1) / s;
        return Vector2<T>(x * inv, y * inv);
    }
    template <typename U>
    Vector2<T> operator/=(U s) {
        Float inv = Float(1) / s;
        x *= inv;
        y *= inv;
        return *this;
    }

    Vector2<T> operator-() const {
        return Vector2<T>(-x, -y);
    }

    Float Norm2() const {
        return x * x + y * y;
    }
    Float Norm() const {
        return std::sqrt(Norm2());
    }

    T x, y;
};

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const Vector2<T> &v) {
    return out << "Vector[ " << v.x << ", " << v.y << " ]";
}

template <typename T>
inline Vector2<T> Normalize(const Vector2<T> &v) {
    return v / v.Norm();
}

template <typename T>
inline T Dot(const Vector2<T> &v1, const Vector2<T> &v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

template <typename T>
inline T Cross(const Vector2<T> &v1, const Vector2<T> &v2) {
    return v1.x * v2.y - v1.y * v2.x;
}

using Vector2f = Vector2<Float>;
using Vector2i = Vector2<int>;

}
