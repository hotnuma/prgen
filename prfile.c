#include "prfile.h"

#include <libpath.h>

FileType file_get_type(const char *filepath)
{
    const char *ext = path_ext(filepath, true);

    if (!ext)
        return FTYPE_UNKNOWN;

    if (strcmp(ext, ".h") == 0)
    {
        return FTYPE_HEADER;
    }
    else if (strcmp(ext, ".c") == 0)
    {
        return FTYPE_SOURCE;
    }

    return FTYPE_UNKNOWN;
}

ProjectFile* prfile_new(const char *filepath, FileType type)
{
    ProjectFile *file = (ProjectFile*) malloc(sizeof(ProjectFile));
    file->filepath = cstr_new(filepath);
    file->type = type;

    return file;
}

void prfile_free(ProjectFile *file)
{
    if (!file)
        return;

    cstr_free(file->filepath);
}

bool prfile_isheader(ProjectFile *file)
{
    return (file->type == FTYPE_HEADER);
}

bool prfile_issource(ProjectFile *file)
{
    return (file->type == FTYPE_SOURCE);
}


