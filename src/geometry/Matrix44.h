#pragma once

#include "defines.h"
#include "Vector3.h"
#include "Point3.h"
#include "Normal3.h"

#include <cstring>
#include <iostream>

namespace toy_renderer {
    
class Matrix44;

// 4x4 Float matrix
Matrix44 Mul(const Matrix44 &m1, const Matrix44 &m2);
Matrix44 Transpose(const Matrix44 &m);
Matrix44 Inverse(const Matrix44 &m);

class Matrix44 {
  public:
    Matrix44() {
        m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
        m[0][1] = m[0][2] = m[0][3] = 0;
        m[1][0] = m[1][2] = m[1][3] = 0;
        m[2][0] = m[2][1] = m[2][3] = 0;
        m[3][0] = m[3][1] = m[3][2] = 0;
    }
    Matrix44(const Float mat[4][4]) {
        std::memcpy(m, mat, 16 * sizeof(Float));
    }
    Matrix44(Float m00, Float m01, Float m02, Float m03,
             Float m10, Float m11, Float m12, Float m13,
             Float m20, Float m21, Float m22, Float m23,
             Float m30, Float m31, Float m32, Float m33) {
        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[0][3] = m03;
        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[1][3] = m13;
        m[2][0] = m20;
        m[2][1] = m21;
        m[2][2] = m22;
        m[2][3] = m23;
        m[3][0] = m30;
        m[3][1] = m31;
        m[3][2] = m32;
        m[3][3] = m33;
    }

    Float *operator[](int i) {
        return m[i];
    }
    const Float *operator[](int i) const {
        return m[i];
    }

    bool operator==(const Matrix44 &mat) const {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (m[i][j] != mat[i][j])
                    return false;
        return true;
    }
    bool operator!=(const Matrix44 &mat) const {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (m[i][j] == mat[i][j])
                    return false;
        return true;
    }

    Matrix44 operator*(const Matrix44 &m) const {
        return Mul(*this, m);
    }

    template <typename T>
    Vector3<T> operator()(const Vector3<T> &v) const {
        T x = v.x, y = v.y, z = v.z;
        return Vector3<T>(m[0][0] * x + m[0][1] * y + m[0][2] * z,
                          m[1][0] * x + m[1][1] * y + m[1][2] * z,
                          m[2][0] * x + m[2][1] * y + m[2][2] * z);
    }
    template <typename T>
    Vector3<T> operator*(const Vector3<T> &v) const {
        return (*this)(v);
    }

    template <typename T>
    Point3<T> operator()(const Point3<T> &p) const {
        T x = p.x, y = p.y, z = p.z;
        return Point3<T>(m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3],
                        m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3],
                        m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3]);
    }
    template <typename T>
    Point3<T> operator*(const Point3<T> &p) const {
        return (*this)(p);
    }

    template <typename T>
    Normal3<T> operator()(const Normal3<T> &n) const {
        T x = n.x, y = n.y, z = n.z;
        Matrix44 t = Inverse(*this);
        return Normal3<T>(t[0][0] * x + t[1][0] * y + t[2][0] * z,
                          t[0][1] * x + t[1][1] * y + t[2][1] * z,
                          t[0][2] * x + t[1][2] * y + t[2][2] * z);
    }

    Float m[4][4];
};

inline std::ostream &operator<<(std::ostream &out, const Matrix44 &m) {
    return out << "[ [ " << m[0][0] << ", " << m[0][1] << ", " <<
                            m[0][2] << ", " << m[0][3] << " ] [ " <<
                            m[1][0] << ", " << m[1][1] << ", " <<
                            m[1][2] << ", " << m[1][3] << " ] [ " <<
                            m[2][0] << ", " << m[2][1] << ", " <<
                            m[2][2] << ", " << m[2][3] << " ] [ " <<
                            m[3][0] << ", " << m[3][1] << ", " <<
                            m[3][2] << ", " << m[3][3] << " ] ]";
}

Matrix44 Translate(const Point3f &p);
Matrix44 Translate(const Vector3f &v);
Matrix44 Translate(Float x, Float y, Float z);
Matrix44 Scale(const Vector3f &v);
Matrix44 Scale(Float x, Float y, Float z);
Matrix44 RotateX(Float angle);
Matrix44 RotateY(Float angle);
Matrix44 RotateZ(Float angle);
Matrix44 Rotate(const Vector3f &axis, Float angle);
Matrix44 LookAt(const Point3f &pos, const Point3f &look, const Vector3f &up);
Matrix44 Perspective(Float fov, Float aspect, Float n, Float f);
Matrix44 Orthographic(Float l, Float r, Float b, Float t, Float n, Float f);

}
