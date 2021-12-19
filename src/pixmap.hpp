#pragma once

#include "defines.hpp"
#include "posix-shm.hpp"
#include "xcbconnection.hpp"
#include "drawable.hpp"

#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include <cstddef>
#include <optional>

namespace xcbwraper {

class Pixmap final : public Drawable {

public:
    struct CreateInfo {
        XcbConnection::Shared           connection;
        std::optional< xcb_drawable_t > xcbId;
    };

    explicit Pixmap( const CreateInfo & );
    ~Pixmap();

    operator xcb_pixmap_t() const;

};
}   // namespace xcbwraper
