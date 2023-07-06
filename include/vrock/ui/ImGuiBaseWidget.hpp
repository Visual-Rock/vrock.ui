#pragma once

#define IMGUI_ENABLE_FREETYPE
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <memory>

#ifndef VROCKUI_API
#define VROCKUI_API
#endif

namespace vrock::ui
{
    class Application;

    class VROCKUI_API ImGuiBaseWidget
    {
    public:
        ImGuiBaseWidget( std::shared_ptr<vrock::ui::Application> app );
        ~ImGuiBaseWidget( );

        virtual void setup( ) = 0;
        virtual void render( ) = 0;
        virtual void terminate( ) = 0;

        auto get_visibility( ) const -> bool;
        auto set_visibility( bool new_visibility ) -> void;

    protected:
        bool visibility = true;
        std::shared_ptr<vrock::ui::Application> app;

        bool t = true;
        bool f = false;
    };
} // namespace vrock::ui
