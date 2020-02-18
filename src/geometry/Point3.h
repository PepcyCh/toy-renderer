#pragma once

#include "defines.h"
#include "Vector3.h"

#include <iostream>

namespace toy_renderer {

// Point3
template <typename T>
class Point3 {
  public:
    Point3() : x(0), y(0), z(0) {}
    Point3(T x, T y, T z) : x(x), y(y), z(z) {}
    explicit Point3(T x) : x(x), y(x), z(x) {}
    explicit Point3(const Vector3<T> &v) : x(v.x), y(v.y), z(v.z) {}

    T &operator[](int i) {
        return i == 0 ? x : (i == 1 ? y : z);
    }
    T operator[](int i) const {
        return i == 0 ? x : (i == 1 ? y : z);
    }

    bool operator==(const Point3<T> &p) const {
        return x == p.x && y == p.y && z == p.z;
    }
    bool operator!=(const Point3<T> &p) const {
        return x != p.x || y != p.y || z != p.z;
    }

    Point3<T> operator+(const Vector3<T> &v) const {
        return Point3<T>(x + v.x, y + v.y, z + v.z);
    }
    Point3<T> &operator+=(const Vector3<T> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Point3<T> operator+(const Point3<T> &p) const {
        return Point3<T>(x + p.x, y + p.y, z + p.z);
    }
    Point3<T> &operator+=(const Point3<T> &p) {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }

    Vector3<T> operator-(const Point3<T> &p) const {
        return Vector3<T>(x - p.x, y - p.y, z - p.z);
    }
    Point3<T> operator-(const Vector3<T> &v) const {
        return Point3<T>(x - v.x, y - v.y, z - v.z);
    }
    Point3<T> &operator-=(const Vector3<T> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    template <typename U>
    Point3<T> operator*(U s) const {
        return Point3<T>(x * s, y * s, z * s);
    }
    template <typename U>
    Point3<T> &operator*=(U s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    template <typename U>
    Point3<T> operator/(U s) const {
        Float inv = Float(1) / s;
        return Point3<T>(x * inv, y * inv, z * inv);
    }
    template <typename U>
    Point3<T> operator/=(U s) {
        Float inv = Float(1) / s;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    Point3<T> operator-() const {
        return Point3<T>(-x, -y, -z);
    }

    T x, y, z;
};

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const Point3<T> &p) {
    return out << "Point[ " << p.x << ", " << p.y << ", " << p.z << " ]";
}

using Point3f = Point3<Float>;
using Point3i = Point3<int>;

}
