#include "vrock/ui/Application.hpp"

class MainWindow : public vrock::ui::ImGuiBaseWidget
{
public:
    MainWindow( std::shared_ptr<vrock::ui::Application> app ) : vrock::ui::ImGuiBaseWidget( app )
    {
    }

    void setup( ) final
    {
    }
    void render( ) final
    {
        ImGui::ShowDemoWindow( );
    }
    void terminate( ) final
    {
    }
};

int main( )
{
    auto log_cfg = vrock::log::LoggerConfig( "ui" ).set_log_level( vrock::log::Debug ).add_console_colored( );

    auto app_config = vrock::ui::ApplicationConfig( );
    app_config.application_name = "simple example";
    app_config.config_flags |= ImGuiConfigFlags_DockingEnable;

    auto app = std::make_shared<vrock::ui::Application>( vrock::log::create_logger( log_cfg ) );
    app->run( app_config, std::make_shared<MainWindow>( app ) );
    return 0;
}