project(
    'prgen',
    ['c'],
    default_options: ['c_std=c99'],
    license: 'GPL-2.0',
    version: '1.0',
)

c_args = [
    '-Wall',
    '-Wextra',
    '-O2',
]

app_deps = [
    dependency('tinyc'),
]

app_src = [
    'main.c',
    'pathcmp.c',
    'prfile.c',
    'project.c',
    'wparser.c',
]

executable(
    meson.project_name(),
    c_args : c_args,
    dependencies : app_deps,
    sources : app_src,
    install : true,
)


