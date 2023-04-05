
if host_machine.system() == 'darwin'
  add_languages('objcpp')
endif

include_dirs = include_directories('imgui', 'imgui/backends')
sources = files(
  'imgui/misc/cpp/imgui_stdlib.cpp',
  'imgui/imgui.cpp',
  'imgui/imgui_demo.cpp',
  'imgui/imgui_draw.cpp',
  'imgui/imgui_tables.cpp',
  'imgui/imgui_widgets.cpp',
)

cpp = meson.get_compiler('cpp')
dependencies = []

libgl_dep = dependency('gl')
if libgl_dep.found()
  sources += 'imgui/backends/imgui_impl_opengl3.cpp'
  dependencies += libgl_dep
  dependencies += cpp.find_library('dl', required: false)
endif
glfw_dep = dependency('glfw3')
if glfw_dep.found()
  sources += 'imgui/backends/imgui_impl_glfw.cpp'
  dependencies += glfw_dep
endif
glad_dep = dependency('glew')
if glad_dep.found()
  dependencies += glad_dep
endif

api = '-DIMGUI_API=__attribute__((visibility("default")))'
if host_machine.system() == 'windows'
  api = '-DIMGUI_API=@0@'.format(get_option('default_library') != 'static' ? '__declspec(dllexport)' : '')
endif

imgui = library(
  'imgui',
  sources,
  cpp_args: api,
  gnu_symbol_visibility: 'hidden',
  dependencies: dependencies,
  include_directories: include_dirs,
)

if host_machine.system() == 'windows'
  api = '-DIMGUI_API=@0@'.format(get_option('default_library') != 'static' ? '__declspec(dllimport)' : '')
endif

imgui_dep = declare_dependency(
  compile_args: api,
  include_directories: include_dirs,
  link_with: imgui,
  dependencies: dependencies,
)

if get_option('tests')

endif