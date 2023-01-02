/* Projekt: IFJ07
 * Subor: mergesort.h
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Hlavickovy soubor k mergesort.c (implementace algoritmu Merge sort)
 */

#ifndef MERGESORT_H
#define MERGESORT_H

/**
 * Provede serazeni algoritmem Merge sort
 * @param arrayIn Neserazene pole znaku
 * @param arrayOut Serazene pole pole znaku
 * @param left Levy kraj vyrezu pole
 * @param right Pravy kraj vyrezu pole
 */
void mergeSort(char arrayIn[], char arrayOut[],int left, int right);

/**
 * Provede slouceni dvou serazenych casti aktualniho vyrezu pole (pomocna funkce pro funkci mergesort)
 * @param arrayIn Neserazene pole znaku
 * @param arrayOut Serazene pole pole znaku
 * @param left Levy kraj vyrezu pole
 * @param right Pravy kraj vyrezu pole
 */
void merge(char arrayIn[], char arrayOut[], int left, int right);

#endif
