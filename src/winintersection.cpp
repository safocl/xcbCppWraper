#include "winintersection.hpp"
#include "drawablegeometry.hpp"

#include <algorithm>

namespace xcbwraper {
Intersection intersect( DrawableGeometry::Info windowOne,
                        DrawableGeometry::Info windowTwo ) {
    int16_t top    = std::max( windowOne.leftTopPoint.y, windowTwo.leftTopPoint.y );
    int16_t bottom = std::min( windowOne.rightBotPoint.y, windowTwo.rightBotPoint.y );
    int16_t left   = std::max( windowOne.leftTopPoint.x, windowTwo.leftTopPoint.x );
    int16_t right  = std::min( windowOne.rightBotPoint.x, windowTwo.rightBotPoint.x );

    int16_t width  = right - left;
    int16_t height = bottom - top;

    if ( width < 0 || height < 0 )
        return Intersection {};

    return Intersection { { left, top },
                          static_cast< uint16_t >( width ),
                          static_cast< uint16_t >( height ),
                          true };
}
}   // namespace xcbwraper
