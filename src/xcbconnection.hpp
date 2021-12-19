#pragma once

#include "defines.hpp"
//#include "event.hpp"
//#include "types.hpp"

#include <string>
#include <xcb/xcb.h>

#include <memory>

namespace xcbwraper {

struct SetupInfo;
class GenericEvent;

class XcbConnection final {
public:
    using Shared = std::shared_ptr< XcbConnection >;

private:
    xcb_connection_t * mConnection;

public:
    XcbConnection();
    ~XcbConnection();

    operator xcb_connection_t *();

    void flush();

    SetupInfo getSetup() const;

    u32 generateId() const;

    GenericEvent waitForEvent() const;
    GenericEvent pollForEvent() const;
};

}   // namespace xcbwraper
