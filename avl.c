/* Projekt: IFJ07
 * Subor: avl.c
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Subor s operaciami  nad tabulkou symbolov (AVL strom)
 * Autor: xduric00
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "avl.h"
#include "str.h"
#include "test.h"

/* Inicializacia tabulky symbolov */
void initTS(TSItem **root)
{
  (*root) = NULL;
  return;
}

/* Jednoducha rotacia LL */
void rotateLL(TSItem **root, TSItem *parentCritic,TSItem *critic)
{
  TSItem *A;
  TSItem *B;
 
  A = critic;
  B = A->left;
  
  A->left = B->right;
  B->right = A;

  if (parentCritic == NULL)
    (*root) = B;
  else
  {
    if (parentCritic->right == critic)
      parentCritic->right = B;
    else
      parentCritic->left = B;
  }
  
  A->balance = 0;
  B->balance = 0;

  return;
}

/* Jednoducha rotacia RR */
void rotateRR(TSItem **root, TSItem *parentCritic,TSItem *critic)
{
  TSItem *A;
  TSItem *B;
 
  A = critic;
  B = A->right;
  
  A->right = B->left;
  B->left = A;

  if (parentCritic == NULL)
    (*root) = B;
  else
  {
    if (parentCritic->right == critic)
      parentCritic->right = B;
    else
      parentCritic->left = B;
  }
  
  A->balance = 0;
  B->balance = 0;

  return;
}

/* Dvojita rotacia DLR  */
void rotateDLR(TSItem **root, TSItem *parentCritic,TSItem *critic)
{
  TSItem *A;
  TSItem *B;
  TSItem *C;

  A = critic;
  B = A->left;
  C = B->right;

  if (A == NULL || B == NULL || C == NULL)
    return;

  B->right = C->left;
  A->left = C->right;
  C->left = B;
  C->right = A;

  if (parentCritic == NULL)
    (*root) = C;
  else
  {
    if (parentCritic->right == critic)
      parentCritic->right = C;
    else
      parentCritic->left = C;
  }
 
  if (C->balance == 1)
  {
    A->balance = 0;
    B->balance = -1;
  }
  else if (C->balance == -1)
  {
    A->balance = 1;
    B->balance = 0;
  }
  else
  {
    A->balance = 0;
    B->balance = 0;
  }
  
  C->balance = 0;
  return;
}

/* Dvojita rotacia DRL  */
void rotateDRL(TSItem **root, TSItem *parentCritic,TSItem *critic)
{
  TSItem *A;
  TSItem *B;
  TSItem *C;

  A = critic;
  B = A->right;
  C = B->left;

  if (A == NULL || B == NULL || C == NULL)
    return;

  B->left = C->right;
  A->right = C->left;
  C->left = A;
  C->right = B;

  if (parentCritic == NULL)
    (*root) = C;
  else
  {
    if (parentCritic->right == critic)
      parentCritic->right = C;
    else
      parentCritic->left = C;
  }
 
  // toto si overit ako na tu balanciu
  if (C->balance == 1)
  {
    A->balance = -1;
    B->balance = 0;
  }
  else if (C->balance == -1)
  {
    A->balance = 0;
    B->balance = 1;
  }
  else
  {
    A->balance = 0;
    B->balance = 0;
  }
  
  C->balance = 0;
  return;
}

/* Vkladanie do tabulky symbolov */
TSItem *insertToTS(TSItem **root, tString *key, TSItemType type, TSItemValue value)
{
  TSItem *actNode = (*root);
  TSItem *parentActNode = NULL;
  TSItem *criticNode = (*root);
  TSItem *parentCriticNode = NULL;
  TSItem *temp;

  //vytvorenie polozky
  if ((*root) == NULL)
  {
    (*root) = (TSItem *)malloc(sizeof(TSItem));
    //test
    testAlloc((void *)(*root));
    (*root)->name = strInit();
    strCopyToStr((*root)->name, key);
    (*root)->type = type;
    (*root)->value = value;
    (*root)->balance = 0;
    (*root)->left = NULL;
    (*root)->right = NULL;
    return (*root);
  }

  // najdenie uzlu
  while (true)
  { 
    // ak actNode->name == key len vratim ukazatel (neprepisujem data)
    // doslo by k prepisaniu dat syntaktickeho analyzatora :-)
    if (strCompareStr(actNode->name, key) == 0)
      return actNode;

    // hladanie smerom do lava, actNode->name > key
    else if (strCompareStr(actNode->name, key) == 1)
    {
      if (actNode->left == NULL)
	break;
      
      if (actNode->balance != 0)
      {
       	criticNode = actNode;
	parentCriticNode = parentActNode;
      }
      
      parentActNode = actNode;
      actNode = actNode->left;
    }

    // hladanie smerom do prava, actNode->name < key
    else 
    {
      if (actNode->right == NULL)
	break;
      
      if (actNode->balance != 0)
      {
	criticNode = actNode;
	parentCriticNode = parentActNode;
      }

      parentActNode = actNode;
      actNode = actNode->right;
    }

  } // koniec while(true)

  // zacne od kritickeho uzla, zmeni balanciu a nasledne vlozi
  if (criticNode != NULL)
    actNode = criticNode;
  
  while (true)
  {
    // do lava
    if ((strCompareStr(actNode->name, key)) == 1)
    {
      --(actNode->balance);
      if (actNode->left == NULL)
      {
	temp = insertToTS(&actNode->left, key, type, value);
        break;
      }
      else
	actNode = actNode->left;
    }
    // do prava
    else
    {
      ++(actNode->balance);
      if (actNode->right == NULL)
      {
	temp = insertToTS(&actNode->right, key, type, value);
        break;
      }
      else
	actNode = actNode->right;
    }
  } // koniec while(true)

  // rotovanie ak doslo k poruseniu
  if (criticNode != NULL)
  {
    if (criticNode->balance == -2)
    { 
      if (criticNode->left != NULL && criticNode->left->balance == -1)
        rotateLL(root, parentCriticNode, criticNode);
      else
        rotateDLR(root, parentCriticNode, criticNode);
    }
    else if (criticNode->balance == 2)
    {
      if (criticNode->right != NULL && criticNode->right->balance == 1)
        rotateRR(root, parentCriticNode, criticNode);
      else
        rotateDRL(root, parentCriticNode, criticNode);
    }
  }
  
  return temp;
}

/* Vyhladavnie v tabulke symbolov, ak je strom prazdny
 * vrati NULL inak pointer na dany prvok TS 
 */ 
TSItem *searchTS(TSItem *root, tString *key)
{
  if (root != NULL)
  {
    if (strCompareStr(root->name, key) == 0)
      return root;

    else if ((strCompareStr(root->name, key)) == 1)
      return searchTS(root->left, key);
    
    else 
      return searchTS(root->right, key);
  }
  else
    return NULL;
}

/* Zrusenie AVL stromu (TS) */
void deallocTS(TSItem **root)
{
  if ((*root) != NULL)
  {
    deallocTS(&(*root)->left);
    deallocTS(&(*root)->right);
    strDealloc((*root)->name);
    if ((*root)->type == tStr)
      strDealloc((*root)->value.stringValue);
    free((void *)(*root));
    (*root) = NULL;
  }
}

/* Generuje nazov pre konstanty alebo docasnu premennu, ulozeni do TS a  
 * vracia ukazatel do TS 
 */
TSItem *generator(char c, TSItemType type, TSItemValue value)
{
    static int i = 1;
    static int j = 1;
    tString *name = strInit();
    
    addCharToStr(name, c);
        
    if (c == '$')
    {
      itostr(i, name);
      ++i;
    }
    else
    {
      itostr(j, name);
      ++j;
    }
    TSItem *temp = insertToTS(&TSItemRoot, name, type, value);
    strDealloc(name);

    return temp;
}

/* Prevadza int hodnotu na tString,
 * pouziva sa pri generovani mena
 */
void itostr(int i, tString *text)
{
    tString *temp = strInit();
    int k;
    
    while (i != 0)
    {
        k = i % 10;
        i = i / 10;
        addCharToStr(temp, k + '0');
    }
    
    int tempLength = returnLength(temp);

    int j;
    for(j = tempLength-1; j >= 0; --j)
        addCharToStr(text, temp->text[j]);

    strDealloc(temp);
}

