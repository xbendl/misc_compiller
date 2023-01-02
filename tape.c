/* Projekt: IFJ07
 * Subor: tape.c
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Operace nad paskou
 */

#include <stdlib.h>
#include <stdio.h>
#include "tape.h"
#include "parser.h"

/**
 * Inicializace pasky s polozkami triadresneho kodu
 */
void TapeInit()
{
    if((T = malloc(sizeof(Tape))) == NULL)
    {   // Nedostatek pameti
        exit(otherError);
    }
    T->first = NULL;
    T->last = NULL;
    TapeItem *firstItem = TapeInsert(I_LAB, NULL, NULL, NULL);   // Vlozeni pomocne instrukce I_LAB na zacatek seznamu
    T->first = firstItem;
    T->last = firstItem;
}

/**
 * Uvolneni pasky
 */
void TapeDestroy()
{
    TapeItem *item = T->first;

    while(T->first != NULL)
    {   // Pruchod paskou
        item = T->first;
        T->first = T->first->nextItem;
        free(item);   // Uvolneni aktualne prvni polozky na pasce
    }
    free(T);          // Uvolneni globalni promenne ukazujici na pasku
}

/**
 * Vlozi na konec pasky novou polozku
 */
TapeItem *TapeInsert(Instruction instruction, TSItem *opr1, TSItem *opr2, TSItem *dest)
{
    TapeItem *newItem;
    if((newItem = malloc(sizeof(TapeItem))) == NULL)
    {   // Nedostatek pameti
        exit(otherError);
    }
    newItem->instruction = instruction;
    newItem->opr1 = opr1;
    newItem->opr2 = opr2;
    newItem->dest = dest;
    newItem->nextItem = NULL;

    if(T->last != NULL)
    {   // Poslednimu prvku nastavim odkaz nextItem na vkladany prvek newItem
        T->last->nextItem = newItem;
    }
    if(T->first == NULL)
    {   // Je-li paska prazdna, nastavim ukazatel na prvni prvek na newItem
        T->first = newItem;
    }
    T->last = newItem;  // Aktualizace odkazu na posledni prvek

    return newItem;
}
