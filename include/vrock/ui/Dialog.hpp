#pragma once

#include "ImGuiBaseWidget.hpp"

namespace vrock::ui
{
    class Dialog : public ImGuiBaseWidget
    {
    public:
        Dialog( std::shared_ptr<vrock::ui::Application> app, std::string title,
                ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking )
            : ImGuiBaseWidget( app ), title( title ), closed( false )
        {
        }
        ~Dialog( )
        {
        }

        auto close( ) -> void
        {
            closed = true;
        }

        auto is_closed( ) -> bool
        {
            return closed;
        }

        auto get_title( ) -> std::string
        {
            return title;
        }

        auto get_flags( ) -> ImGuiWindowFlags
        {
            return flags;
        }

    private:
        std::string title;
        bool closed = false;
        ImGuiWindowFlags flags;
    };

    template <class T>
    concept DefaultConstructible = std::is_trivially_default_constructible_v<T>;

    template <DefaultConstructible T> class ModalDialog : public Dialog
    {
    public:
        ModalDialog( std::shared_ptr<vrock::ui::Application> app, std::string title,
                     ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking )
            : Dialog( app, title, flags )
        {
        }
        ~ModalDialog( )
        {
            if ( future.wait_for( std::chrono::milliseconds( 0 ) ) != std::future_status::ready )
                result.set_value( T( ) );
        }

        auto close( T res = T( ) ) -> void
        {
            result.set_value( res );
            Dialog::close( );
        }

        auto set_promise( std::promise<T> promise ) -> void
        {
            result = std::move( promise );
            future = result.get_future( ).share( );
        }

        auto get_future( ) -> std::shared_future<T>
        {
            return future;
        }

    private:
        std::promise<T> result;
        std::shared_future<T> future;
    };
} // namespace vrock::ui