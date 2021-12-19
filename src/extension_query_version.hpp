#pragma once

#include <iostream>
#include <xcb/dri3.h>
#include <xcb/damage.h>
#include <xcb/composite.h>
#include <xcb/present.h>
#include <xcb/shm.h>
#include <xcb/shape.h>
#include <xcb/xfixes.h>
#include <xcb/xcb.h>

#include <memory>

namespace xcbwraper {
#define CHECK_QUERY_VERSION( EXTENSION )                                          \
    inline void EXTENSION##CheckQueryVersion(                                     \
    xcb_connection_t * connection, int needMajorVersion, int needMinorVersion ) { \
        auto queryCoockie = xcb_##EXTENSION##_query_version(            \
        connection, needMajorVersion, needMinorVersion );                         \
                                                                                  \
        std::unique_ptr< xcb_##EXTENSION##_query_version_reply_t >                \
        queryReply { xcb_##EXTENSION##_query_version_reply(            \
        connection, queryCoockie, nullptr ) };                          \
                                                                                  \
        if ( !queryReply ) {                                                      \
            throw std::runtime_error(                                             \
            #EXTENSION " query version request failed " );         \
        }                                                                         \
    }

CHECK_QUERY_VERSION( composite )
CHECK_QUERY_VERSION( damage )
CHECK_QUERY_VERSION( dri3 )
CHECK_QUERY_VERSION( present )
CHECK_QUERY_VERSION( xfixes )

//inline void dri3CheckQueryVersion( xcb_connection_t * connection,
//                                   int                needMajorVersion,
//                                   int                needMinorVersion ) {
//    auto queryCookie =
//    xcb_dri3_query_version( connection, needMajorVersion, needMinorVersion );
//
//    std::unique_ptr< xcb_dri3_query_version_reply_t > queryReply {
//        xcb_dri3_query_version_reply( connection, queryCookie, nullptr )
//    };
//
//    if ( !queryReply ) {
//        throw std::runtime_error(
//        "dri3 query version request failed " );
//    }
//
//    std::cout << "dri3 version is " << queryReply->major_version << "."
//              << queryReply->minor_version << std::endl;
//}

inline void shapeCheckQueryVersion( xcb_connection_t * connection ) {
    auto queryCookie = xcb_shape_query_version( connection );

    std::unique_ptr< xcb_shape_query_version_reply_t > queryReply {
        xcb_shape_query_version_reply( connection, queryCookie, nullptr )
    };

    if ( !queryReply ) {
        throw std::runtime_error(
        "shape query version request failed" );
    }
}

inline void shmCheckQueryVersion( xcb_connection_t * connection ) {
    auto queryCookie = xcb_shm_query_version( connection );

    std::unique_ptr< xcb_shm_query_version_reply_t > queryReply {
        xcb_shm_query_version_reply( connection, queryCookie, nullptr )
    };

    if ( !queryReply ) {
        throw std::runtime_error(
        "shm query version request failed" );
    }
}

#undef CHECK_QUERY_VERSION
}   // namespace xcbwraper
