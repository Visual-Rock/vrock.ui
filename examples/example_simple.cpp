#include "vrock/ui/Application.hpp"

class MainWindow : public vrock::ui::ImGuiBaseWidget
{
public:
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
    auto app = std::make_shared<vrock::ui::Application>( );
    app->run( std::make_shared<MainWindow>( ) );
    return 0;
}