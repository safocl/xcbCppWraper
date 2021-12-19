#pragma once

#include "defines.hpp"

namespace xcbwraper {
struct Point final {
    using CoordType = s16;
    CoordType x;
    CoordType y;

    bool operator>( Point other ) { return x > other.x && y > other.y; }
    bool operator<( Point other ) { return x < other.x && y < other.y; }
    bool operator>=( Point other ) {
        return ( x > other.x && y > other.y ) || ( x == other.x && y == other.y );
    }
    bool operator<=( Point other ) {
        return ( x < other.x && y < other.y ) || ( x == other.x && y == other.y );
    }
};
}
