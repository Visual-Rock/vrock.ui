How to use
=======================================

vrock libs use the meson build system so integrating it into your meson project is fairly easy.
you can create a .wrap file and put it in your `subproject` folder. The file should look like this.

.. code-block:: ini
    :caption: wrap-file

    [wrap-git]
    url=https://github.com/Visual-Rock/vrock.ui
    revision=head
    depth=1

    [provide]
    vrockui=vrockui_dep

with this file in your `subproject` you can add the following lines to your meson.build file

.. code-block::
    :caption: dependency object

    uilib_dep = dependency('vrockui', fallback: ['vrockui', 'vrockui_dep'])
    # you can add the default_options to build tests, examples and docs 
    uilib_dep = dependency('vrockui', 
        fallback: ['vrockui', 'vrockui_dep'], 
        default_options: ['tests=true', 'examples=true', 'docs=true']
    )

now you can use `uilib_dep` as a meson dependency on your build targets like this.

.. code-block::
    :caption: adding as dependency

    exe = executable(
        'your_executable', 'main.cpp',
        dependencies: [ uilib_dep ]
    )