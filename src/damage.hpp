#pragma once

#include "defines.hpp"
#include "event.hpp"

#include <xcb/damage.h>
#include <xcb/xproto.h>

namespace xcbwraper {

class DamageNotifyEvent final : public GenericEvent {
public:
    enum class ReportLevel : u32 {
        eRawRectangles   = XCB_DAMAGE_REPORT_LEVEL_RAW_RECTANGLES,
        eDeltaRectangles = XCB_DAMAGE_REPORT_LEVEL_DELTA_RECTANGLES,
        eBoundingBox     = XCB_DAMAGE_REPORT_LEVEL_BOUNDING_BOX,
        eNonEmpty        = XCB_DAMAGE_REPORT_LEVEL_NON_EMPTY
    };

private:
    ReportLevel level;

public:
    DamageNotifyEvent();
};

}   // namespace xcbwraper
