#include <iostream>
#include <memory>
#include <stdexcept>

#include "drawablegeometry.hpp"
#include "xcbwindowprop.hpp"
#include "xcbconnection.hpp"

#include <xcb/xproto.h>

namespace xcbwraper {
XcbWindowProp::XcbWindowProp( WindowIDType windowID ) : mWindow( windowID ) {
    XcbConnection connection {};

    std::unique_ptr< xcb_get_geometry_reply_t > geometryRep { xcb_get_geometry_reply(
    connection, xcb_get_geometry( connection, windowID ), nullptr ) };

    std::unique_ptr< xcb_query_tree_reply_t > tree { xcb_query_tree_reply(
    connection, xcb_query_tree( connection, windowID ), nullptr ) };

    auto screen = xcb_setup_roots_iterator( xcb_get_setup( connection ) ).data;

    std::unique_ptr< xcb_translate_coordinates_reply_t > trans {
        xcb_translate_coordinates_reply(
        connection,
        xcb_translate_coordinates(
        connection, windowID, screen->root, geometryRep->x, geometryRep->y ),
        nullptr )
    };

    DrawableGeometry::CreateInfo wgci { .leftTopPoint = Point { .x = trans->dst_x,
                                                                .y = trans->dst_y },
                                        .width        = geometryRep->width,
                                        .height       = geometryRep->height,
                                        .borderWidth  = geometryRep->border_width };
    mDrawableGeometry = DrawableGeometry( wgci ).getInfo();

    std::unique_ptr< xcb_get_property_reply_t > nameReply { xcb_get_property_reply(
    connection,
    xcb_get_property(
    connection, false, windowID, XCB_ATOM_WM_CLASS, XCB_ATOM_STRING, 0, 3 ),
    nullptr ) };
    mClassName = static_cast< char * >( xcb_get_property_value( nameReply.get() ) );

    std::unique_ptr< xcb_get_window_attributes_reply_t > windowAttrs(
    xcb_get_window_attributes_reply(
    connection, xcb_get_window_attributes( connection, mWindow ), nullptr ) );
    mWindowMapState = windowAttrs->map_state;

    auto depthIter = std::make_unique< xcb_depth_iterator_t >(
    xcb_screen_allowed_depths_iterator( screen ) );

    for ( ; depthIter->rem; xcb_depth_next( depthIter.get() ) ) {
        auto visualIter = std::make_unique< xcb_visualtype_iterator_t >(
        xcb_depth_visuals_iterator( depthIter->data ) );
        for ( ; visualIter->rem; xcb_visualtype_next( visualIter.get() ) )
            if ( windowAttrs->visual == visualIter->data->visual_id ) {
                mBitPerRgb = visualIter->data->bits_per_rgb_value;
                break;
            }
    }
}

XcbWindowProp::~XcbWindowProp() = default;

DrawableGeometry::Info XcbWindowProp::getGeometry() const {
    return mDrawableGeometry;
}

std::string XcbWindowProp::getClass() const { return mClassName; }

WindowIDType XcbWindowProp::getID() const { return mWindow; }

bool XcbWindowProp::isViewable() const {
    return mWindowMapState & XCB_MAP_STATE_VIEWABLE;
}

std::uint8_t XcbWindowProp::getBitPerRGB() const {
    if ( mBitPerRgb == 0 )
        throw std::runtime_error(
        "In XcbWindowProp::getBitPerRGB() mBitPerRgb is null" );
    return mBitPerRgb;
}

}   // namespace xcbwraper
