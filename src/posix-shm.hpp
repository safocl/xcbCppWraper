#pragma once

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <span>

namespace posix {

class SharedMemory final {
    int         mId { 0 };
    void *      mPtr { nullptr };
    std::size_t mSize { 0 };

public:
    using Shared = std::shared_ptr< SharedMemory >;

    SharedMemory();
    explicit SharedMemory( std::size_t sizeInBytes );
    ~SharedMemory();

    operator bool() const;

    template < class NeededType > std::span< NeededType > getData() const {
        std::size_t size = mSize / ( sizeof( NeededType ) / sizeof( std::uint8_t ) );

        std::span< NeededType > data( static_cast< NeededType * >( mPtr ), size );

        return data;
    }

    void init( std::size_t sizeInBytes );
    void reinit( std::size_t sizeInBytes );
    void release();
    int  getId() const;
    std::size_t getSize() const;
};

}   // namespace posix
