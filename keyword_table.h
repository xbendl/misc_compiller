/* Projekt: IFJ07
 * Subor: keyword_table.h
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Hlavickovy subor k tabulke klucovych slov.
 * Autor: xduric00
 */

#ifndef KEYWORD_TABLE_H
#define KEYWORD_TABLE_H

#include "scanner.h" 
#include "str.h"

// pocet klucovych slov
#define KW 9

// zoradena tabulka klucovych slov
char *keyword[KW] = {"double", "else", "if", "int", "read", "sort", "string", "while", "write"};

tToken sentKeyword(tString *string);

#endif
