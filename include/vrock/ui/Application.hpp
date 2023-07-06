#pragma once

#include <functional>
#include <memory>
#include <string>

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

        auto run( const ApplicationConfig &config, std::shared_ptr<ImGuiBaseWidget> root ) -> int;

        auto rename_window( const std::string &title ) -> void;
        auto close_handler( std::function<bool( )> fn ) -> void;

        std::shared_ptr<log::Logger> logger;

    protected:
        std::function<void( const std::string & )> rename;
        std::function<bool( )> close_handler_ = []( ) { return true; };
        bool should_close = false;
    };
} // namespace vrock::ui
