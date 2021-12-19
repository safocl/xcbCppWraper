#include "errno-exception.hpp"

#include <cstring>

namespace xcbwraper {

ErrnoException::ErrnoException( std::string_view where, int errCode ) :
mWhatArg( where ) {
    mWhatArg += " return error { ";
    mWhatArg += std::strerror( errCode );
    mWhatArg += " }";
}

ErrnoException::~ErrnoException() = default;

const char * ErrnoException::what() const noexcept { return mWhatArg.c_str(); }

}   // namespace xcbwraper
