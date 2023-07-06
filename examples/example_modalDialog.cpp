#include "vrock/ui/Application.hpp"

#include "vrock/utils/FutureHelper.hpp"

class DialogSelectNumber : public vrock::ui::ModalDialog<int>
{
public:
    DialogSelectNumber( std::shared_ptr<vrock::ui::Application> app, std::string title )
        : vrock::ui::ModalDialog<int>( app, title )
    {
    }

    void setup( ) final
    {
        number = 0;
    }

    void render( ) final
    {
        ImGui::DragInt( "Number:", &number, 1.0f, 0, 100 );
        if ( ImGui::Button( "close" ) )
            close( number );
    }

    void terminate( ) final
    {
    }

    int number = 0;
};

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

        if ( ImGui::Begin( "Test" ) )
        {
            if ( ImGui::Button( "open" ) )
                number = app->open_modal_dialog<int>( std::make_shared<DialogSelectNumber>( app, "Select a Number" ) );
            if ( number.valid( ) && vrock::utils::future_ready( number ) )
            {
                vrock::log::get_logger( "ui" )->log->debug( "number: {}", number.get( ) );
                number = std::shared_future<int>( );
            }
            ImGui::End( );
        }
    }
    void terminate( ) final
    {
    }

    std::shared_future<int> number;
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