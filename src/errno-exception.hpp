#pragma once

#include <exception>
#include <string>
#include <string_view>

namespace xcbwraper {

class ErrnoException : public std::exception {
    std::string mWhatArg;

public:
    explicit ErrnoException( std::string_view whereAndWhatReturnErr, int errCode );
    virtual ~ErrnoException();
    const char * what() const noexcept override;
};
}


