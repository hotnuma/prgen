#include "project.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define APPNAME "prgen"

static void error_exit(const char *msg)
{
    if (!msg || *msg == '\0')
    {
        msg = "an error occurred";
    }

    printf("*** %s\nabort...\n", msg);

    exit(EXIT_FAILURE);
}

static void usage_exit()
{
    printf("*** usage :");
    printf("%s -m -s <projectname>", APPNAME);
    printf("%s <projectname>", APPNAME);
    printf("abort...\n");

    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    ProjectAuto *project = project_new();

    CStringAuto *dirpath = cstr_new_size(64);
    bool config = false;
    const char *name = NULL;

    int n = 1;

    while (n < argc)
    {
        const char *part = argv[n];

        if (strcmp(part, "-m") == 0)
        {
            project->write_meson = true;
        }
        else if (strcmp(part, "-s") == 0)
        {
            project->write_install = true;
        }
        else if (strcmp(part, "-c") == 0)
        {
            config = true;
        }
        else
        {
            name = argv[n];
        }

        ++n;
    }

    // project directory
    char *pwd = getcwd(NULL, 0);
    cstr_copy(dirpath, pwd);
    free(pwd);

    if (cstr_isempty(dirpath))
        error_exit("unable to read current directory");

    // generate config.h.in
    if (config)
    {
        WordParser *parser = project->parser;

        if (!wp_readdir(parser, c_str(dirpath)))
            usage_exit();

        int size = cstrlist_size(parser->list);

        if (size < 1)
            return EXIT_SUCCESS;

        printf("/* config.h.in.  Generated from sources by prgen.  */\n\n");

        for (int i = 0; i < size; ++i)
        {
            CString *item = cstrlist_at(parser->list, i);
            printf("#mesondefine %s\n\n", c_str(item));
        }

        return EXIT_SUCCESS;
    }

    if (!name)
        usage_exit();

    if (!project_parse(project, c_str(dirpath), name))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


