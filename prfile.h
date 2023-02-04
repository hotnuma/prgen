#ifndef PRFILE_H
#define PRFILE_H

#include <cstring.h>

typedef enum
{
    FTYPE_UNKNOWN,
    FTYPE_HEADER,
    FTYPE_SOURCE,

} FileType;

typedef struct
{
    CString *filepath;
    FileType type;

} ProjectFile;

FileType file_get_type(const char *filepath);

ProjectFile* prfile_new(const char *filepath, FileType type);
void prfile_free(ProjectFile *file);

bool prfile_isheader(ProjectFile *file);
bool prfile_issource(ProjectFile *file);

#endif // PRFILE_H


