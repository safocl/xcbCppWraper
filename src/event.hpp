#pragma once

#include "defines.hpp"

#include <xcb/damage.h>
#include <xcb/xproto.h>

namespace xcbwraper {

enum class EventType : u8 {
    eDamage    = XCB_DAMAGE_NOTIFY,
    eExpose    = XCB_EXPOSE,
    eCreate    = XCB_CREATE_NOTIFY,
    eMap       = XCB_MAP_NOTIFY,
    eUnmap     = XCB_UNMAP_NOTIFY,
    eDestroy   = XCB_DESTROY_NOTIFY,
    eConfigure = XCB_CONFIGURE_NOTIFY
};

class GenericEvent {
protected:
    EventType type { XCB_NONE };
    u8        pad0 {};
    u16       sequence {};
    u32       length {};

public:
    virtual ~GenericEvent() {};

    EventType getType() const;
};
}   // namespace xcbwraper
