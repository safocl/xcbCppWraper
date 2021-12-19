#pragma once

#include "xcbconnection.hpp"

#include <string>
#include <vector>

namespace xcbwraper {
class XorgExtensionsInfo final {
public:
    struct CreateInfo final {
        XcbConnection::Shared connection;
    };

private:
    XcbConnection::Shared      mConnection;
    std::vector< std::string > mExtensionsNamesVec;

public:
    XorgExtensionsInfo( const CreateInfo & );

    void print() const;
    bool matchPresent( std::string_view name ) const;
};
}   // namespace xcbwraper
