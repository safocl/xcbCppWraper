#include "posix-shm.hpp"
#include "errno-exception.hpp"

#include <stdexcept>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cerrno>

namespace posix {

using xcbwraper::ErrnoException;

SharedMemory::SharedMemory() = default;

SharedMemory::SharedMemory( std::size_t sizeInBytes ) { init( sizeInBytes ); }

SharedMemory::~SharedMemory() { release(); }

SharedMemory::operator bool() const { return static_cast< bool >( mPtr ); }

void SharedMemory::init( std::size_t sizeInBytes ) {
    if ( sizeInBytes == 0 )
        throw std::runtime_error(
        "In void SharedMemory::init( std::size_t sizeInBytes ) : sizeInBytes is zero" );

    mSize = sizeInBytes;
    mId   = shmget( IPC_PRIVATE, mSize, IPC_CREAT | 0777 );

    if ( mId == -1 )
        throw ErrnoException(
        "In SharedMemory::SharedMemory( std::size_t sizeInBytes ) : shmget( IPC_PRIVATE, sizeInBytes, IPC_CREAT | 0777 )",
        errno );

    mPtr = shmat( mId, nullptr, 0 );
    if ( mPtr && *static_cast< int * >( mPtr ) == -1 )
        throw ErrnoException(
        "In SharedMemory::SharedMemory( std::size_t sizeInBytes ) : shmat( mId, nullptr, 0 )",
        errno );
}

void SharedMemory::reinit( std::size_t sizeInBytes ) {
    if ( sizeInBytes == 0 )
        throw std::runtime_error(
        "In SharedMemory::reinit( std::size_t sizeInBytes ) : sizeInBytes == 0 , this value is bad!!!" );

    if ( sizeInBytes != mSize ) {
        if ( *this )
            release();

        init( sizeInBytes );
    }
}

void SharedMemory::release() {
    if ( shmdt( mPtr ) == -1 )
        throw ErrnoException( "In SharedMemory::release() : shmdt( mPtr )", errno );

    if ( shmctl( mId, IPC_RMID, nullptr ) == -1 )
        throw ErrnoException(
        "In SharedMemory::release() : shmctl( mId, IPC_RMID, nullptr )", errno );

    mSize = 0;
    mId   = 0;
    mPtr  = nullptr;
}

int SharedMemory::getId() const { return mId; }
std::size_t SharedMemory::getSize() const { return mSize; }
}   // namespace posix
