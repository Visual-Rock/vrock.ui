
if host_machine.system() == 'darwin'
  add_languages('objcpp')
endif

im_include_dirs = include_directories('imgui', 'imgui/backends', 'imgui_theme')
im_sources = files(
  'imgui/misc/cpp/imgui_stdlib.cpp',
  'imgui/misc/freetype/imgui_freetype.cpp',
  'imgui/imgui.cpp',
  'imgui/imgui_demo.cpp',
  'imgui/imgui_draw.cpp',
  'imgui/imgui_tables.cpp',
  'imgui/imgui_widgets.cpp',
)

cpp = meson.get_compiler('cpp')
im_dependencies = []

if get_option('renderer') == 'opengl'
  libgl_dep = dependency('gl')
  libfreetype_dep = dependency('freetype')
  im_dependencies += libfreetype_dep 
  if libgl_dep.found()
    im_sources += 'imgui/backends/imgui_impl_opengl3.cpp'
    im_dependencies += libgl_dep
    im_dependencies += cpp.find_library('dl', required: false)
  endif
  glfw_dep = dependency('glfw3')
  if glfw_dep.found()
    im_sources += 'imgui/backends/imgui_impl_glfw.cpp'
    im_dependencies += glfw_dep
  endif
  glad_dep = dependency('glew')
  if glad_dep.found()
    im_dependencies += glad_dep
  endif
endif

if get_option('renderer') == 'vulkan'
  libvulkan_dep = dependency('vulkan')
  libfreetype_dep = dependency('freetype')
  im_dependencies += libfreetype_dep
  if libvulkan_dep.found()
    im_sources += 'imgui/backends/imgui_impl_vulkan.cpp'
    im_dependencies += libvulkan_dep
    im_dependencies += cpp.find_library('dl', required: false)
  endif
  glfw_dep = dependency('glfw3')
  if glfw_dep.found()
    im_sources += 'imgui/backends/imgui_impl_glfw.cpp'
    im_dependencies += glfw_dep
  endif
endif

api = '-DIMGUI_API=__attribute__((visibility("default")))'
if host_machine.system() == 'windows'
  api = '-DIMGUI_API=@0@'.format(get_option('default_library') != 'static' ? '__declspec(dllexport)' : '')
endif

imgui = library(
  'imgui',
  im_sources,
  cpp_args: api,
  gnu_symbol_visibility: 'hidden',
  dependencies: im_dependencies,
  include_directories: im_include_dirs,
)

if host_machine.system() == 'windows'
  api = '-DIMGUI_API=@0@'.format(get_option('default_library') != 'static' ? '__declspec(dllimport)' : '')
endif

imgui_dep = declare_dependency(
  compile_args: api,
  include_directories: im_include_dirs,
  link_with: imgui,
  dependencies: im_dependencies,
)

subdir('./tinyfiledialog/')

if get_option('tests')

endif