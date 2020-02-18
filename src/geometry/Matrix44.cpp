#include "Matrix44.h"

#include <cmath>
#include <limits>

namespace toy_renderer {
    
Matrix44 Mul(const Matrix44 &m1, const Matrix44 &m2) {
    Matrix44 res;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            res[i][j] = m1[i][0] * m2[0][j] +
                        m1[i][1] * m2[1][j] +
                        m1[i][2] * m2[2][j] +
                        m1[i][3] * m2[3][j];
    return res;
}

Matrix44 Transpose(const Matrix44 &m) {
    return Matrix44(m[0][0], m[1][0], m[2][0], m[3][0],
                    m[0][1], m[1][1], m[2][1], m[3][1],
                    m[0][2], m[1][2], m[2][2], m[3][2],
                    m[0][3], m[1][3], m[2][3], m[3][3]);
}

Matrix44 Inverse(const Matrix44 &m) {
    Matrix44 res;
    Matrix44 tmp = m;
    for (int i = 0; i < 4; i++) {
        int pivot = i;
        Float max = std::abs(tmp[i][i]);
        for (int j = i + 1; j < 4; j++) {
            Float val = std::abs(tmp[j][i]);
            if (val > max) {
                max = val;
                pivot = j;
            }
        }

        if (i != pivot) {
            for (int j = 0; j < 4; j++) {
                std::swap(tmp[i][j], tmp[pivot][j]);
                std::swap(res[i][j], res[pivot][j]);
            }
        }

        if (max < std::numeric_limits<Float>::epsilon()) return m;
        Float pivinv = Float(1) / max;
        for (int j = 0; j < 4; j++) if (i != j) {
            Float f = tmp[j][i] * pivinv;
            for (int k = 0; k < 4; k++) {
                tmp[j][k] -= f * tmp[i][k];
                res[j][k] -= f * res[i][k];
            }
        }

        for (int j = 0; j < 4; j++) {
            tmp[i][j] *= pivinv;
            res[i][j] *= pivinv;
        }
    }

    return res;
}

Matrix44 Translate(const Point3f &p) {
    return Matrix44(1, 0, 0, p.x,
                    0, 1, 0, p.y,
                    0, 0, 1, p.z,
                    0, 0, 0, 1);
}
Matrix44 Translate(const Vector3f &v) {
    return Matrix44(1, 0, 0, v.x,
                    0, 1, 0, v.y,
                    0, 0, 1, v.z,
                    0, 0, 0, 1);
}
Matrix44 Translate(Float x, Float y, Float z) {
    return Matrix44(1, 0, 0, x,
                    0, 1, 0, y,
                    0, 0, 1, z,
                    0, 0, 0, 1);
}

Matrix44 Scale(const Vector3f &v) {
    return Matrix44(v.x, 0, 0, 0,
                    0, v.y, 0, 0,
                    0, 0, v.z, 0,
                    0, 0, 0, 1);
}
Matrix44 Scale(Float x, Float y, Float z) {
    return Matrix44(x, 0, 0, 0,
                    0, y, 0, 0,
                    0, 0, z, 0,
                    0, 0, 0, 1);
}

Matrix44 RotateX(Float angle) {
   Float c = std::cos(angle), s = std::sin(angle);
   return Matrix44(1, 0,  0, 0,
                   0, c, -s, 0,
                   0, s,  c, 0,
                   0, 0,  0, 1);
}
Matrix44 RotateY(Float angle) {
    Float c = std::cos(angle), s = std::sin(angle);
    return Matrix44( c, 0, s, 0,
                     0, 1, 0, 0,
                    -s, 0, c, 0,
                     0, 0, 0, 1);
}
Matrix44 RotateZ(Float angle) {
    Float c = std::cos(angle), s = std::sin(angle);
    return Matrix44(c, -s, 0, 0,
                    s,  c, 0, 0,
                    0,  0, 1, 0,
                    0,  0, 0, 1);
}
Matrix44 Rotate(const Vector3f &axis, Float angle) {
    Vector3f a = Normalize(axis);
    Float c = std::cos(angle), s = std::sin(angle), mc = 1 - c;
    return Matrix44(c + a[0] * a[0] * mc, a[0] * a[1] * mc - a[2] * s,
                        a[2] * a[0] * mc + a[1] * s, 0,
                    a[0] * a[1] * mc + a[2] * s, c + a[1] * a[1] * mc,
                        a[1] * a[2] * mc - a[0] * s, 0,
                    a[2] * a[0] * mc - a[1] * s, a[1] * a[2] * mc + a[0] * s,
                        c + a[2] * a[2] * mc, 0,
                    0, 0, 0, 1);
}

Matrix44 LookAt(const Point3f &pos, const Point3f &look, const Vector3f &up) {
    Vector3f w = Normalize(pos - look);
    Vector3f u = Normalize(Cross(up, w));
    Vector3f v = Cross(w, u);
    Vector3f eye(pos);
    return Matrix44(u[0], u[1], u[2], -Dot(u, eye),
                    v[0], v[1], v[2], -Dot(v, eye),
                    w[0], w[1], w[2], -Dot(w, eye),
                    0, 0, 0, 1);
}

Matrix44 Perspective(Float fov, Float aspect, Float n, Float f) {
    Float t = std::tan(fov * 0.5), invz = 1 / (f - n);
    return Matrix44(1 / (aspect * t), 0, 0, 0,
                    0, 1 / t, 0, 0,
                    0, 0, -(f + n) * invz, -2 * f * n * invz,
                    0, 0, -1, 0);
}

Matrix44 Orthographic(Float l, Float r, Float b, Float t, Float n, Float f) {
    Float invw = 1 / (r - l);
    Float invh = 1 / (t - b);
    Float invd = 1 / (f - n);
    return Matrix44(2 * invw, 0,  0, -(l + r) * invw,
                    0, 2 * invh,  0, -(b + t) * invh,
                    0, 0, -2 * invd, -(n + f) * invd,
                    0, 0, 0, 1);
}

}
