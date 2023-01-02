/* Projekt: IFJ07
 * Subor: interpret.h
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Interpretace trojadresneho kodu
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "avl.h"
#include "tape.h"
#include "mergesort.h"
#include "parser.h"

/**
 * Provadi interpretaci instrukci
 */
int interpretation()
{
    TapeItem *item = T->first;      //inicializace 
    TapeItem *nextItem;

    int cmp, c;
    tString *userInput;
    TSItem *opr1, *opr2, *dest;     // Operandy v triadresnem kodu
    char *arrayOut;                 // Ukazatel na pomocny retezec pro Merge Sort
    int loadedInt;
    double loadedDouble;
    char *wrongChar = NULL;

    while(item != NULL)
    {   // Dokud je na pasce nejaka instrukce
        nextItem = item->nextItem;

        opr1 = item->opr1;
        opr2 = item->opr2;
        dest = item->dest;

        if(item->instruction != I_GOTO)
        {   // Pri instrukci I_GOTO nejsou hodnoty operandu typu TSItem
            if((opr1 != NULL) && (opr1->type == tStr) && (opr1->value.stringValue == NULL))
            {   // Obsah neinicializovych promennych typu string nastavime na prazdny retezec
                opr1->value.stringValue = strInit();
            }
            if((opr2 != NULL) && (opr2->type == tStr) && (opr2->value.stringValue == NULL))
            {
                opr2->value.stringValue = strInit();
            }
            if((dest != NULL) && (dest->type == tStr) && (dest->value.stringValue == NULL))
            {
                dest->value.stringValue = strInit();
            }
        }
        switch(item->instruction)
        {   // Vyber akce na zaklade aktualni instrukce
           case I_READ:
                // Instrukce nacteni hodnoty do promenne
                userInput = strInit();
                if(dest->type == tInt)
                {   // Nacitani do promenne typu int
                    while((c = getc(stdin)) != '\n')
                    {   // Nacitani uzivatelova vstupu do odradkovani
                        addCharToStr(userInput, c);
                    }
                    wrongChar = NULL;   // Ukazatel na prvni nekorektni znak
                    loadedInt = strtol(userInput->text, &wrongChar, 0);

                    if(*wrongChar != '\0')
                    {
                        strDealloc(userInput);
                        return interpretError;
                    }
                    dest->value.intValue = loadedInt;
                    //printf("# I_READ (int): loaded %s, convert %d, wrongChar %c\n", userInput->text, loadedInt, *wrongChar);
                }
                else if(dest->type == tDouble)
                {   // Nacitani do promenne typu double
                    while((c = getc(stdin)) != '\n')
                    {   // Nacitani uzivatelova vstupu
                        addCharToStr(userInput, c);
                    }
                    wrongChar = NULL;   // Ukazatel na prvni nekorektni znak
                    loadedDouble = strtod(userInput->text, &wrongChar);

                    if(*wrongChar != '\0')
                    {
                        strDealloc(userInput);
                        return interpretError;
                    }
                    dest->value.doubleValue = loadedDouble;
                    //printf("# I_READ (double): loaded %s, convert %f, wrongChar %c\n", userInput->text, loadedDouble, *wrongChar);
                }
                else
                {   // Nacitani do promenne typu string
                    while((c = getc(stdin)) != '\n')
                    {   // Nacitani uzivatelova vstupu
                        addCharToStr(userInput, c);
                    }
                    dest->value.stringValue = strCopyToStr(dest->value.stringValue, userInput);
                    //printf("# I_READ (string): Precteny vstup (string): %s\n", userInput->text);
                }
                strDealloc(userInput);
                break;
            case I_WRITE:
                // Instrukce nacteni hodnoty do promenne
                if(dest->type == tInt)
                {
                    //printf("# I_WRITE: Vypis (int): %d\n", dest->value.intValue);
                    printf("%d", dest->value.intValue);
                }
                else if(dest->type == tDouble)
                {
                    //printf("# I_WRITE: Vypis (double): %f\n", dest->value.doubleValue);
                    printf("%f", dest->value.doubleValue);
                }
                else
                {
                    //printf("# I_WRITE: Vypis (double): %s\n", dest->value.stringValue);
                    printf("%s", dest->value.stringValue->text);
                }
                break;

            case I_ADD:
                // Instrukce scitani dvou operandu
                if(dest->type == tInt)
                {
                    dest->value.intValue = opr1->value.intValue + opr2->value.intValue;
                    //printf("# I_ADD: Vypocitana hodnota (int): %d\n", dest->value.intValue);
                }
                else if(dest->type == tDouble)
                {
                    dest->value.doubleValue = opr1->value.doubleValue + opr2->value.doubleValue;
                    //printf("# I_ADD: Vypocitana hodnota (double): %f\n", dest->value.doubleValue);
                }
                else
                {
                    strCopyToStr(dest->value.stringValue, opr1->value.stringValue);      // Nejprve nakopirujeme do vysledneho retezec z prvniho operandu
                    strConcatWithStr(dest->value.stringValue, opr2->value.stringValue);  // Nasledne konkatenujeme takto ziskany retezec s retezcem z druheho operandu
                    //printf("# I_ADD: Konkatenace retezcu %s\n", dest->value.stringValue->text);
                }
                break;
            case I_SUB:
                // Instrukce odcitani dvou operandu
                if(dest->type == tInt)
                {
                    dest->value.intValue = opr1->value.intValue - opr2->value.intValue;
                    //printf("# I_SUB: Vypocitana hodnota (int): %d\n", dest->value.intValue);
                }
                else
                {
                    dest->value.doubleValue = opr1->value.doubleValue - opr2->value.doubleValue;
                    //printf("# I_SUB: Vypocitana hodnota (double): %f\n", dest->value.doubleValue);
                }
                break;

            case I_MUL:
                // Instrukce nasobeni dvou operandu
                if(dest->type == tInt)
                {
                    dest->value.intValue = opr1->value.intValue * opr2->value.intValue;
                    //printf("# I_MUL: Vypocitana hodnota (int): %d\n", dest->value.intValue);
                }
                else
                {
                    dest->value.doubleValue = opr1->value.doubleValue * opr2->value.doubleValue;
                    //printf("# I_MUL: Vypocitana hodnota (double): %f\n", dest->value.doubleValue);
                }
                break;
            case I_DIV:
                // Instrukce deleni dvou operandu
                if(opr2->value.doubleValue == 0)
                {   // Chyba - deleni nulou
                    //printf("# I_DIV: Deleni nulou.\n");
                    return interpretError;
                }

                dest->value.doubleValue = opr1->value.doubleValue / opr2->value.doubleValue;
                //printf("# I_DIV: Vypocitana hodnota (vzdy double): %f\n", dest->value.doubleValue);
                break;
            case I_POW:
                // Instrukce mocnina
                dest->value.doubleValue = pow(opr1->value.doubleValue, opr2->value.doubleValue);
                if(isnan(dest->value.doubleValue) || isinf(dest->value.doubleValue))
                {   // Chyba - Vysledek neni cislo, pripadne je mimo rozsah
                    //printf("# I_POW: Vypocitana hodnota (chyba): %f\n", dest->value.doubleValue);
                    return interpretError;
                }
                else
                {
                    //printf("# I_POW: Vypocitana hodnota (vzdy double): %f\n", dest->value.doubleValue);
                }
                break;
            case I_INT_TO_DOUBLE:
                // Instrukce konverze promenne z int na double
                dest->value.doubleValue = (double) opr1->value.intValue;
                //printf("# I_INT_TO_REAL: Konvertovana hodnota %d (int) na %f (double)\n", dest->value.intValue, dest->value.doubleValue);
                break;
            case I_ASSIGN:
                // Instrukce prirazeni
                if(dest->type == tInt)
                {
                    dest->value.intValue = opr1->value.intValue;
                    //printf("# I_ASSIGN: Prirazena hodnota %d\n", dest->value.intValue);
                }
                else if(dest->type == tDouble)
                {
                    dest->value.doubleValue = opr1->value.doubleValue;
                    //printf("# I_ASSIGN: Prirazena hodnota %f\n", dest->value.doubleValue);
                }
                else
                {
                    dest->value.stringValue = strCopyToStr(dest->value.stringValue, opr1->value.stringValue);
                    //printf("# I_ASSIGN: Prirazena hodnota %s\n", dest->value.stringValue->text);
                }
                break;
            case I_SORT:
                // Serazeni retezce podle ordinalni hodnoty znaku
                if((arrayOut = malloc(opr1->value.stringValue->length + 1)) == NULL)
                {   // Nedostatek pameti pro vytvoreni pomocneho pole pro metodu Merge sort
                    exit(otherError);
                }
                arrayOut[opr1->value.stringValue->length] = '\0';

                mergeSort(opr1->value.stringValue->text, arrayOut, 0, opr1->value.stringValue->length - 1);

                //printf("# I_SORT: Serazeno na %s\n", arrayOut);
                free(arrayOut);      // Uvolneni pomocneho pole
                break;
            case I_LESS:
                // Instrukce porovnani - Je-li op1 < op2 vraci 1, jinak 0
                if(opr1->type == tInt)
                {
                    if(opr1->value.intValue < opr2->value.intValue)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                }
                else if(opr1->type == tDouble)
                {
                    if(opr1->value.doubleValue < opr2->value.doubleValue)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                }
                else
                {
                    if(strCompareStr(opr1->value.stringValue, opr2->value.stringValue) == -1)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                }
                //printf("# I_LESS: Vysledek porovnani: %d\n", dest->value.intValue);
                break;
            case I_GREATER:
                // Instrukce porovnani - Je-li op1 > op2 vraci 1, jinak 0
                if(opr1->type == tInt)
                {
                    if(opr1->value.intValue > opr2->value.intValue)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                }
                else if(opr1->type == tDouble)
                {
                    if(opr1->value.doubleValue > opr2->value.doubleValue)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                }
                else
                {
                    if(strCompareStr(opr1->value.stringValue, opr2->value.stringValue) == 1)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                }
                //printf("# I_GREATER: Vysledek porovnani: %d\n", dest->value.intValue);
                break;
            case I_LE:
                // Instrukce porovnani - Je-li op1 <= op2 vraci 1, jinak 0
                if(opr1->type == tInt)
                {
                    if(opr1->value.intValue <= opr2->value.intValue)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                    //printf("# I_LE: opr1(%d)=%d, op2r(%d)=%d\n",opr1->type, opr1->value.intValue,opr2->type, opr2->value.intValue );
                }
                else if(opr1->type == tDouble)
                {
                    if(opr1->value.doubleValue <= opr2->value.doubleValue)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                    //printf("# I_LE: opr1(%d)=%f, op2r(%d)=%f\n",opr1->type, opr1->value.doubleValue,opr2->type, opr2->value.doubleValue );
                }
                else
                {
                    cmp = strCompareStr(opr1->value.stringValue, opr2->value.stringValue);
                    if(cmp == -1 || cmp == 0)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                }
                //printf("# I_LE: Vysledek porovnani: %d\n", dest->value.intValue);
                break;
            case I_GE:
                // Instrukce porovnani - Je-li op1 >= op2 vraci 1, jinak 0
                if(opr1->type == tInt)
                {
                    if(opr1->value.intValue >= opr2->value.intValue)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                    //printf("# I_LE: opr1(%d)=%d, op2r(%d)=%d\n",opr1->type, opr1->value.intValue,opr2->type, opr2->value.intValue );
                }
                else if(opr1->type == tDouble)
                {
                    if(opr1->value.doubleValue >= opr2->value.doubleValue)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                    //printf("# I_GE: opr1(%d)=%f, op2r(%d)=%f\n",opr1->type, opr1->value.doubleValue,opr2->type, opr2->value.doubleValue);
                }
                else
                {
                    cmp = strCompareStr(opr1->value.stringValue, opr2->value.stringValue);
                    if(cmp == 1 || cmp == 0)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                }
                //printf("# I_GE: Vysledek porovnani: %d\n", dest->value.intValue);
                break;
            case I_EQ:
                // Instrukce porovnani - Je-li op1 = op2 vraci 1, jinak 0
                if(opr1->type == tInt)
                {
                    if(opr1->value.intValue == opr2->value.intValue)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                    //printf("# I_EQ: opr1(%d)=%d, op2r(%d)=%d\n",opr1->type, opr1->value.intValue,opr2->type, opr2->value.intValue );
                }
                else if(opr1->type == tDouble)
                {
                    if(opr1->value.doubleValue == opr2->value.doubleValue)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                    //printf("# I_EQ: opr1(%d)=%f, op2r(%d)=%f\n",opr1->type, opr1->value.doubleValue,opr2->type, opr2->value.doubleValue);
                }
                else
                {
                    if(strCompareStr(opr1->value.stringValue, opr2->value.stringValue) == 0)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                }
                //printf("# I_EQ: Vysledek porovnani: %d\n", dest->value.intValue);
                break;
            case I_NEQ:
                // Instrukce porovnani - Je-li op1 != op2 vraci 1, jinak 0
                if(opr1->type == tInt)
                {
                    if(opr1->value.intValue != opr2->value.intValue)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                    //printf("# I_NEQ: opr1(%d)=%d, op2r(%d)=%d\n",opr1->type, opr1->value.intValue,opr2->type, opr2->value.intValue );
                }
                else if(opr1->type == tDouble)
                {
                    if(opr1->value.doubleValue != opr2->value.doubleValue)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                    //printf("# I_NEQ: opr1(%d)=%f, op2r(%d)=%f\n",opr1->type, opr1->value.doubleValue,opr2->type, opr2->value.doubleValue);
                }
                else
                {
                    if(strCompareStr(opr1->value.stringValue, opr2->value.stringValue) != 0)
                        dest->value.intValue = 1;
                    else
                        dest->value.intValue = 0;
                }
                //printf("# I_NEQ: Vysledek porovnani: %d\n", dest->value.intValue);
                break;
            case I_NOT:
                // Instrukce negace - Je-li op1 = 0, vraci 1, jinak 0
                if(opr1->value.intValue == 0)
                    dest->value.intValue = 1;
                else
                    dest->value.intValue = 0;
                //printf("# I_NOT: Vysledek negace: %d\n", dest->value.intValue);
                break;
            case I_GOTO:
                // Instrukce skoku
                if(opr1 == NULL)
                {   // Je-li 1.operand NULL, jde-li o nepodmineny skok
                    nextItem = (TapeItem *) item->dest;   // Adresu pristi instrukce ziskam z pretypovaneho 3.operandu triadresneho kodu
                    //printf("# I_GOTO: Skocim z nuselaku\n");
                }
                else
                {
                    if(opr1->value.intValue)
                    {   // Podminka skoku splnena
                        nextItem = (TapeItem *) item->dest;  // Adresu pristi instrukce ziskam z pretypovaneho 3.operandu triadresneho kodu
                        //printf("# I_GOTO: Skocim z nuselaku\n");
                    }
                    else
                    {   // Podminka skoku nesplnena
                        nextItem = item->nextItem;  //pokracovani bez skoku
                        //printf("# I_GOTO: Rozmyslel jsem si to, jsem slaboch.\n");
                    }
                }
                break;
            case I_LAB:  //navesti pro skoky?
                break;
        }
        item = nextItem;  //aktualizace dalsi instrukce k vykonani
    }
    return noError;
}
