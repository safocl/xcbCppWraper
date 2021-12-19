#pragma once

#include <cstdint>
#include <string>

#include <xcb/xproto.h>

#include "point.hpp"
//#include "xcbconnection.hpp"
#include "drawablegeometry.hpp"

namespace xcbwraper {
using XCBWindowClass = std::string;
using WindowIDType   = xcb_window_t;

class XcbWindowProp final {
    std::string          mClassName;
    DrawableGeometry::Info mDrawableGeometry;
    WindowIDType         mWindow;
    std::uint8_t         mWindowMapState;
    std::uint8_t         mBitPerRgb { 0 };

public:
    explicit XcbWindowProp( WindowIDType windowID );
    XcbWindowProp( XcbWindowProp && ) = default;
    ~XcbWindowProp();
    DrawableGeometry::Info getGeometry() const;
    XCBWindowClass       getClass() const;
    WindowIDType         getID() const;
    bool                 isViewable() const;
    std::uint8_t         getBitPerRGB() const;
};

//class XCBWindowID final {
//    WindowIDType windowID;
//
//public:
//    XCBWindowID( WindowIDType windowID ) : windowID { windowID } {};
//    XcbWindowProp params() const { return XcbWindowProp { windowID }; }
//};

}   // namespace xcbwraper
