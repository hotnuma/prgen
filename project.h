#ifndef PROJECT_H
#define PROJECT_H

#include "wrparser.h"

#include <clist.h>

typedef struct
{
    CString *dirpath;
    CString *name;
    CList *list;
    WordParser *parser;

} Project;

Project* project_new();
void project_free(Project *project);
#define ProjectAuto GC_CLEANUP(_freeProject) Project
GC_UNUSED static inline void _freeProject(Project **project)
{
    project_free(*project);
}

bool project_parse(Project *project, const char *dirpath, const char *name);
bool project_writepro(Project *project);
bool project_writemeson(Project *project);
bool project_writeinstall(Project *project);

#endif // PROJECT_H


