#include "project.h"

#include <print.h>
#include <unistd.h>

static int _app_exit(bool usage, int ret)
{
    if (usage)
        print("*** usage : prgen -d /path/to/project -n projectname");

    return ret;
}

int main(int argc, char **argv)
{
    ProjectAuto *project = project_new();

    c_autofree char *dirpath = NULL;
    const char *name = NULL;

    int n = 1;

    while (n < argc)
    {
        const char *part = argv[n];

        if (strcmp(part, "-d") == 0)
        {
            if (++n >= argc)
                return EXIT_FAILURE;

            dirpath = argv[n];
        }
        else
        {
            name = argv[n];
        }

        n++;
    }

    if (!dirpath)
    {
        dirpath = getcwd(NULL, 0);

        if (!dirpath)
        {
            print("*** unable to read current directory, abort...");
            return EXIT_FAILURE;
        }
    }

    if (!name)
        return _app_exit(true, EXIT_FAILURE);

    if (!project_parse(project, dirpath, name))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


