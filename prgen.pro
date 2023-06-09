TEMPLATE = app
TARGET = prgen
CONFIG = c99 link_pkgconfig
DEFINES =
INCLUDEPATH =
PKGCONFIG =

PKGCONFIG += tinyc

HEADERS = \
    prfile.h \
    project.h \
    wparser.h

SOURCES = \
    0Temp.c \
    main.c \
    prfile.c \
    project.c \
    wparser.c

DISTFILES = \
    install.sh \
    License.txt \
    meson.build \
    Readme.md \


