#include "vrock/ui/ImGuiBaseWidget.hpp"

namespace vrock::ui
{
    ImGuiBaseWidget::ImGuiBaseWidget( )
    {
    }
    ImGuiBaseWidget::~ImGuiBaseWidget( )
    {
    }

    auto ImGuiBaseWidget::get_visibility( ) -> bool
    {
        return visibility;
    }

    auto ImGuiBaseWidget::set_visibility( bool new_visibility ) -> void
    {
        visibility = new_visibility;
    }
}