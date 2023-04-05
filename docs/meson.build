if get_option('docs')

doxygen = find_program('doxygen', required : false)
apidoc = find_program('breathe-apidoc', required : false)
sphinx_build = find_program('sphinx-build', required : false)

conf_data = configuration_data()
conf_data.set('FILES', ' '.join(header) + ' ' + ' '.join(src)) # headers from the src meson.build
doxy_file = configure_file(input: 'Doxyfile.in',
    output : 'Doxyfile',
    configuration : conf_data
)


if doxygen.found()
    # generate doxygen xml files
    doxygen_xml = custom_target('doxygen-xml', output: 'xml', command: [ doxygen, doxy_file])
    
    if apidoc.found()  
        apidoc_gen = custom_target('gen-breathe-apidoc',
            output: 'apidoc',
            command: [ apidoc, '-o', '../docs/out', '-m', '-f', '../docs/xml'],
            depends: [ doxygen_xml ]
        )

        if sphinx_build.found()
            custom_target('docs',
                output: '.',
                command: [ sphinx_build, '-b', 'html', meson.source_root() + '/docs/', meson.source_root() + '/docs/html'],
                install: false,
                depends: [ apidoc_gen ],
                build_by_default : true
            )
        else
            warning('docs can not be build without sphinx-build')
        endif
    else
        warning('docs can not be build without breathe-apidoc')
    endif  
else
    warning('docs can not be build without doxygen')
endif
endif