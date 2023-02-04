#include "wrparser.h"

#include "print.h"
#include <cfile.h>
#include <ctype.h>

WordParser* wp_new(WordParseFunc func)
{
    WordParser *parser = (WordParser*) malloc(sizeof(WordParser));
    parser->list = cstrlist_new_size(128);
    parser->func = func;

    return parser;
}

void wp_free(WordParser *parser)
{
    if (!parser)
        return;

    cstrlist_free(parser->list);

    free(parser);
}

bool wp_readfile(WordParser *parser, const char *filepath)
{
    if (!parser->func || !file_exists(filepath))
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
            if (parser->func(c_str(word)))
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
        if (isalpha(*p) || *p =='_')
        {
            char *word = p;
            while (isalnum(*p) || *p =='_')
                ++p;

            cstr_clear(result);
            cstr_append_len(result, word, p - word);

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


