#include "pixmap.hpp"
#include "exception.hpp"
#include "posix-shm.hpp"
#include "xcbconnection.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <xcb/xcb.h>
#include <xcb/shm.h>
#include <xcb/xproto.h>
#include <xcb/composite.h>

namespace xcbwraper {
Pixmap::Pixmap( const Pixmap::CreateInfo & ci ) :
Drawable( Drawable::CreateInfo { .connection = ci.connection, .xcbId = ci.xcbId } ) {
}

Pixmap::~Pixmap() { xcb_free_pixmap( *mConnection, *this ); }

Pixmap::operator xcb_pixmap_t() const {
    return static_cast< xcb_pixmap_t >( mId.value() );
}


}   // namespace xcbwraper
