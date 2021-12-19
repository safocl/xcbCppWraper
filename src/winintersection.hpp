#pragma once

#include <cstdint>

#include "point.hpp"
#include "drawablegeometry.hpp"

namespace xcbwraper {
struct Intersection final {
    Point    leftTopPoint { 0, 0 };
    uint16_t width { 0 };
    uint16_t height { 0 };
    bool     isExist { false };
             operator bool() const { return isExist; }
};

Intersection intersect( DrawableGeometry::Info windowOne,
                        DrawableGeometry::Info windowTwo );
}   // namespace xcbwraper
