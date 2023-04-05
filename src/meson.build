src = [
    'ImGuiBaseWidget.cpp',
    'Application.cpp'
]

header = [
    '../include/vrock/ui/ImGuiBaseWidget.hpp',
    '../include/vrock/ui/Application.hpp'
]

public_header = include_directories('../include')

uilib = library(meson.project_name(), src,
    include_directories: public_header,
    dependencies: imgui_dep
)

uilib_dep = declare_dependency(
    include_directories: public_header,
    link_with: uilib,
    dependencies: imgui_dep
)
set_variable(meson.project_name() + '_dep', uilib_dep)

install_headers(header, subdir: 'vrock/ui')

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : '',
  subdirs : meson.project_name(),
  libraries : uilib,
)