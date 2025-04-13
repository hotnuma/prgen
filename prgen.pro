TEMPLATE = app
TARGET = prgen
CONFIG = c99 link_pkgconfig
DEFINES = _GNU_SOURCE bool=BOOL true=TRUE false=FALSE
INCLUDEPATH =
PKGCONFIG =

PKGCONFIG += tinyc

HEADERS = \
    pathcmp.h \
    prfile.h \
    project.h \
    wparser.h

SOURCES = \
    0Temp.c \
    main.c \
    pathcmp.c \
    prfile.c \
    project.c \
    wparser.c

DISTFILES = \
    install.sh \
    License.txt \
    meson.build \
    Readme.md \


