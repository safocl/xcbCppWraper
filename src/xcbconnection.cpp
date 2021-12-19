#include "xcbconnection.hpp"
#include "types.hpp"
#include "event.hpp"

#include <xcb/xcb.h>

namespace xcbwraper {

XcbConnection::XcbConnection() : mConnection( xcb_connect( nullptr, nullptr ) ) {}
XcbConnection::~XcbConnection() { xcb_disconnect( *this ); }

XcbConnection::operator xcb_connection_t *() { return mConnection; }

void XcbConnection::flush() { xcb_flush( *this ); }

}   // namespace xcbwraper
