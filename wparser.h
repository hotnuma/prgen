#ifndef WPARSER_H
#define WPARSER_H

#include <cstringlist.h>

typedef bool (*WordTestFunc) (const char*);

typedef struct _WordParser WordParser;

struct _WordParser
{
    CStringList *list;
    WordTestFunc test;
};

WordParser* wp_new(WordTestFunc func);
void wp_free(WordParser *parser);

#define WordParserAuto GC_CLEANUP(_freeWordParser) WordParser
GC_UNUSED static inline void _freeWordParser(WordParser **parser)
{
    wp_free(*parser);
}

bool wp_readdir(WordParser *parser, const char *indir);
bool wp_readfile(WordParser *parser, const char *filepath);
void wp_sort(WordParser *parser);

bool get_word(char **start, CString *result);
bool word_test(const char *word);

#endif // WPARSER_H


