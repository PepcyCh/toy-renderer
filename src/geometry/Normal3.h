#pragma once

#include "defines.h"

#include <cmath>
#include <iostream>

namespace toy_renderer {

template <typename T>
class Vector3;

// Normal3
template <typename T>
class Normal3 {
  public:
    Normal3() : x(0), y(0), z(0) {}
    Normal3(T x, T y, T z) : x(x), y(y), z(z) {}
    explicit Normal3(T x) : x(x), y(x), z(x) {}
    explicit Normal3(const Vector3<T> &v) : x(v.x), y(v.y), z(v.z) {}

    T &operator[](int i) {
        return i == 0 ? x : (i == 1 ? y : z);
    }
    T operator[](int i) const {
        return i == 0 ? x : (i == 1 ? y : z);
    }

    bool operator==(const Normal3<T> &n) const {
        return x == n.x && y == n.y && z == n.z;
    }
    bool operator!=(const Normal3<T> &n) const {
        return x != n.x || y != n.y || z != n.z;
    }

    Normal3<T> operator+(const Normal3<T> &n) const {
        return Normal3<T>(x + n.x, y + n.y, z + n.z);
    }
    Normal3<T> &operator+=(const Normal3<T> &n) {
        x += n.x;
        y += n.y;
        z += n.z;
        return *this;
    }

    Normal3<T> operator-(const Normal3<T> &n) const {
        return Normal3<T>(x - n.x, y - n.y, z - n.z);
    }
    Normal3<T> &operator-=(const Normal3<T> &n) {
        x -= n.x;
        y -= n.y;
        z -= n.z;
        return *this;
    }

    template <typename U>
    Normal3<T> operator*(U s) const {
        return Normal3<T>(x * s, y * s, z * s);
    }
    template <typename U>
    Normal3<T> &operator*=(U s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    template <typename U>
    Normal3<T> operator/(U s) const {
        Float inv = Float(1) / s;
        return Normal3<T>(x * inv, y * inv, z * inv);
    }
    template <typename U>
    Normal3<T> operator/=(U s) {
        Float inv = Float(1) / s;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    Normal3<T> operator-() const {
        return Normal3<T>(-x, -y, -z);
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
inline std::ostream &operator<<(std::ostream &out, const Normal3<T> &n) {
    return out << "Normal[ " << n.x << ", " << n.y << ", " << n.z << " ]";
}

template <typename T>
inline Normal3<T> Normalize(const Normal3<T> &n) {
    return n / n.Norm();
}

using Normal3f = Normal3<Float>;
using Normal3i = Normal3<int>;

}
