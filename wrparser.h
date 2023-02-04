#ifndef WRPARSER_H
#define WRPARSER_H

#include <cstringlist.h>

typedef bool (*WordParseFunc) (const char*);

typedef struct
{
    CStringList *list;
    WordParseFunc func;

} WordParser;

WordParser* wp_new(WordParseFunc func);
void wp_free(WordParser *parser);

#define WordParserAuto GC_CLEANUP(_freeWordParser) WordParser
GC_UNUSED static inline void _freeWordParser(WordParser **parser)
{
    wp_free(*parser);
}

bool wp_readfile(WordParser *parser, const char *filepath);
void wp_sort(WordParser *parser);

bool get_word(char **start, CString *result);

#endif // WRPARSER_H


