#include "exception.hpp"

namespace xcbwraper {

Exception::Exception( std::string_view where, u8 errorCode ) :
mWhere( where ), mErrorCode( errorCode ) {
    std::string strErrorCode {};

#define CASE_MACRO( errorName, dstString ) \
    case XCB_##errorName: dstString = #errorName ""; break;

    switch ( mErrorCode ) {
        CASE_MACRO( ACCESS, strErrorCode )
        CASE_MACRO( ALLOC, strErrorCode )
        CASE_MACRO( ATOM, strErrorCode )
        CASE_MACRO( COLORMAP, strErrorCode )
        CASE_MACRO( CURSOR, strErrorCode )
        CASE_MACRO( DRAWABLE, strErrorCode )
        CASE_MACRO( FONT, strErrorCode )
        CASE_MACRO( G_CONTEXT, strErrorCode )
        CASE_MACRO( ID_CHOICE, strErrorCode )
        CASE_MACRO( IMPLEMENTATION, strErrorCode )
        CASE_MACRO( LENGTH, strErrorCode )
        CASE_MACRO( MATCH, strErrorCode )
        CASE_MACRO( NAME, strErrorCode )
        CASE_MACRO( PIXMAP, strErrorCode )
        CASE_MACRO( REQUEST, strErrorCode )
        CASE_MACRO( VALUE, strErrorCode )
        CASE_MACRO( WINDOW, strErrorCode )
    }

#undef CASE_MACRO

    mWhere += " : error " + strErrorCode + " with code " +
              std::to_string( static_cast< int >( mErrorCode ) );
}

const char * Exception::what() const noexcept { return mWhere.c_str(); }
}
