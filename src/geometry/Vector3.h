#pragma once

#include "defines.h"

#include <cmath>
#include <iostream>

namespace toy_renderer {

template <typename T>
class Point3;
template <typename T>
class Normal3;

// Vector3
template <typename T>
class Vector3 {
  public:
    Vector3() : x(0), y(0), z(0) {}
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
    explicit Vector3(T x) : x(x), y(x), z(x) {}
    explicit Vector3(const Point3<T> &p) : x(p.x), y(p.y), z(p.z) {}
    explicit Vector3(const Normal3<T> &n) : x(n.x), y(n.y), z(n.z) {}

    T &operator[](int i) {
        return i == 0 ? x : (i == 1 ? y : z);
    }
    T operator[](int i) const {
        return i == 0 ? x : (i == 1 ? y : z);
    }

    bool operator==(const Vector3<T> &v) const {
        return x == v.x && y == v.y && z == v.z;
    }
    bool operator!=(const Vector3<T> &v) const {
        return x != v.x || y != v.y || z != v.z;
    }

    Vector3<T> operator+(const Vector3<T> &v) const {
        return Vector3<T>(x + v.x, y + v.y, z + v.z);
    }
    Vector3<T> &operator+=(const Vector3<T> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3<T> operator-(const Vector3<T> &v) const {
        return Vector3<T>(x - v.x, y - v.y, z - v.z);
    }
    Vector3<T> &operator-=(const Vector3<T> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template <typename U>
    Vector3<T> operator*(U s) const {
        return Vector3<T>(x * s, y * s, z * s);
    }
    template <typename U>
    Vector3<T> &operator*=(U s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    template <typename U>
    Vector3<T> operator/(U s) const {
        Float inv = Float(1) / s;
        return Vector3<T>(x * inv, y * inv, z * inv);
    }
    template <typename U>
    Vector3<T> operator/=(U s) {
        Float inv = Float(1) / s;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    Vector3<T> operator-() const {
        return Vector3<T>(-x, -y, -z);
    }

    Float Norm2() const {
        return x * x + y * y + z * z;
    }
    Float Norm() const {
        return std::sqrt(Norm2());
    }

    T x, y, z;
};

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const Vector3<T> &v) {
    return out << "Vector[ " << v.x << ", " << v.y << ", " << v.z << " ]";
}

template <typename T>
inline Vector3<T> Normalize(const Vector3<T> &v) {
    return v / v.Norm();
}

template <typename T>
inline T Dot(const Vector3<T> &v1, const Vector3<T> &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
inline Vector3<T> Cross(const Vector3<T> &v1, const Vector3<T> &v2) {
    return Vector3<T>(v1.y * v2.z - v1.z * v2.y,
                      v1.z * v2.x - v1.x * v2.z,
                      v1.x * v2.y - v1.y * v2.x);
}

using Vector3f = Vector3<Float>;
using Vector3i = Vector3<int>;

}
