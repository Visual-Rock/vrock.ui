#pragma once

#include <imgui.h>

namespace vrock::ui
{
    class ImGuiBaseWidget
    {
    public:
        ImGuiBaseWidget( );
        ~ImGuiBaseWidget( );

        virtual void setup( ) = 0;
        virtual void render( ) = 0;
        virtual void terminate( ) = 0;

        auto get_visibility( ) -> bool;
        auto set_visibility( bool new_visibility ) -> void;

    protected:
        bool visibility = true;

        bool t = true;
        bool f = false;
    };
} // namespace vrock::ui
