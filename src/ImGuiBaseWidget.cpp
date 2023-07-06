#include "vrock/ui/ImGuiBaseWidget.hpp"

namespace vrock::ui
{
    ImGuiBaseWidget::ImGuiBaseWidget( std::shared_ptr<vrock::ui::Application> app ) : app( app )
    {
    }
    ImGuiBaseWidget::~ImGuiBaseWidget( )
    {
    }

    auto ImGuiBaseWidget::get_visibility( ) const -> bool
    {
        return visibility;
    }

    auto ImGuiBaseWidget::set_visibility( bool new_visibility ) -> void
    {
        visibility = new_visibility;
    }
}