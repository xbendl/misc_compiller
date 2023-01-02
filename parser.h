/* Projekt: IFJ07
 * Subor: parser.h
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Hlavickovy subor k syntaktickemu analyzatoru.
 * Autor: xdanko00
 */

#ifndef PARSER_H
#define PARSER_H
     
#include "scanner.h"

typedef enum
{
  noError,
  lexError,
  syntaxError,
  semanticError,
  interpretError,
  otherError
} tError;


tError expressionParser (TSItem **);
tError parser();
tError declaration(tToken, TSItem *);
tError body(tToken, TSItem *);
tError statement(tToken, TSItem *);
tError bodyorstat(tToken, TSItem *);
tError ids(tToken, TSItem *);
tError expressions(tToken, TSItem *);
void printToken(tToken);

#endif
