#include <cstddef>
#include <exception>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <span>

#include <xcb/xproto.h>

#include "pixmap.hpp"
#include "drawable.hpp"
#include "types.hpp"
#include "xcbconnection.hpp"
#include "xcbwindowprop.hpp"
#include "posix-shm.hpp"

namespace xcbwraper {

class Window : public Drawable {
public:
    enum class DestroyStrategy : u8 { eDetach, eClose };

protected:
    DestroyStrategy     mStrategy { DestroyStrategy::eDetach };

public:
    struct CreateInfo final {
        XcbConnection::Shared connection {};
        std::optional< xcb_window_t > xcbId {};
        DestroyStrategy     destroyStrategy { DestroyStrategy::eDetach };
    };

    explicit Window( const CreateInfo & );
    Window( const Window & ) = default;
    Window( Window && )      = default;
    virtual ~Window();
    Window & operator=( const Window & ) = default;
    Window & operator=( Window && ) = default;

    Window & operator=( xcb_window_t );
             operator xcb_window_t() const;
             operator bool() const;

             void attachOffscreenImageToPixmap( const Pixmap & destination );

             struct GetPropertyInfo final {
                 AtomTypes         type;
                 PropertyFormat    format;
                 std::vector< u8 > value;
             };
             GetPropertyInfo getProperty( Atoms     atom,
                                          AtomTypes atomType,
                                          bool      deleteProperty ) const;

             using ChangePropertyData = std::span< u8 >;
             void changeProperty( Atoms,
                                  AtomTypes,
                                  PropertyFormat,
                                  ChangePropertyMode,
                                  ChangePropertyData );

             void deleteProperty( Atoms property );

             std::vector< Atoms > listProperties() const;

             struct QueryTreeInfo final {
                 WindowShared                root;
                 WindowShared                parent;
                 std::vector< WindowShared > children;
             };
             QueryTreeInfo queryTree() const;

             virtual void redirect();
             virtual void redirectSubwindow();
             virtual void unredirect();
             virtual void unredirectSubwindow();
             virtual void release();
             virtual void map();
             virtual void unmap();
             virtual void fullDamaged();
             void         detach();
             bool         isViewable() const;
};

class CompositeWindow : public Window {
public:
    explicit CompositeWindow( CreateInfo );
    virtual ~CompositeWindow();

    virtual void release() override;
    void         redirectSubwindow() override;
    void         unredirectSubwindow() override;
};

using WindowShared = std::shared_ptr< Window >;

}   // namespace xcbwraper
