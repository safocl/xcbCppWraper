#pragma once

#include "defines.hpp"

#include <vector>
#include <string>
#include <memory>

#include <xcb/xproto.h>


namespace xcbwraper {

inline constexpr u32 pocketSizeInBytes { 4 };

enum class ChangePropertyMode {
    eReplace = XCB_PROP_MODE_REPLACE,
    /**< Discard the previous property value and store the new data. */

    ePrepend = XCB_PROP_MODE_PREPEND,
    /**< Insert the new data before the beginning of existing data. The `format` must
match existing property value. If the property is undefined, it is treated as
defined with the correct type and format with zero-length data. */

    eAppend = XCB_PROP_MODE_APPEND
    /**< Insert the new data after the beginning of existing data. The `format` must
match existing property value. If the property is undefined, it is treated as
defined with the correct type and format with zero-length data. */
};

enum class PropertyFormat : u8 { e0 = 0, e8 = 8, e16 = 16, e32 = 32 };

enum class AtomTypes : u32 {
    eAny             = XCB_ATOM_ANY,
    ePrimary         = XCB_ATOM_PRIMARY,
    eSecondary       = XCB_ATOM_SECONDARY,
    eArc             = XCB_ATOM_ARC,
    eAtom            = XCB_ATOM_ATOM,
    eBitmap          = XCB_ATOM_BITMAP,
    eCardinal        = XCB_ATOM_CARDINAL,
    eColormap        = XCB_ATOM_COLORMAP,
    eCursor          = XCB_ATOM_CURSOR,
    eDrawable        = XCB_ATOM_DRAWABLE,
    eFont            = XCB_ATOM_FONT,
    eInteger         = XCB_ATOM_INTEGER,
    ePixmap          = XCB_ATOM_PIXMAP,
    ePoint           = XCB_ATOM_POINT,
    eRectangle       = XCB_ATOM_RECTANGLE,
    eResourceManager = XCB_ATOM_RESOURCE_MANAGER,
    eString          = XCB_ATOM_STRING,
    eVisualId        = XCB_ATOM_VISUALID,
    eWindow          = XCB_ATOM_WINDOW,
    eNotice          = XCB_ATOM_NOTICE
};

enum class Atoms : u32 {
    eWmCommand          = XCB_ATOM_WM_COMMAND,
    eWmHints            = XCB_ATOM_WM_HINTS,
    eWmClientMachine    = XCB_ATOM_WM_CLIENT_MACHINE,
    eWmIconName         = XCB_ATOM_WM_ICON_NAME,
    eWmIconSize         = XCB_ATOM_WM_ICON_SIZE,
    eWmName             = XCB_ATOM_WM_NAME,
    eWmNormalHints      = XCB_ATOM_WM_NORMAL_HINTS,
    eWmSizeHints        = XCB_ATOM_WM_SIZE_HINTS,
    eWmZoomHints        = XCB_ATOM_WM_ZOOM_HINTS,
    eBuffer0            = XCB_ATOM_CUT_BUFFER0,
    eBuffer1            = XCB_ATOM_CUT_BUFFER1,
    eBuffer2            = XCB_ATOM_CUT_BUFFER2,
    eBuffer3            = XCB_ATOM_CUT_BUFFER3,
    eBuffer4            = XCB_ATOM_CUT_BUFFER4,
    eBuffer5            = XCB_ATOM_CUT_BUFFER5,
    eBuffer6            = XCB_ATOM_CUT_BUFFER6,
    eBuffer7            = XCB_ATOM_CUT_BUFFER7,
    eRgbColorMap        = XCB_ATOM_RGB_COLOR_MAP,
    eRgbBestMap         = XCB_ATOM_RGB_BEST_MAP,
    eRgbBlueMap         = XCB_ATOM_RGB_BLUE_MAP,
    eRgbDefaultMap      = XCB_ATOM_RGB_DEFAULT_MAP,
    eRgbGrayMap         = XCB_ATOM_RGB_GRAY_MAP,
    eRgbGreenMap        = XCB_ATOM_RGB_GREEN_MAP,
    eRgbRedMap          = XCB_ATOM_RGB_RED_MAP,
    eMinSpace           = XCB_ATOM_MIN_SPACE,
    eNormSpace          = XCB_ATOM_NORM_SPACE,
    eMaxSpace           = XCB_ATOM_MAX_SPACE,
    eEndSpace           = XCB_ATOM_END_SPACE,
    eSuperScriptX       = XCB_ATOM_SUPERSCRIPT_X,
    eSuperScriptY       = XCB_ATOM_SUPERSCRIPT_Y,
    eSubscriptX         = XCB_ATOM_SUBSCRIPT_X,
    eSubscriptY         = XCB_ATOM_SUBSCRIPT_Y,
    eUnderlinePosition  = XCB_ATOM_UNDERLINE_POSITION,
    eUnderlineThickness = XCB_ATOM_UNDERLINE_THICKNESS,
    eStrikeoutAscent    = XCB_ATOM_STRIKEOUT_ASCENT,
    eStrikeoutDescent   = XCB_ATOM_STRIKEOUT_DESCENT,
    eItalicAngle        = XCB_ATOM_ITALIC_ANGLE,
    eXHeight            = XCB_ATOM_X_HEIGHT,
    eQuadWidth          = XCB_ATOM_QUAD_WIDTH,
    eWeight             = XCB_ATOM_WEIGHT,
    ePointSize          = XCB_ATOM_POINT_SIZE,
    eResolution         = XCB_ATOM_RESOLUTION,
    eCopyright          = XCB_ATOM_COPYRIGHT,
    eFontName           = XCB_ATOM_FONT_NAME,
    eFamilyName         = XCB_ATOM_FAMILY_NAME,
    eFullName           = XCB_ATOM_FULL_NAME,
    eCapHeight          = XCB_ATOM_CAP_HEIGHT,
    eWmClass            = XCB_ATOM_WM_CLASS,
    eWmTransientFor     = XCB_ATOM_WM_TRANSIENT_FOR
};

enum class ImageOrder {
    eLSBFirst = XCB_IMAGE_ORDER_LSB_FIRST,
    eMSBFirst = XCB_IMAGE_ORDER_MSB_FIRST
};

enum class ScanlineBits : u8 { e8 = 8, e16 = 16, e32 = 32 };

enum class BitsPerPixel : u8 {
    e1  = 1,
    e4  = 4,
    e8  = 8,
    e16 = 16,
    e24 = 24,
    e32 = 32
};

enum class VisualClass {
    eStaticGray  = XCB_VISUAL_CLASS_STATIC_GRAY,
    eStaticColor = XCB_VISUAL_CLASS_STATIC_COLOR,
    eTrueColor   = XCB_VISUAL_CLASS_TRUE_COLOR,
    eGrayScale   = XCB_VISUAL_CLASS_GRAY_SCALE,
    ePseudoColor = XCB_VISUAL_CLASS_STATIC_GRAY,
    eStaticGrey  = XCB_VISUAL_CLASS_STATIC_GRAY,
};

enum class BackingStore {
    eNever      = XCB_BACKING_STORE_NOT_USEFUL,
    eWhenMapped = XCB_BACKING_STORE_WHEN_MAPPED,
    eAlways     = XCB_BACKING_STORE_ALWAYS
};

struct ProtocolVersion final {
    u16 major;
    u16 minor;
};

struct ResourceId final {
    u32 base;
    u32 mask;
};

struct Scanline final {
    ScanlineBits unit;
    ScanlineBits pad;
};

struct Format final {
    u8           depth;
    BitsPerPixel bitsPerPixel;
    ScanlineBits scanlinePad;
};

using VisualId = xcb_visualid_t;

struct RgbMask final {
    u32 red;
    u32 green;
    u32 blue;
};

struct Visual final {
    VisualId    id;
    VisualClass visualClass;
    RgbMask     rgbMask;
    u8          bitsPerRgbValue;
    u16         colorMapEntries;
};

using ListOfVisuals = std::vector< Visual >;

struct Depth final {
    u8               depth;
    ListOfVisuals    visuals;
};

enum class EventMask {
    eNoEvent              = XCB_EVENT_MASK_NO_EVENT,
    eKeyPress             = XCB_EVENT_MASK_KEY_PRESS,
    eKeyRelease           = XCB_EVENT_MASK_KEY_RELEASE,
    eButtonPress          = XCB_EVENT_MASK_BUTTON_PRESS,
    eButtonRelease        = XCB_EVENT_MASK_BUTTON_RELEASE,
    eEnterWindow          = XCB_EVENT_MASK_ENTER_WINDOW,
    eLeaveWindow          = XCB_EVENT_MASK_LEAVE_WINDOW,
    ePointerMotion        = XCB_EVENT_MASK_POINTER_MOTION,
    ePointerMotionHint    = XCB_EVENT_MASK_POINTER_MOTION_HINT,
    eButton1Motion        = XCB_EVENT_MASK_BUTTON_1_MOTION,
    eButton2Motion        = XCB_EVENT_MASK_BUTTON_2_MOTION,
    eButton3Motion        = XCB_EVENT_MASK_BUTTON_3_MOTION,
    eButton4Motion        = XCB_EVENT_MASK_BUTTON_4_MOTION,
    eButton5Motion        = XCB_EVENT_MASK_BUTTON_5_MOTION,
    eButtonMotion         = XCB_EVENT_MASK_BUTTON_MOTION,
    eKeyMapState          = XCB_EVENT_MASK_KEYMAP_STATE,
    eExposure             = XCB_EVENT_MASK_EXPOSURE,
    eVisibilityChange     = XCB_EVENT_MASK_VISIBILITY_CHANGE,
    eStructureNotify      = XCB_EVENT_MASK_STRUCTURE_NOTIFY,
    eResizeRedirect       = XCB_EVENT_MASK_RESIZE_REDIRECT,
    eSubstructureNotify   = XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY,
    eSubstructureRedirect = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,
    eFocusChange          = XCB_EVENT_MASK_FOCUS_CHANGE,
    ePropertyChange       = XCB_EVENT_MASK_PROPERTY_CHANGE,
    eColorMapChange       = XCB_EVENT_MASK_COLOR_MAP_CHANGE,
    eOwnerGrabButton      = XCB_EVENT_MASK_OWNER_GRAB_BUTTON
};

class Window;

using ListOfDepths = std::vector< Depth >;
using ColorMap     = xcb_colormap_t;
using WindowShared = std::shared_ptr< Window >;

struct Screen final {
    WindowShared root;
    u16          widthInPixels;
    u16          heightInPixels;
    u16          widthInMillimeters;
    u16          heightInMillimeters;
    ListOfDepths allowedDepths;
    u8           rootDepth;
    VisualId     rootVisual;
    ColorMap     defaultColormap;
    u32          whitePixel;
    u32          blackPixel;
    u16          minInstalledMaps;
    u16          maxInstalledMaps;
    BackingStore backingStores;
    bool         saveUnders;
    EventMask    currentInputMasks;
};

using ListOfFormat = std::vector< Format >;
using ListOfScreen = std::vector< Screen >;
using Keycode      = xcb_keycode_t;

struct SetupInfo final {
    ProtocolVersion protocolVersion;
    std::string     vendor;
    u32             releaseNumber;
    ResourceId      resourceId;
    ImageOrder      imageImageOrder;
    Scanline        bitmapScanline;
    ImageOrder      bitmapBitOrder;
    ListOfFormat    pixmapFormats;
    ListOfScreen    roots;
    u32             motionBufferSize;
    u16             maximumRequestLength;
    Keycode         minKeycode;
    Keycode         maxKeycode;
};
}
