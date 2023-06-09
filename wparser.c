#include "wparser.h"

#include <cdirparser.h>
#include <cfile.h>
#include <libstr.h>
#include <ctype.h>
#include <fnmatch.h>

#include <print.h>

WordParser* wp_new(WordTestFunc func)
{
    WordParser *parser = (WordParser*) malloc(sizeof(WordParser));
    parser->list = cstrlist_new_size(128);
    parser->test = func;

    return parser;
}

void wp_free(WordParser *parser)
{
    if (parser->list)
        cstrlist_free(parser->list);

    free(parser);
}

bool wp_readdir(WordParser *parser, const char *indir)
{
    CDirParserAuto *dir = cdirparser_new();
    if (!cdirparser_open(dir, indir, CDP_FILES | CDP_SUBDIRS))
        return false;

    CStringAuto *filepath = cstr_new_size(256);

    while (cdirparser_read(dir, filepath, NULL))
    {
        if (fnmatch("*.c", c_str(filepath), 0) == FNM_NOMATCH
            && fnmatch("*.h", c_str(filepath), 0) == FNM_NOMATCH)
        {
            continue;
        }

        if (!wp_readfile(parser, c_str(filepath)))
            return false;
    }

    return true;
}

bool wp_readfile(WordParser *parser, const char *filepath)
{
    if (!parser->test || !file_exists(filepath))
        return false;

    CFileAuto *file = cfile_new();
    if (!cfile_read(file, filepath))
        return false;

    CStringAuto *line = cstr_new_size(32);
    CStringAuto *word = cstr_new_size(32);

    while (cfile_getline(file, line))
    {
        char *curr = cstr_data(line);

        while (get_word(&curr, word))
        {
            if (parser->test(c_str(word)))
            {
                if (cstrlist_find(parser->list, c_str(word), true) == -1)
                {
                    cstrlist_append(parser->list, c_str(word));
                }
            }
        }
    }

    return true;
}

void wp_sort(WordParser *parser)
{
    cstrlist_sort(parser->list, false);
}

bool get_word(char **start, CString *result)
{
    // end of buffer ?
    if (**start == '\0')
        return false;

    // start of line.
    char *first = *start;
    char *p = first;

    while (1)
    {
        if (isupper(*p) || *p =='_')
        {
            char *word = p;
            while (isupper(*p) || isdigit(*p) || *p =='_')
                ++p;

            if (islower(*p))
                continue;

            cstr_copy_len(result, word, p - word);

            // move to next word.
            *start = p;

            return true;
        }
        else if (*p == '\0')
        {
            return false;
        }

        ++p;
    }
}

static const char* _keywords[] =
{
    "DEBUG",
    "GETTEXT_PACKAGE",
    "PACKAGE",
    "PACKAGE_BUGREPORT",
    "PACKAGE_LOCALE_DIR",
    "PACKAGE_NAME",
    "PACKAGE_STRING",
    "PACKAGE_TARNAME",
    "PACKAGE_URL",
    "PACKAGE_VERSION",
    "VERSION",
    NULL
};

bool word_test(const char *word)
{
    if (str_startswith(word, "HAVE_", true))
    {
        if (strcmp(word, "HAVE_CONFIG_H") == 0)
            return false;

        return true;
    }

    int n = 0;

    while (_keywords[n] != NULL)
    {
        if (strcmp(word, _keywords[n]) == 0)
            return true;

        ++n;
    }

    return false;
}


