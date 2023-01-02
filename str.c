/* Projekt: IFJ07
 * Subor: str.c
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Operacie pre pracu s nekonecne dlhymi retazcami
 * Autor: xduric00
 */

#include <stdlib.h>
#include <string.h>

#include "str.h"
#include "test.h"

/* Alokuje miesto pre retazec s pociatocnou velkostou, length 
 * nastavi na 0 a text na prazdny retazec
 */
tString *strInit(void)
{
  tString *str;

  str = (tString *)malloc(sizeof(tString));
  str->text = (char *)malloc(sizeof(char) * FIRST_ALLOC);
  
  // chyba este test
  testAlloc((void *)(str->text));
  
  str->length = 0;
  str->text[0] = '\0';
  str->sizeOfMemory = FIRST_ALLOC;

  return str;
}

/* Uprace ak uz nie je retazec potrebny */
void strDealloc(tString *str)
{
  if(str != NULL)
  {
    free((void *)str->text);
    free((void *)str);
  }
  return;
}

/* Na koniec str prida znak c */
void addCharToStr(tString *str, char c)
{
  if (str->length + 1 == str->sizeOfMemory)
  {
    str->sizeOfMemory += FIRST_ALLOC;
    
    str->text = (char *)realloc((void *)str->text, str->sizeOfMemory);
    // este poriesit testovanie, temp pouzijem na odalokovanie
    // v pripade nedostaku pamati
    testAlloc((void *)(str->text));
  }
  str->text[str->length] = c;
  ++str->length;
  str->text[str->length] = '\0';
 
  return;
}

/* Skonkatenuje prvy a druhy retazec a vrati ukazatel na 
 * prvy retazec
 */
tString *strConcatWithStr(tString *str1, tString *str2)
{
  if (str1->length == 0 && str2->length == 0)
    return str1;
  
  if (str2->length > 0)
  {
    unsigned lengthBoth =  str1->length + str2->length + 1;

    if (str1->sizeOfMemory < lengthBoth)
      {
	str1->sizeOfMemory = lengthBoth;
	
	str1->text = (char *)realloc((void *)str1->text, str1->sizeOfMemory);
	// otestovat
	testAlloc((void *)(str1->text));
      }

    str1->length = lengthBoth - 1;
    strcat(str1->text, str2->text);
  }
  
  return str1;
}

/* Skonkatenuje k prvemu retazcu char * a vrati ukazatel na 
 * prvy retazec
 */
tString *strConcatWithChar(tString *str1, char *str2)
{
  unsigned lengthStr2 = strlen(str2);
  if (str1->length == 0 && lengthStr2 == 0)
    return str1;

  if (lengthStr2 > 0)
  {
    unsigned lengthBoth =  str1->length + lengthStr2 + 1;

    if (str1->sizeOfMemory < lengthBoth)
    {
      str1->sizeOfMemory = lengthBoth;
     
      str1->text = (char *)realloc((void *)str1->text, str1->sizeOfMemory);
      // chyba este test
      testAlloc((void *)(str1->text));
    }

    str1->length = lengthBoth - 1;
    strcat(str1->text, str2);
  }

  return str1;
}

/* Prekopiruje str2 do str1 */
tString *strCopyToStr(tString *str1, tString *str2)
{
  if (str1->sizeOfMemory < (str2->length + 1))
  {
    str1->sizeOfMemory = str2->length + 1;
    
    str1->text = (char *)realloc((void *)str1->text, str1->sizeOfMemory);
    // otestovat
    testAlloc((void *)(str1->text));
  }

  str1->length = str2->length;
  strcpy(str1->text, str2->text);

  return str1;
}

/* Prekopiruje str1 do str2 */
tString *charCopyToStr(tString *str1, char *str2)
{
  unsigned lengthStr2 = strlen(str2);
  if (str1->sizeOfMemory < (lengthStr2 + 1))
  {
    str1->sizeOfMemory = lengthStr2 + 1;
    
    str1->text = (char *)realloc((void *)str1->text, str1->sizeOfMemory);
    // otestovat
    testAlloc((void *)(str1->text));
  }

  str1->length = lengthStr2;
  strcpy(str1->text, str2);

  return str1;
}

/* Porovna dve retazce, pri ekvivalenci vracia 0, vrati -1
 * ak je str1 lexikograficky mensie ako str2, v opacnom pripade
 * vrati 1
 */
int strCompareStr(tString *str1, tString *str2)
{
  int i;
  for (i = 0; i < str1->length && i < str2->length; ++i)
    if (str1->text[i] < str2->text[i])
      return -1;
    else if (str1->text[i] > str2->text[i])
      return 1;
  
  if (str1->length < str2->length)
    return -1;
  else if (str1->length > str2->length)
    return 1;
  else
    return 0;
}
 
/* Porovna retazec a char *, pri ekvivalenci vracia 0, vrati -1
 * ak je str1 lexikograficky mesnie ako str2, v opacnom pripade
 * vrati 1
 */
int strCompareChar(tString *str1, char *str2)
{
  int i;
  for (i = 0; i < str1->length && str2[i] != '\0'; ++i)
    if (str1->text[i] < str2[i])
      return -1;
    else if (str1->text[i] > str2[i])
      return 1;
  
  if (i == str1->length && str2[i] == '\0')
    return 0;
  else if (i == str1->length)
    return -1;
  else
    return 1;
}

/* Vrati dlzku retazca */
unsigned returnLength(tString *str)
{
  return str->length;
}

/* Vrati text struktury */
char *returnText(tString *str)
{
  return str->text;
}
