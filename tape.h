/* Projekt: IFJ07
 * Subor: tape.h
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Hlavickovy soubor k tape.c (operace nad paskou)
 */

#ifndef TAPE_H
#define TAPE_H

#include "avl.h"

// Vycet pouzivanych instrukci
typedef enum 
{
    I_READ,                    // (I_READ, , , kam_nacitam) 
    I_WRITE,                   // (I_WRITE, , , co_vypisuji) 
    I_LAB,
    I_GOTO,                    // (I_GOTO, podminka_skoku, , kam_skocim)
    I_ASSIGN,                  // (I_ASSIGN, odkud_vezmu, , kam_priradim)
    I_ADD,
    I_SUB,
    I_MUL,
    I_DIV,
    I_POW,
    I_LESS,
    I_GREATER,
    I_LE,
    I_GE,
    I_EQ,
    I_NEQ,
    I_NOT,                      // (I_NEQ, normalni, ,negovana)
    I_SORT,                     // (I_SORT, neserazena, ,serazena)
    I_INT_TO_DOUBLE,            // (I_INT_TO_REAL, puvodni, , konvertovana)
} Instruction;

// struktura reprezentujici triadresny kod
typedef struct tapeItem
{
    Instruction instruction;
    TSItem *opr1;
    TSItem *opr2;
    TSItem *dest;
    struct tapeItem *nextItem;
} TapeItem;

// struktura uchovavajici odkaz na prvni a posledni polozku triadresneho kodu
typedef struct
{
    TapeItem *first;
    TapeItem *last;
} Tape;

/**
 * Inicializace pasky s polozkami triadresneho kodu
 * @param t Ukazatel na pasku
 */
void TapeInit();

/**
 * Uvolneni pasky
 * @param t Ukazatel na pasku
 */
void TapeDestroy();

/**
 * Vlozi na konec pasky novou polozku
 * @param t Ukazatel na pasku
 * @param item Ukazatel na vkladanou polozku
 */
TapeItem *TapeInsert(Instruction instruction, TSItem *opr1, TSItem *opr2, TSItem *dest);

// Globalni promenna ukazujici na pasku
Tape *T;

#endif
