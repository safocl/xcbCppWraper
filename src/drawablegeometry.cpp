#include "drawablegeometry.hpp"

namespace xcbwraper {

DrawableGeometry::DrawableGeometry( DrawableGeometry::CreateInfo ci ) :
mDrawableInfo( Info { .leftTopPoint = ci.leftTopPoint,
                      .width        = ci.width,
                      .height       = ci.height,
                      .borderWidth  = ci.borderWidth,
                      .depth        = ci.depth } ) {
    compute();
}

DrawableGeometry::Info DrawableGeometry::getInfo() const { return mDrawableInfo; }

void DrawableGeometry::reinit( CreateInfo ci ) {
    mDrawableInfo.leftTopPoint = ci.leftTopPoint;
    mDrawableInfo.width        = ci.width;
    mDrawableInfo.height       = ci.height;
    mDrawableInfo.borderWidth  = ci.borderWidth;

    compute();
}

void DrawableGeometry::compute() {
    mDrawableInfo.rightBotPoint =
    Point { static_cast< Point::CoordType >( mDrawableInfo.leftTopPoint.x +
                                             mDrawableInfo.width ),
            static_cast< Point::CoordType >( mDrawableInfo.leftTopPoint.y +
                                             mDrawableInfo.height ) };

    mDrawableInfo.rightTopPoint =
    Point { static_cast< Point::CoordType >( mDrawableInfo.leftTopPoint.x +
                                             mDrawableInfo.width ),
            static_cast< Point::CoordType >( mDrawableInfo.leftTopPoint.y ) };

    mDrawableInfo.leftBotPoint =
    Point { static_cast< Point::CoordType >( mDrawableInfo.leftTopPoint.x ),
            static_cast< Point::CoordType >( mDrawableInfo.leftTopPoint.y +
                                             mDrawableInfo.height ) };
}

}   // namespace xcbwraper
