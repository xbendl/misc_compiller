/* Projekt: IFJ07
 * Subor: mergesort.c
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Algoritmus Merge sort
 */

#include <stdlib.h>
#include "mergesort.h"


/**
 * Provede serazeni algoritmem Merge sort
 */
void mergeSort(char *arrayIn, char *arrayOut,int left, int right)
{
    int middle = (left + right) / 2;
    int i;
    if(left >= right)
    {   // Rekurzivni volani probiha dokud neni vyrez dostatecne maly - 1 prvek
        return;
    }
    mergeSort(arrayIn, arrayOut, left, middle);      // Rekurzivni volani pro 1.polovinu aktualniho vyrezu pole
    mergeSort(arrayIn, arrayOut, middle + 1, right); // Rekurzivni volani pro 2.polovinu aktualniho vyrezu pole
    merge(arrayIn, arrayOut, left, right);           // Slouceni dvou serazenych polovin aktualniho vyrezu pole

    for(i = left; i <= right; i++) 
    {   // Kopiruje prave serazenou cast vystupniho pole do vstupniho pole
        arrayIn[i] = arrayOut[i];
    }
}

/**
 * Provede slouceni dvou serazenych casti aktualniho vyrezu pole (pomocna funkce pro funkci mergesort)
 */
void merge(char *arrayIn, char *arrayOut, int left, int right)
{
    int middle = (left + right) / 2;  // stred aktualniho vyrezu pole
    int j = left;                     // index ve vystupnim poli arrayOut
    int ind1 = left;                  // index znaku v levem vyrezu pole
    int ind2 = middle + 1;            // index znaku v pravem vyrezu pole

    while((ind1 <= middle) && (ind2 <= right))
    {   // Vybere se mensi znak z cela a vlozi se do vystupniho pole
        if(arrayIn[ind1] <= arrayIn[ind2])
        {   // Slucuji-li stejne hodnoty, zaradim nejprve levy prvek - Timto zajistim stabilitu
            arrayOut[j] = arrayIn[ind1];
            ind1++;
        }
        else
        {
           arrayOut[j] = arrayIn[ind2];
           ind2++;
        }

        j++;
    }

    while (ind1 <= middle) 
    {   // Vlozime zbyvajici znaky z leveho vyrezu (znaky praveho vyrezu jsou jiz ve vystupnim poli vsechny)
        arrayOut[j] = arrayIn[ind1];
        ind1++;
        j++; 
    }

    while (ind2 <= right)
    {   // Vlozime zbyvajici znaky z praveho vyrezu (znaky leveho vyrezu jsou jiz ve vystupnim poli vsechny)
        arrayOut[j] = arrayIn[ind2];
        ind2++;
        j++;
    }
}
