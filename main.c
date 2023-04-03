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
        else
        {
            name = argv[n];
        }

        n++;
    }

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

    if (!name)
        return _app_exit(true, EXIT_FAILURE);

    if (!project_parse(project, c_str(dirpath), name))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


