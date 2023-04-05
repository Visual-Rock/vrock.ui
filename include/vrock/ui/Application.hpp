#pragma once

#include <memory>

#include "ImGuiBaseWidget.hpp"

namespace vrock::ui
{
    class Application
    {
    public:
        Application( )
        {
        }
        ~Application( )
        {
        }

        auto run( std::shared_ptr<ImGuiBaseWidget> root ) -> int;
    };
} // namespace vrock::ui
