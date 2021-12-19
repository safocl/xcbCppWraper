#include "window.hpp"
#include "drawable.hpp"
#include "errno-exception.hpp"
#include "exception.hpp"
#include "posix-shm.hpp"
#include "types.hpp"
#include "xcbconnection.hpp"
#include "xcbwindowprop.hpp"
#include "extension_query_version.hpp"
#include "defines.hpp"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <exception>
#include <optional>
#include <cmath>

#include <xcb/composite.h>
#include <xcb/shape.h>
#include <xcb/xcb.h>
#include <xcb/xfixes.h>
#include <xcb/xproto.h>
#include <xcb/shm.h>
#include <xcb/dri3.h>
#include <xcb/damage.h>

namespace xcbwraper {

Window::Window( const Window::CreateInfo & ci ) :
Drawable( Drawable::CreateInfo { .connection = ci.connection, .xcbId = ci.xcbId } ),
mStrategy( ci.destroyStrategy ) {}

Window::~Window() {
    if ( *this ) {
        switch ( mStrategy ) {
        case DestroyStrategy::eClose:
            xcb_destroy_window( *mConnection, *this );
            xcb_flush( *mConnection );
            break;
        case DestroyStrategy::eDetach: detach(); break;
    }
    }
}

Window & Window::operator=( xcb_window_t window ) {
    mId.value() = window;
    return *this;
}

Window::operator xcb_window_t() const {
    return static_cast< xcb_window_t >( mId.value() );
}

Window::operator bool() const { return mId && mId != XCB_NONE; }

Window::GetPropertyInfo
Window::getProperty( Atoms atom, AtomTypes atomType, bool deleteProperty ) const {
    u32 longOffset {};
    u32 longLength { 1024 };
    u32 remaining {};

    std::vector< u8 > data;
    data.reserve( longLength * pocketSizeInBytes );

    u8         format {};
    xcb_atom_t type {};

    do {
        auto coockie = xcb_get_property( *mConnection,
                                         deleteProperty,
                                         *this,
                                         static_cast< xcb_atom_t >( atom ),
                                         static_cast< xcb_atom_t >( atomType ),
                                         longOffset,
                                         longLength );

        xcb_generic_error_t *                       err;
        std::unique_ptr< xcb_get_property_reply_t > reply { xcb_get_property_reply(
        *mConnection, coockie, &err ) };

        if ( !reply ) {
            if ( err ) {
                std::unique_ptr< xcb_generic_error_t > uniqueErr { err };
                throw Exception(
                "In [Window::GetPropertyInfo Window::getProperty( Atoms atom, AtomTypes atomType, bool deleteProperty ) const]",
                uniqueErr->error_code );
            } else
                throw std::runtime_error(
                "In [Window::GetPropertyInfo Window::getProperty( Atoms a      tom, AtomTypes atomType, bool deleteProperty ) const]: Unknow error" );
        }

        format = reply->format;
        type   = reply->type;

        const auto readLength = xcb_get_property_value_length( reply.get() );

        data.resize( ( longOffset + readLength ) * pocketSizeInBytes );

        std::memcpy( data.data() + longOffset * pocketSizeInBytes,
                     xcb_get_property_value( reply.get() ),
                     readLength * pocketSizeInBytes );

        longOffset += readLength;

        remaining = reply->bytes_after;
    } while ( remaining );

    return GetPropertyInfo { .type   = static_cast< AtomTypes >( type ),
                             .format = static_cast< PropertyFormat >( format ),
                             .value  = std::move( data ) };
}

void Window::changeProperty( Atoms              atomName,
                             AtomTypes          atomType,
                             PropertyFormat     fmt,
                             ChangePropertyMode mode,
                             ChangePropertyData data ) {
    u32 dataSize {};

    switch ( fmt ) {
    case PropertyFormat::e8: dataSize = data.size(); break;
    case PropertyFormat::e16:
        if ( data.size() / 2 )
            throw std::runtime_error(
            "in [void Window::changeProperty(...)]: incompatible data format" );
        dataSize = data.size() / 2;
        break;
    case PropertyFormat::e32:
        if ( data.size() / 4 )
            throw std::runtime_error(
            "in [void Window::changeProperty(...)]: incompatible data format" );
        dataSize = data.size() / 4;
        break;
    default:
        throw std::runtime_error(
        "in [void Window::changeProperty(...)]: incompatible data format" );
    }

    xcb_change_property( *mConnection,
                         static_cast< u8 >( mode ),
                         *this,
                         static_cast< xcb_atom_t >( atomName ),
                         static_cast< xcb_atom_t >( atomType ),
                         static_cast< u8 >( fmt ),
                         dataSize,
                         data.data() );
}

void Window::deleteProperty( Atoms property ) {
    xcb_delete_property(
    *mConnection, *this, static_cast< xcb_atom_t >( property ) );
}

std::vector< Atoms > Window::listProperties() const {
    auto coockie = xcb_list_properties( *mConnection, *this );

    std::unique_ptr< xcb_list_properties_reply_t > reply { xcb_list_properties_reply(
    *mConnection, coockie, nullptr ) };

    auto atomsLength = xcb_list_properties_atoms_length( reply.get() );
    std::vector< Atoms > listProps( atomsLength );

    std::memcpy( listProps.data(),
                 xcb_list_properties_atoms( reply.get() ),
                 atomsLength * sizeof( Atoms ) );

    return listProps;
}

Window::QueryTreeInfo Window::queryTree() const {
    auto coockie = xcb_query_tree( *mConnection, *this );

    xcb_generic_error_t *                          err;
    std::unique_ptr< xcb_query_tree_reply_t >      reply { xcb_query_tree_reply(
    *mConnection, coockie, &err ) };
    if ( !reply ) {
        if ( err )
            throw Exception( "In [QueryTreeInfo Window::queryTree() const]",
                             err->error_code );
        else
            throw std::runtime_error(
            "In [QueryTreeInfo Window::queryTree() const]: Unknow error" );
    }

    std::vector< WindowShared > children {};

    std::span< xcb_window_t > childrenList(
    xcb_query_tree_children( reply.get() ),
    xcb_query_tree_children_length( reply.get() ) );

    for ( std::span< xcb_window_t >::size_type i {}; i < childrenList.size(); ++i ) {
        Window::CreateInfo ci { .connection = mConnection,
                                .xcbId      = childrenList[ i ] };
        children.emplace_back( ci );
    }

    return QueryTreeInfo { .root     = std::make_shared< Window >( CreateInfo {
                           .connection = mConnection, .xcbId = reply->root } ),
                           .parent   = std::make_shared< Window >( CreateInfo {
                           .connection = mConnection, .xcbId = reply->parent } ),
                           .children = std::move( children ) };
}

void Window::redirect() {
    if ( mId.value() == XCB_NONE )
        throw std::runtime_error( "Using Window::release() for null window id" );

    xcb_composite_redirect_window(
    *mConnection, *this, XCB_COMPOSITE_REDIRECT_MANUAL );

    //xcb_flush( *mConnection );
}

void Window::redirectSubwindow() {
    if ( mId.value() == XCB_NONE )
        throw std::runtime_error( "Using Window::release() for null window id" );

    xcb_composite_redirect_subwindows(
    *mConnection, *this, XCB_COMPOSITE_REDIRECT_MANUAL );

    //xcb_flush( *mConnection );
}

void Window::unredirect() {
    if ( mId.value() == XCB_NONE )
        throw std::runtime_error( "Using Window::release() for null window id" );

    xcb_composite_unredirect_window(
    *mConnection, *this, XCB_COMPOSITE_REDIRECT_MANUAL );

    xcb_flush( *mConnection );
}

void Window::unredirectSubwindow() {
    if ( mId.value() == XCB_NONE )
        throw std::runtime_error( "Using Window::release() for null window id" );

    xcb_composite_unredirect_subwindows(
    *mConnection, *this, XCB_COMPOSITE_REDIRECT_MANUAL );

    xcb_flush( *mConnection );
}

void Window::release() {
    if ( mId.value() == XCB_NONE )
        throw std::runtime_error( "Using Window::release() for null window id" );

    xcb_destroy_window( *mConnection, mId.value() );
    mId.value() = XCB_NONE;
    xcb_flush( *mConnection );
}

void Window::map() {
    if ( mId.value() == XCB_NONE )
        throw std::runtime_error( "Using Window::map() for null window id" );

    if ( !isViewable() )
        xcb_map_window( *mConnection, mId.value() );
    xcb_flush( *mConnection );
}

void Window::unmap() {
    if ( mId.value() == XCB_NONE )
        throw std::runtime_error( "Using Window::unmap() for null window id" );

    if ( isViewable() )
        xcb_unmap_window( *mConnection, mId.value() );
    xcb_flush( *mConnection );
}

void Window::fullDamaged() {
    if ( mId.value() == XCB_NONE )
        throw std::runtime_error( "Using Window::fullDamaged() for null window id" );

    xcb_xfixes_region_t region = xcb_generate_id( *mConnection );

    xcb_xfixes_create_region_from_window(
    *mConnection, region, *this, XCB_SHAPE_SK_BOUNDING );

    xcb_damage_damage_t damage = xcb_generate_id( *mConnection );

    xcb_damage_create(
    *mConnection, damage, *this, XCB_DAMAGE_REPORT_LEVEL_BOUNDING_BOX );

    xcb_damage_add( *mConnection, *this, region );

    xcb_damage_destroy( *mConnection, damage );

    xcb_xfixes_destroy_region( *mConnection, region );

    xcb_flush( *mConnection );
}

void Window::attachOffscreenImageToPixmap( const Pixmap & destination ) {
    xcb_composite_name_window_pixmap( *mConnection, *this, destination );
}

void Window::detach() { mId.value() = XCB_NONE; }

bool Window::isViewable() const {
    std::unique_ptr< xcb_get_window_attributes_reply_t > windowAttrs(
    xcb_get_window_attributes_reply(
    *mConnection,
    xcb_get_window_attributes( *mConnection, mId.value() ),
    nullptr ) );

    return windowAttrs->map_state == XCB_MAP_STATE_VIEWABLE;
}

CompositeWindow::CompositeWindow( CompositeWindow::CreateInfo ci ) :
CompositeWindow::Window( ci ) {}

CompositeWindow::~CompositeWindow() = default;

void CompositeWindow::release() {
    if ( mId.value() != XCB_NONE )
        xcb_composite_release_overlay_window( *mConnection, mId.value() );
    mId.value() = XCB_NONE;
    xcb_flush( *mConnection );
}

void CompositeWindow::redirectSubwindow() {}

void CompositeWindow::unredirectSubwindow() {}
}   // namespace xcbwraper
