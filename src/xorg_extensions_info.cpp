#include "xorg_extensions_info.hpp"

#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include <memory>
#include <iostream>

namespace xcbwraper {

XorgExtensionsInfo::XorgExtensionsInfo( const CreateInfo & ci ) :
mConnection( ci.connection ) {
    auto listExtensionsCoockie = xcb_list_extensions( *mConnection );

    std::unique_ptr< xcb_list_extensions_reply_t > listExtensionsReply {
        xcb_list_extensions_reply( *mConnection, listExtensionsCoockie, nullptr )
    };

    auto extensionNamesIterator =
    xcb_list_extensions_names_iterator( listExtensionsReply.get() );

    auto extensionNamesIteratorEnd = xcb_str_end( extensionNamesIterator );

    for ( ; extensionNamesIterator.data != extensionNamesIteratorEnd.data;
          xcb_str_next( &extensionNamesIterator ) ) {
        mExtensionsNamesVec.emplace_back(
        xcb_str_name( extensionNamesIterator.data ),
        xcb_str_name_length( extensionNamesIterator.data ) );
    }
}

void XorgExtensionsInfo::print() const {
    std::cout << std::endl
              << "************** Avaible Xorg extensions **************"
              << std::endl;

    for ( auto extName : mExtensionsNamesVec )
        std::cout << "\t[ " << extName << " ]" << std::endl;

    std::cout << "*****************************************************" << std::endl
              << std::endl;
}

}   // namespace xcbwraper
