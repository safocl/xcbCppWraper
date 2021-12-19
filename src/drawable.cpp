#include "drawable.hpp"
#include "exception.hpp"

#include <xcb/shm.h>

namespace xcbwraper {

Drawable::Drawable( const CreateInfo & ci ) :
mConnection( ci.connection ), mId( ci.xcbId ) {}

Drawable::~Drawable() {}

Drawable::operator xcb_drawable_t() const { return mId.value(); }

void Drawable::attachImageDataToShm( posix::SharedMemory::Shared shm ) const {
    if ( !mId )
        throw Exception(
        "In void Drawable::attachImageDataToShm( posix::SharedMemory::Shared shm ) const",
        XCB_DRAWABLE );

    auto          geometry       = getGeometry().getInfo();
    constexpr u32 plane          = ~0;

    auto imageSize = getImageDataSize();

    //if ( allocAligmentInBytes )
    //if ( allocAligmentInBytes.value() != 0 ) {
    //const double k = static_cast< double >( imageSize ) /
    //static_cast< double >( allocAligmentInBytes.value() );
    //imageSize = std::ceil( k ) * allocAligmentInBytes.value();
    //}

    if ( imageSize > shm->getSize() )
        throw Exception(
        "In void Drawable::attachImageDataToShm( posix::SharedMemory::Shared shm ) const",
        XCB_DRAWABLE );

    shm->reinit( imageSize );

    const xcb_shm_seg_t shmSeg = xcb_generate_id( *mConnection );

    xcb_shm_attach( *mConnection, shmSeg, shm->getId(), false );

    const auto shmGetImgCoockie = xcb_shm_get_image( *mConnection,
                                                     mId.value(),
                                                     0,
                                                     0,
                                                     geometry.width,
                                                     geometry.height,
                                                     plane,
                                                     XCB_IMAGE_FORMAT_Z_PIXMAP,
                                                     shmSeg,
                                                     0 );

    xcb_generic_error_t * error = nullptr;

    std::unique_ptr< xcb_shm_get_image_reply_t > shmGetImgReply {
        xcb_shm_get_image_reply( *mConnection, shmGetImgCoockie, &error )
    };

    if ( error ) {
        std::unique_ptr< xcb_generic_error_t > errorUnique { error };

        throw Exception(
        "In void Drawable::attachImageDataToShm( posix::SharedMemory::Shared shm ) const: xcb_shm_get_image_reply()",
        error->error_code );
    }

    xcb_shm_detach( *mConnection, shmSeg );

    xcb_flush( *mConnection );
}

DrawableGeometry Drawable::getGeometry() const {
    xcb_generic_error_t * error;

    std::unique_ptr< xcb_get_geometry_reply_t > geometryRep { xcb_get_geometry_reply(
    *mConnection, xcb_get_geometry( *mConnection, mId.value() ), &error ) };

    if ( !geometryRep && error ) {
        if ( error ) {
            std::unique_ptr< xcb_generic_error_t > err( error );

            throw Exception(
            "In u32 Drawable::getGeometry() const : xcb_get_geometry_reply",
            error->error_code );
        } else
            throw std::runtime_error(
            "In u32 Drawable::getGeometry() const : xcb_get_geometry_reply : Unknow error" );
    }

    std::unique_ptr< xcb_query_tree_reply_t > tree { xcb_query_tree_reply(
    *mConnection, xcb_query_tree( *mConnection, mId.value() ), nullptr ) };

    if ( !tree && error ) {
        if ( error ) {
            std::unique_ptr< xcb_generic_error_t > err( error );

            throw Exception(
            "In u32 Drawable::getGeometry() const : xcb_query_tree_reply",
            error->error_code );
        } else
            throw std::runtime_error(
            "In u32 Drawable::getGeometry() const : xcb_query_tree_reply : Unknow error" );
    }
    auto screen = xcb_setup_roots_iterator( xcb_get_setup( *mConnection ) ).data;

    std::unique_ptr< xcb_translate_coordinates_reply_t > trans {
        xcb_translate_coordinates_reply(
        *mConnection,
        xcb_translate_coordinates(
        *mConnection, mId.value(), screen->root, geometryRep->x, geometryRep->y ),
        nullptr )
    };

    DrawableGeometry::CreateInfo ci { .leftTopPoint =
                                      Point { .x = trans->dst_x, .y = trans->dst_y },
                                      .width       = geometryRep->width,
                                      .height      = geometryRep->height,
                                      .borderWidth = geometryRep->border_width };
    return DrawableGeometry( ci );
}

Drawable::DrawableId Drawable::getXcbId() const { return mId.value(); }

u32 Drawable::getImageDataSize() const {
    auto geometry = getGeometry().getInfo();

    auto getImageCoockie = xcb_get_image( *mConnection,
                                          XCB_IMAGE_FORMAT_Z_PIXMAP,
                                          *this,
                                          0,
                                          0,
                                          geometry.width,
                                          geometry.height,
                                          ~0 );

    xcb_generic_error_t * error;

    std::unique_ptr< xcb_get_image_reply_t > getImageReply { xcb_get_image_reply(
    *mConnection, getImageCoockie, &error ) };

    if ( !getImageReply ) {
        if ( error )
            throw Exception(
            "In u32 Drawable::getImageDataSize() const : xcb_get_image_reply",
            error->error_code );
        else
            throw std::runtime_error(
            "In u32 Drawable::getImageDataSize() const : xcb_get_image_reply : Unknow error" );
    }

    return static_cast< u32 >( xcb_get_image_data_length( getImageReply.get() ) );
}

}   // namespace xcbwraper
