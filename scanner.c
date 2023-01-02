/* Projekt: IFJ07
 * Subor: scanner.c
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Lexikalny analyzator.
 * Autor: xduric00
 */

#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

#include "str.h"
#include "scanner.h"

// stavy konecneho automatu
typedef enum
{
  sInit,
  sIdentifier,
  sLiteralInt,
  sLiteralDouble,
  sDecimalPart,
  sExponentPart,
  sSignPart,
  sEscapeSequence,
  sChar,
  sAssigment,
  sEqual,
  sExclamation,
  sGreater,
  sLess,
  sDivide,
  sBeginComentar,
  sEndComentar
} state;

/* Vracia token a ukazatel do tabulky symbolov */
tToken getNextToken(TSItem ** item)
{
  state currentState = sInit;
  state nextState;
  int c;
  tString *container = strInit(); 
  TSItemValue initValue;

  while (true)
  {
    switch(currentState)
    {
      case sInit:
	c = getc(fr);

	if (isspace(c))
	  nextState = sInit;

        else if (isalpha(c))
        {
	  addCharToStr(container, c);
	  nextState = sIdentifier;
	}

	else if (isdigit(c))
        {
	  addCharToStr(container, c);
	  nextState = sLiteralInt;
	}

	else if (c == '"')
	  nextState = sChar;
	else if (c == '!')
	  nextState = sExclamation;
	else if (c == '<')
	  nextState = sLess;
	else if (c == '>')
	  nextState = sGreater;
	else if (c == '=')
	  nextState = sAssigment;
	else if (c == '/')
	  nextState = sDivide;
	else if (c == '{')
	{
	  strDealloc(container);
	  return tBlockStart;
	}
	else if (c == '}')
	{
	  strDealloc(container);
	  return tBlockEnd;
	}
	else if (c == ',')
	{
	  strDealloc(container);
	  return tComma;
	}
	else if (c == ';')
	{
	  strDealloc(container);
	  return tSemicollon;
	}
	else if (c == '(')
	{
	  strDealloc(container);
	  return tLeftParenthesis;
	}
	else if (c == ')')
	{
	  strDealloc(container);
	  return tRightParenthesis;
	}
	else if (c == '^')
	{
	  strDealloc(container);
	  return tOpPow;
	}
	else if (c == '+')
	{
	  strDealloc(container);
	  return tOpPlus;
	}
	else if (c == '-')
	{
	  strDealloc(container);
	  return tOpMinus;
	}
	else if (c == '*')
	{
	  strDealloc(container);
	  return tOpMultiply;
	}
	else if (c == '$')
	{
	  strDealloc(container);
	  return tDollar;
	}
	else if (c == EOF)
	{
	  strDealloc(container);
	  return tEOF;
	}
	else
	{
	  strDealloc(container);
	  return tInvalidToken;
	}
	break;

      case sIdentifier:
	c = getc(fr);
	if (isalnum(c))
        {
	  addCharToStr(container, c);
	  nextState = sIdentifier;
	}
	else
        {
	  ungetc(c, fr);
	  tToken token = sentKeyword(container);
	  // ak to je klucove slovo, len sa vrati
          if (token != tIdentifier)
	  {
	    strDealloc(container);
	    return token;
	  }
	  // ak identifikator, musim ulozit do TS
          initValue.intValue = 0;
          *item = insertToTS(&TSItemRoot, container, tNone, initValue);
	  strDealloc(container);
	  return token;
	}
	break;

      case sLiteralInt:
	c = getc(fr);
	if (isdigit(c))
	{
	  addCharToStr(container, c);
	  nextState = sLiteralInt;
	}
	else if (c == '.')
	{
	  addCharToStr(container, c);
	  nextState = sDecimalPart;
	}
	else if (c == 'e' || c == 'E')
	{
	  addCharToStr(container, c);
	  nextState = sExponentPart;
	}
	else
        {
	  ungetc(c, fr);
          initValue.intValue = atoi(container->text);
          *item = generator('$', tInt, initValue);
	  strDealloc(container);
          return tLiteralInt;
	}
	break;

      case sDecimalPart:
	c = getc(fr);
	if (isdigit(c))
        {
	  addCharToStr(container, c);
	  nextState = sLiteralDouble;
	}
	else
	{
	  strDealloc(container);
	  return tInvalidToken;
	}
	break;

      case sExponentPart:
	c = getc(fr);
	if (isdigit(c))
	{
	  addCharToStr(container, c);
	  nextState = sLiteralDouble;
	}
	else if (c == '+' || c == '-')
	{
	  addCharToStr(container, c);
	  nextState = sSignPart;
	}
	else
	{
	  strDealloc(container);
	  return tInvalidToken;
	}
	break;
	
      case sSignPart:
	c = getc(fr);
	if (isdigit(c))
	{
	  addCharToStr(container, c);
	  nextState = sLiteralDouble;
	}
	else
	{
	  strDealloc(container);
	  return tInvalidToken;
	}
	break;

      case sLiteralDouble:
	c = getc(fr);
	if (isdigit(c))
	{
	  addCharToStr(container, c);
	  nextState = sLiteralDouble;
	}
	else if (c == 'e' || c == 'E')
	{
	  addCharToStr(container, c);
	  nextState = sExponentPart;
	}
	else
	{
	  ungetc(c, fr);
          initValue.doubleValue = strtod(container->text, NULL);
          *item = generator('$', tDouble, initValue);
	  strDealloc(container);
	  return tLiteralDouble;
	}
	break;

      case sChar:
	c = getc(fr);
	if (c == '"')
	{
	  // uloz do TS, retazec, typ, vymysli meno
          initValue.stringValue = strInit();
          strCopyToStr(initValue.stringValue, container);
          *item = generator('$', tStr, initValue);
	  strDealloc(container);
	  return tLiteralString;
	}
	else if (c == '\\')
	{ 
	  nextState = sEscapeSequence;
	}
	else if (isprint(c))
	{
	  addCharToStr(container, c);
	  nextState = sChar;
	}
	else
	{
	  strDealloc(container);
	  return tInvalidToken;
	}
	break;

      case sEscapeSequence:
	c = getc(fr);
	if (c == 'n')
	{
	  addCharToStr(container, '\n');
	  nextState = sChar;
	}
	else if (c == 't')
	{
	  addCharToStr(container, '\t');
	  nextState = sChar;
	}
	else if (c == '\\')
	{
	  addCharToStr(container, '\\');
	  nextState = sChar;
	}
	else if (c == '"')
	{
	  addCharToStr(container, '\"');
	  nextState = sChar;
	}
	else
	{
	  strDealloc(container);
	  return tInvalidToken;
	}
	break;
	  
      case sAssigment:
	c = getc(fr);
	if (c == '=')
	{
	  strDealloc(container);
	  return tOpEqual;
	}
	else
        {
	  ungetc(c, fr);
	  strDealloc(container);
	  return tOpAssignment;
	}
	break;

      case sExclamation:
	c = getc(fr);
	if (c == '=')
	{
	  strDealloc(container);
	  return tOpNonEqual;
	}
	else
	{
	  strDealloc(container);
	  return tInvalidToken;
	}
	break;

      case sGreater:
	c = getc(fr);
	if (c == '=')
	{
	  strDealloc(container);
	  return tOpGreaterEqual;
	}
	else
        {
	  ungetc(c, fr);
	  strDealloc(container);
	  return tOpGreater;
	}
	break;

      case sLess:
	c = getc(fr);
	if (c == '=')
	{
	  strDealloc(container);
	  return tOpLessEqual;
	}
	else
        {
	  ungetc(c, fr);
	  strDealloc(container);
	  return tOpLess;
	}
	break;

      case sDivide:
	c = getc(fr);
	if (c == '*')
	  nextState = sBeginComentar;
	else
        {
	  ungetc(c,fr);
	  strDealloc(container);
	  return tOpDivide;
	}
	break;

      case sBeginComentar:
	c = getc(fr);
	if (c == EOF)
	{
	  strDealloc(container);
	  return tInvalidToken;
	}
	else if (c == '*')
	  nextState = sEndComentar;
	else 
	  nextState = sBeginComentar;
	break;

      case sEndComentar:
	c = getc(fr);
	if (c == '/')
	  nextState = sInit;
	else if (c == EOF)
	{
	  strDealloc(container);
	  return tInvalidToken;
	}
	else if (c == '*')
	  nextState = sEndComentar;
	else
	  nextState = sBeginComentar;
	break;
       
      default:
	break;
    }

    currentState = nextState;
  }

}
