#include "project.h"

#include "prfile.h"
#include <cdirparser.h>
#include <cfile.h>
#include <libapp.h>
#include <libpath.h>
#include <libstr.h>
#include <print.h>
#include <sys/stat.h>

static bool _test_word(const char *word)
{
    return (str_startswith(word, "HAVE_", true));
}

static int _compare(void *entry1, void *entry2)
{
    ProjectFile *e1 = *((ProjectFile**) entry1);
    ProjectFile *e2 = *((ProjectFile**) entry2);

    //return strcmp(c_str(e1->filepath), c_str(e2->filepath));

    return path_cmp(c_str(e1->filepath), c_str(e2->filepath));
}

Project* project_new()
{
    Project *project = malloc(sizeof(Project));

    project->dirpath = cstr_new_size(64);
    project->name = cstr_new_size(16);
    project->list = clist_new(128, (CDeleteFunc) prfile_free);
    project->parser = wp_new(_test_word);

    return project;
}

void project_free(Project *project)
{
    if (!project)
        return;

    cstr_free(project->dirpath);
    cstr_free(project->name);
    clist_free(project->list);
    wp_free(project->parser);

    free(project);
}

bool project_parse(Project *project, const char *dirpath, const char *name)
{
    if (!dir_exists(dirpath) || !name || !*name)
        return false;

    cstr_copy(project->dirpath, dirpath);
    cstr_copy(project->name, name);
    clist_clear(project->list);

    CDirParserAuto *dir = cdirparser_new();
    if (!cdirparser_open(dir, dirpath, CDP_FILES | CDP_SUBDIRS | CDP_RELATIVE))
        return 1;

    CList *list = project->list;
    CStringAuto *filepath = cstr_new_size(64);
    CStringAuto *fullpath = cstr_new_size(64);

    while (cdirparser_read(dir, filepath))
    {
        const char *pathstr = c_str(filepath);

        if (str_startswith(pathstr, "build/", true)
            || str_startswith(pathstr, "other/", true))
            continue;

        FileType type = file_get_type(pathstr);

        if (type != FTYPE_HEADER && type != FTYPE_SOURCE)
            continue;

        ProjectFile *prfile = prfile_new(pathstr, type);
        clist_append(list, prfile);

        path_join(fullpath, dirpath, c_str(filepath));
        if (!wp_readfile(project->parser, c_str(fullpath)))
            print("parse error : %s", c_str(fullpath));
    }

    clist_sort(list, (CCompareFunc) _compare);
    wp_sort(project->parser);

    int size = clist_size(list);

    for (int i = 0; i < size; ++i)
    {
        ProjectFile *prfile = (ProjectFile*) clist_at(list, i);
        const char *pathstr = c_str(prfile->filepath);

        print(pathstr);
    }

    project_writepro(project);
    project_writemeson(project);
    project_writeinstall(project);

    return true;
}

bool project_writepro(Project *project)
{
    CStringAuto *fullpath = cstr_new_size(64);
    path_join(fullpath, c_str(project->dirpath), c_str(project->name));
    cstr_append(fullpath, ".pro");

    CFileAuto *outfile = cfile_new();
    if (!cfile_open(outfile, c_str(fullpath), "wb"))
        return false;

    cfile_write(outfile, "TEMPLATE = app\n");
    cfile_writefmt(outfile, "TARGET = %s\n", c_str(project->name));
    cfile_write(outfile, "CONFIG = c99 link_pkgconfig\n");
    cfile_write(outfile, "DEFINES =\n");
    cfile_write(outfile, "INCLUDEPATH =\n");
    cfile_write(outfile, "PKGCONFIG =\n");
    cfile_write(outfile, "\n");
    cfile_write(outfile, "#PKGCONFIG += tinyc\n");
    cfile_write(outfile, "#PKGCONFIG += gtk+-3.0\n");
    cfile_write(outfile, "#PKGCONFIG += glib-2.0\n");
    cfile_write(outfile, "\n");

    int size = clist_size(project->list);

    cfile_write(outfile, "HEADERS = \\\n");

    for (int i = 0; i < size; ++i)
    {
        ProjectFile *prfile = (ProjectFile*) clist_at(project->list, i);

        if (prfile_isheader(prfile))
        {
            const char *pathstr = c_str(prfile->filepath);
            cfile_writefmt(outfile, "    %s \\\n", pathstr);

            //print(pathstr);
        }
    }
    cfile_write(outfile, "\n");

    cfile_write(outfile, "SOURCES = \\\n");
    for (int i = 0; i < size; ++i)
    {
        ProjectFile *prfile = (ProjectFile*) clist_at(project->list, i);

        if (prfile_issource(prfile))
        {
            const char *pathstr = c_str(prfile->filepath);
            cfile_writefmt(outfile, "    %s \\\n", pathstr);
        }
    }
    cfile_write(outfile, "\n");

    cfile_write(outfile, "DISTFILES = \\\n");
    cfile_write(outfile, "    install.sh \\\n");
    cfile_write(outfile, "    meson.build \\\n");
    cfile_write(outfile, "\n");
    cfile_write(outfile, "\n");

    return true;
}

bool project_writemeson(Project *project)
{
    CStringAuto *fullpath = cstr_new_size(64);
    path_join(fullpath, c_str(project->dirpath), "meson.build");

    CFileAuto *outfile = cfile_new();
    if (!cfile_open(outfile, c_str(fullpath), "wb"))
        return false;

    cfile_write(outfile, "project(\n");
    cfile_writefmt(outfile, "    '%s',\n", c_str(project->name));
    cfile_write(outfile, "    ['c'],\n");
    cfile_write(outfile, "    version : '1.0',\n");
    cfile_write(outfile, "    license : 'GPL-2.0',\n");
    cfile_write(outfile, "    default_options : [\n");
    cfile_write(outfile, "        'c_std=c99',\n");
    cfile_write(outfile, "    ],\n");
    cfile_write(outfile, ")\n");
    cfile_write(outfile, "\n");
    cfile_write(outfile, "c_args = [\n");
    cfile_write(outfile, "    '-Wall',\n");
    cfile_write(outfile, "    '-Wextra',\n");
    cfile_write(outfile, "    '-O2',\n");
    cfile_write(outfile, "]\n");
    cfile_write(outfile, "\n");
    cfile_write(outfile, "app_deps = [\n");
    cfile_write(outfile, "    #dependency('tinyc'),\n");
    cfile_write(outfile, "    #dependency('gtk+-3.0'),\n");
    cfile_write(outfile, "    #dependency('glib-2.0'),\n");
    cfile_write(outfile, "]\n");
    cfile_write(outfile, "\n");

    int size = clist_size(project->list);

    cfile_write(outfile, "app_src = [\n");
    for (int i = 0; i < size; ++i)
    {
        ProjectFile *prfile = (ProjectFile*) clist_at(project->list, i);

        if (prfile_issource(prfile))
        {
            const char *pathstr = c_str(prfile->filepath);
            cfile_writefmt(outfile, "    '%s',\n", pathstr);
        }
    }
    cfile_write(outfile, "]\n");
    cfile_write(outfile, "\n");

    cfile_write(outfile, "executable(\n");
    cfile_write(outfile, "    meson.project_name(),\n");
    cfile_write(outfile, "    c_args : c_args,\n");
    cfile_write(outfile, "    dependencies : app_deps,\n");
    cfile_write(outfile, "    sources : app_src,\n");
    cfile_write(outfile, "    install : false,\n");
    cfile_write(outfile, ")\n");
    cfile_write(outfile, "\n");
    cfile_write(outfile, "\n");

    return true;
}

bool project_writeinstall(Project *project)
{
    CStringAuto *fullpath = cstr_new_size(64);
    path_join(fullpath, c_str(project->dirpath), "install.sh");

    CFileAuto *outfile = cfile_new();
    if (!cfile_open(outfile, c_str(fullpath), "wb"))
        return false;

    cfile_write(outfile, "#!/usr/bin/bash\n");
    cfile_write(outfile, "\n");
    cfile_write(outfile, "BASEDIR=\"$(dirname -- \"$(readlink -f -- \"$0\";)\")\"\n");
    cfile_write(outfile, "\n");
    cfile_write(outfile, "dest=$BASEDIR/build\n");
    cfile_write(outfile, "if [[ -d $dest ]]; then\n");
    cfile_write(outfile, "    rm -rf $dest\n");
    cfile_write(outfile, "fi\n");
    cfile_write(outfile, "\n");
    cfile_write(outfile, "meson build -Dbuildtype=plain\n");
    cfile_write(outfile, "ninja -C build\n");
    cfile_write(outfile, "sudo ninja -C build install\n");
    cfile_write(outfile, "\n");
    cfile_write(outfile, "\n");

    fchmod(cfile_fd(outfile), 0755);

    return true;
}


