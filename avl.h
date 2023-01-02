/* Projekt: IFJ07
 * Subor: avl.h
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Hlavickovy subor k avl.c (implementacia tabulky symbolov)
 * Autor: xduric00
 */

#ifndef AVL_H
#define AVL_H

#include "str.h"

// typ identifikatora alebo konstanty
typedef enum 
{
  tNone, 
  tInt, 
  tDouble, 
  tStr
} TSItemType;

// hodnota identifikatora alebo konstanty
typedef union 
{
  int intValue; 
  double doubleValue; 
  tString *stringValue;
} TSItemValue;

// struktura avl stromu
typedef struct TSItemStruct
{
  tString *name;     // meno identifikatora alebo konstanty, kluc AVL
  TSItemType type;   // typ identifikatora alebo konstanty
  TSItemValue value; // hodnota
  int balance;
  struct TSItemStruct *left;
  struct TSItemStruct *right;
} TSItem;

TSItem * TSItemRoot;

// operacie nad AVL stromom
void initTS(TSItem **root);
void rotateLL(TSItem **root, TSItem *parentCritic, TSItem *critic);
void rotateRR(TSItem **root, TSItem *parentCritic, TSItem *critic);
void rotateDLR(TSItem **root, TSItem *parentCritic, TSItem *critic);
void rotateDRL(TSItem **root, TSItem *parentCritic, TSItem *critic);
TSItem * insertToTS(TSItem **root, tString *key, TSItemType type, TSItemValue value);
void deallocTS(TSItem **root);
TSItem *searchTS(TSItem *root, tString *key);

// ostatne operacie suviasiace s vkladanim do TS
TSItem *generator(char c, TSItemType type, TSItemValue value);
void itostr(int i, tString *text);

#endif
