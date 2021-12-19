#pragma once

#include "defines.hpp"
#include "point.hpp"
#include "types.hpp"

namespace xcbwraper {

class DrawableGeometry final {
public:
    struct CreateInfo final {
        Point        leftTopPoint {};
        u16          width {};
        u16          height {};
        u16          borderWidth {};
        BitsPerPixel depth {};
    };

    struct Info final {
        Point        leftTopPoint {};
        Point    rightTopPoint{};
        Point    leftBotPoint{};
        Point    rightBotPoint{};
        u16          width {};
        u16          height {};
        u16          borderWidth {};
        BitsPerPixel depth {};
    };

    explicit DrawableGeometry( CreateInfo );

    Info getInfo() const;

    void reinit( CreateInfo );

private:
    void compute();

private:
    Info mDrawableInfo;
};
}   // namespace xcbwraper
