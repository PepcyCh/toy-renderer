#pragma once

#include "defines.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Point2.h"
#include "Point3.h"
#include "Normal3.h"
#include "Matrix44.h"

namespace toy_renderer {
    
const Float PI = 3.141592653589793238463;
const Float PI_INV = 0.3183098861837907;

inline Float Radiance(Float degree) {
    return degree / 180 * PI;
}

inline Float Lerp(Float a, Float b, Float t) {
    return a + (b - a) * t;
}

}
