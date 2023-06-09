#include "project.h"

#include <unistd.h>
#include <string.h>
#include <print.h>

static int _app_exit(bool usage, int ret)
{
    if (usage)
    {
        print("*** usage :");
        print("\tprgen -d /path/to/project projectname");
        print("\tprgen projectname");
    }

    return ret;
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

        if (strcmp(part, "-d") == 0)
        {
            if (++n >= argc)
                return EXIT_FAILURE;

            cstr_copy(dirpath, argv[n]);
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
    if (cstr_isempty(dirpath))
    {
        char *pwd = getcwd(NULL, 0);
        cstr_copy(dirpath, pwd);
        free(pwd);

        if (cstr_isempty(dirpath))
        {
            print("*** unable to read current directory, abort...");
            return EXIT_FAILURE;
        }
    }

    // generate config.h.in
    if (config)
    {
        WordParser *parser = project->parser;

        if (!wp_readdir(parser, c_str(dirpath)))
            _app_exit(false, EXIT_FAILURE);

        int size = cstrlist_size(parser->list);

        if (size < 1)
            return EXIT_SUCCESS;

        print("/* config.h.in.  Generated from sources by prgen.  */\n");

        for (int i = 0; i < size; ++i)
        {
            CString *item = cstrlist_at(parser->list, i);
            print("#mesondefine %s\n", c_str(item));
        }

        return EXIT_SUCCESS;
    }

    if (!name)
        return _app_exit(true, EXIT_FAILURE);

    if (!project_parse(project, c_str(dirpath), name))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


