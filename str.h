/* Projekt: IFJ07
 * Subor: str.h
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Hlavickovy subor k str.c (umoznuje pracovat s nekonecne
 * dlhymi retazcami)
 * Autor: xduric00
 */

#ifndef STR_H
#define STR_H

#define FIRST_ALLOC 16

// struktura pre nekonecne dlhy retazec
typedef struct
{
  char *text;
  unsigned length;
  unsigned sizeOfMemory;
} tString;

// operacie s retazcom
tString *strInit(void);
void strDealloc(tString *str);
void addCharToStr(tString *str, char c);
tString *strConcatWithStr(tString *str1, tString *str2);
tString *strConcatWithChar(tString *str1, char *str2);
tString *strCopyToStr(tString *str1, tString *str2);
tString *charCopyToStr(tString *str1, char *str2);
int strCompareStr(tString *str1, tString *str2);
int strCompareChar(tString *str1, char *str2);
unsigned returnLength(tString *str);
char *returnText(tString *str);

#endif
