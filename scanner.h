/* Projekt: IFJ07
 * Subor: scanner.h
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Hlavickovy subor k lexikalnemu analyzatoru.
 * Autor: xduric00
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "avl.h"

typedef enum
{
  // chybny token
  tInvalidToken,
	
  // chyby nad double
  tInvalidDecPart, // chybna desetinna cast, napr. 1.nic
  tInvalidExpPart, // chybna cast exponentu, napr. 1e@
  tInvalidSign,    // chyba za znamienkom, napr. 1e+@
	
  // chyby nad stringom
  tInvalidString,  // ordinalna hodnota < 32
  tInvalidEscape,  // chybna escape sekvencia
	
  // ostatne chyby
  tInvalidChar,    // nekorektny znak na vstupe
  tInvalidEOF,     // EOF v komentari
	
  // platne tokeny konstant
  tIdentifier,
  tLiteralInt,
  tLiteralDouble,
  tLiteralString,
	
  // operatory
  tOpPlus,
  tOpMinus,
  tOpMultiply,
  tOpDivide,
  tOpPow,
  tOpAssignment,
  tOpEqual,
  tOpNonEqual,
  tOpLess,
  tOpGreater,
  tOpLessEqual,
  tOpGreaterEqual,
	
  // klucove slova
  tTypeInt,
  tTypeDouble,
  tTypeString,
  tIf,
  tElse,
  tWhile,
  tRead,
  tWrite,
  tSort,
	
  // ostatne
  tEOF,              // koniec suboru
  tDollar,           // znak $
  tComma,            // znak ,
  tSemicollon,       // znak ;
  tLeftParenthesis,  // znak (
  tRightParenthesis, // znak )
  tBlockStart,       // znak {
  tBlockEnd          // znak }
} tToken;

FILE *fr;

// samotny automat
tToken getNextToken(TSItem **);

// rozlisuje klucove slova a identifiktory
tToken sentKeyword(tString *string);

#endif
