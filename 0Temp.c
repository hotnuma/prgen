#if 0

#include <unistd.h>

c_autofree char *dirpath = getcwd(NULL, 0);

if (!dirpath)
{
    print("*** unable to read current directory, abort...");
    return EXIT_FAILURE;
}

#endif


