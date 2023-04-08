#pragma once

#include <memory>
#include <string>
#include <functional>

#include "ImGuiBaseWidget.hpp"

#include "vrock/log/Logger.hpp"

namespace vrock::ui
{
    class VROCKUI_API ApplicationConfig 
    {
    public:
        std::string application_name = "vrock.ui";
        uint32_t width = 1280;
        uint32_t height = 720;

        ImGuiConfigFlags config_flags = 0;
        ImGuiBackendFlags backend_flags = 0;
    };

    class VROCKUI_API Application
    {
    public:
        Application( std::shared_ptr<log::Logger> logger ) : logger( logger )
        {
        }
        ~Application( )
        {
        }

        auto run( ApplicationConfig config, std::shared_ptr<ImGuiBaseWidget> root ) -> int;

        auto rename_window( std::string title ) -> void;
    protected:
        std::shared_ptr<log::Logger> logger;
        std::function<void( std::string )> rename;
    };
} // namespace vrock::ui
