#pragma once

#include "defines.h"
#include "Matrix44.h"

namespace toy_renderer {
    
// Transform
class Transform {
  public:
    Transform(const Matrix44 &m) : m(m), mInv(Inverse(m)) {}
    Transform(const Matrix44 &m, const Matrix44 &mInv) : m(m), mInv(mInv) {}
    Transform(const Float mat[0][0]) {
        m = Matrix44(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                     mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                     mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                     mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
        mInv = Inverse(m);
    }

    bool operator==(const Transform &t) const {
        return m == t.m;
    }
    bool operator!=(const Transform &t) const {
        return m != t.m;
    }

    friend Transform Inverse(const Transform &t) {
        return Transform(t.mInv, t.m);
    }
    friend Transform Transpose(const Transform &t) {
        return Transform(Transpose(t.m), Transpose(t.mInv));
    }

    template <typename T>
    Vector3<T> operator()(const Vector3<T> &v) const;
    template <typename T>
    Point3<T> operator()(const Point3<T> &v) const;
    template <typename T>
    Normal3<T> operator()(const Normal3<T> &v) const;

  private:
    Matrix44 m, mInv;
};

}
