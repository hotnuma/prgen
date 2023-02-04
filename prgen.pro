TEMPLATE = app
TARGET = prgen
CONFIG = c99 link_pkgconfig
DEFINES =
INCLUDEPATH =
PKGCONFIG =

PKGCONFIG += tinyc
#PKGCONFIG += gtk+-3.0
#PKGCONFIG = glib-2.0
#PKGCONFIG += dbus-1
#PKGCONFIG += libxml-2.0

HEADERS = \
    prfile.h \
    project.h \
    wrparser.h

SOURCES = \
    prfile.c \
    project.c \
    samples/cdirparser.c \
    samples/cfile_read.c \
    samples/cfile_readwrite.c \
    samples/cfile_words.c \
    samples/cprocess.c \
    0Temp.c \
    main.c \
    samples/func_file_getline.c \
    samples/func_file_read.c \
    samples/func_str_getlineptr.c \
    wrparser.c

DISTFILES = \
    install.sh \
    License.txt \
    meson.build \
    Readme.md \


