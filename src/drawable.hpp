#pragma once

#include "defines.hpp"
#include "posix-shm.hpp"
#include "xcbconnection.hpp"
#include "drawablegeometry.hpp"

#include <cstddef>
#include <xcb/xproto.h>

#include <optional>

namespace xcbwraper {

class Drawable {
public:
    using DrawableId = xcb_drawable_t;
    struct CreateInfo {
        XcbConnection::Shared           connection;
        std::optional< xcb_drawable_t > xcbId;
    };

protected:
    XcbConnection::Shared mConnection;
    std::optional< xcb_drawable_t > mId;

public:
    Drawable( const CreateInfo & );
    virtual ~Drawable();

    operator xcb_drawable_t() const;

    void attachImageDataToShm( posix::SharedMemory::Shared ) const;

    DrawableGeometry getGeometry() const;

    DrawableId getXcbId() const;

    u32 getImageDataSize() const;
};
}   // namespace xcbwraper
