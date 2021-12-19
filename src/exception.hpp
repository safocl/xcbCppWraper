#pragma once

#include "defines.hpp"
#include <exception>
#include <string>
#include <string_view>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

namespace xcbwraper {

class Exception : public std::exception {
    std::string      mWhere;
    u8               mErrorCode;

public:
    Exception( std::string_view where, u8 errorCode );

    virtual const char * what() const noexcept override;
};

}   // namespace xcbwraper
