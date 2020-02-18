#pragma once

#include "defines.h"
#include "Vector2.h"

#include <iostream>

namespace toy_renderer {

template <typename T>
class Point3;

// Point2
template <typename T>
class Point2 {
  public:
    Point2() : x(0), y(0) {}
    Point2(T x, T y) : x(x), y(y) {}
    explicit Point2(T x) : x(x), y(x) {}
    explicit Point2(const Point3<T> &p) : x(p.x), y(p.y) {}
    explicit Point2(const Vector2<T> &v) : x(v.x), y(v.y) {}
    explicit Point2(const Vector3<T> &v) : x(v.x), y(v.y) {}

    T &operator[](int i) {
        return i == 0 ? x : y;
    }
    T operator[](int i) const {
        return i == 0 ? x : y;
    }

    bool operator==(const Point2<T> &p) const {
        return x == p.x && y == p.y;
    }
    bool operator!=(const Point2<T> &p) const {
        return x != p.x || y != p.y;
    }

    Point2<T> operator+(const Vector2<T> &v) const {
        return Point2<T>(x + v.x, y + v.y);
    }
    Point2<T> &operator+=(const Vector2<T> &v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    Point2<T> operator+(const Point2<T> &p) const {
        return Point2<T>(x + p.x, y + p.y);
    }
    Point2<T> &operator+=(const Point2<T> &p) {
        x += p.x;
        y += p.y;
        return *this;
    }

    Vector2<T> operator-(const Point2<T> &p) const {
        return Vector2<T>(x - p.x, y - p.y);
    }
    Point2<T> operator-(const Vector2<T> &v) const {
        return Point2<T>(x - v.x, y - v.y);
    }
    Point2<T> &operator-=(const Vector2<T> &v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    template <typename U>
    Point2<T> operator*(U s) const {
        return Point2<T>(x * s, y * s);
    }
    template <typename U>
    Point2<T> &operator*=(U s) {
        x *= s;
        y *= s;
        return *this;
    }

    template <typename U>
    Point2<T> operator/(U s) const {
        Float inv = Float(1) / s;
        return Point2<T>(x * inv, y * inv);
    }
    template <typename U>
    Point2<T> operator/=(U s) {
        Float inv = Float(1) / s;
        x *= inv;
        y *= inv;
        return *this;
    }

    Point2<T> operator-() const {
        return Point2<T>(-x, -y);
    }

    T x, y;
};

template <typename T>
inline std::ostream &operator<<(std::ostream &out, const Point2<T> &p) {
    return out << "Point[ " << p.x << ", " << p.y << " ]";
}

using Point2f = Point2<Float>;
using Point2i = Point2<int>;

}
