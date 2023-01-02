/* Projekt: IFJ07
 * Subor: keyword_table.c
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Praca s tabulkou klucovych slov
 * Autor: xduric00
 */

#include "keyword_table.h"

/* Zisti ci ide o identifikator alebo klucove slovo.
 * V pripade, ze sa jedna o klucove slovo, vrati token
 * reprezentujuci dane kluc. slovo, inak identifikator.
 */
tToken sentKeyword(tString *string)
{
  int i = 0;

  while (i < KW)
  {
    // nasla som klucove slovo
    if (strCompareChar(string, keyword[i]) == 0)
      break;
    
    // klucove slovo tam nie je
    if (strCompareChar(string, keyword[i]) == -1)
      return tIdentifier;
    
    ++i;
  }

  // rozhodnutie, ktore klucove slovo to bolo
  switch (i)
  {
    case 0:
      return tTypeDouble;
    case 1:
      return tElse;
    case 2:
      return tIf;
    case 3:
      return tTypeInt;
    case 4:
      return tRead;
    case 5:
      return tSort;
    case 6:
      return tTypeString;
    case 7:
      return tWhile;
    case 8:
      return tWrite;
    default:
      return tIdentifier;
  }
}
