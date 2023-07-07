src = [
    'ImGuiBaseWidget.cpp',
    'Application.cpp',
    'FileDialogs.cpp',
    '../external/imgui_theme/spectrum.cpp',
    'stb/stb.cpp',
    'Image.cpp'
]

header = [
    '../include/vrock/ui/ImGuiBaseWidget.hpp',
    '../include/vrock/ui/Application.hpp',
    '../include/vrock/ui/FileDialogs.hpp'
]

public_header = include_directories('../include')
log_dep = dependency('vrocklog')
fmt_dep = dependency('fmt')

api = '-DVROCKUI_API=__attribute__((visibility("default")))'
if host_machine.system() == 'windows'
  api = '-DVROCKUI_API=@0@'.format(get_option('default_library') != 'static' ? '__declspec(dllexport)' : '')
endif

uilib = library(meson.project_name(), src,
    include_directories: public_header,
    cpp_args: [api ],
    gnu_symbol_visibility: 'hidden',
    dependencies: [ imgui_dep, log_dep, tfd_dep, fmt_dep ]
)

if host_machine.system() == 'windows'
  api = '-DVROCKUI_API=@0@'.format(get_option('default_library') != 'static' ? '__declspec(dllimport)' : '')
endif

uilib_dep = declare_dependency(
    compile_args: api,
    include_directories: public_header,
    link_with: uilib,
    dependencies: [ imgui_dep, log_dep, tfd_dep, fmt_dep ]
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