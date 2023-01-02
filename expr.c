/* Projekt:	IFJ07
 * Soubor:	expr.c
 * Team:	xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis:	Vyhodnocovani aritmeticky vyrazu pomoci Precedencni tabulky.
 * Autor:	xdvora14
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "expr.h"


tError expressionParser (TSItem ** item)
{
	TSItem * actInSymbol = NULL; // aktualni vstupni symbol - ukazatel do TS
	PSAItem_t * topTermSymbol; // ukazatel na nejvrchnejsi terminalni symbol na zasobniku
	PTItem tokenType; // typ aktualniho tokenu

	// inicializace zasobniku
	PTStack_t stack = stackInit (); 
	tokenType = relevantToken (getNextToken (&actInSymbol));


	// cyklus prijimajici vstupni retezec vyrazu
	do 
	{
		topTermSymbol = stackTopTerminal (stack);
				
		// pokud se nejedna o token vyskytujici se ve vyrazech, tak zavolam syntakticka chybu 
		if (tokenType == PTError)
		{	
			stackDestroy (stack);
			return syntaxError;
		}
		// pokud se nejedna o token vyskytujici se ve vyrazech, tak zavolam lexikalni chybu 
		if (tokenType == PTInvalid)
		{	
			stackDestroy (stack);
			return lexError;
		}
		// pokud se jedna o promennou, ktera nebyla inicializovana, tak zavolam semantickou chybu
		if (actInSymbol != NULL && actInSymbol->type == tNone)
		{	
			stackDestroy (stack);
			return semanticError;
		}

		// podivam se do Precedencni tabulky, co vlastne budu s vyrazem delat
		switch (PT [topTermSymbol->operator][tokenType])
		{
			case PTEmp: // nepovolena operace
					stackDestroy (stack);
					return syntaxError;

			case PTLt: // delam to stejne jako u rovnitka, resim bez vkladani PTLt == '<'
			case PTEq: // precteny token ze vstupu a dej ho na zasobnik
					if (isTerminal (tokenType) && tokenType != PTIdentifier)
						stackPush (stack, tokenType, NULL);
					else
						stackPush (stack, PTIdentifier, actInSymbol);
						
					tokenType = relevantToken (getNextToken (&actInSymbol)); // nactu dalsi token
					break;

			case PTGt: // vyhodnoceni pri PTGt == '>'
					if (topTermSymbol->operator == PTIdentifier) // pokud je na vrholu terminalni identifikator == 'i'
					{
						topTermSymbol->operator = PTIdentifierNonTerm; // udelam z terminalniho nonterminalni E
						*item = topTermSymbol->operand; // pokud by byl ve vyrazu jen jeden identifikator, tak tady jej vracim
						break;
					}
				
					if (topTermSymbol->operator == PTRightParenthesis) // odstraneni leve zavorky, kdyz prava je na zasobniku
					{
						stackPop (stack); // prava zavorka
						
						PSAItem_t tmp = stackPop (stack); // nonterminal uprostred
						if (tmp.operator != PTIdentifierNonTerm) // kdyz neni nonterminal, chyba
						{	
							stackDestroy (stack);
							return syntaxError;
						}
						
						PSAItem_t lPar  = stackPop (stack); // leva zavorka
						if (lPar.operator != PTLeftParenthesis) // kdyz neni zavorka, chyba
						{	
							stackDestroy (stack);
							return syntaxError;
						}
						
						stackPush (stack, tmp.operator, tmp.operand); // vratim nonterminal na zasobnik
						break;
					}
					
					// dal resim jen binarni operatory tvaru E1 op E2

					PSAItem_t opSecond = stackPop (stack); // nonterminal E2

					if (opSecond.operand == NULL) // pokud neexistuje, chyba
					{	
						stackDestroy (stack);
						return syntaxError;
					}

					PSAItem_t tmp = stackPop (stack); // operator op
					if (tmp.operator == PTIdentifierNonTerm) // pokud je na zasobniku za sebou E2 a E
					{	
						stackDestroy (stack);
						return syntaxError;
					}

					PSAItem_t opFirst = stackPop (stack); // nonterminal E1
					if (opFirst.operand == NULL) // pokud neexistuje, chyba
					{	
						stackDestroy (stack);
						return syntaxError;
					}
					
					TSItemValue xInit;

					switch (tmp.operator)
					{
						case PTPlus: // E + E
							if (exePlusMinusMul (&xInit, item, &opFirst, &opSecond, I_ADD) == semanticError)
							{	
								stackDestroy (stack);
								return semanticError;
							}
							break;

						case PTMinus: // E - E
							if (exePlusMinusMul (&xInit, item, &opFirst, &opSecond, I_SUB) == semanticError)
							{	
								stackDestroy (stack);
								return semanticError;
							}
							break;

						case PTMultiply: // E * E
							if (exePlusMinusMul (&xInit, item, &opFirst, &opSecond, I_MUL) == semanticError)
							{	
								stackDestroy (stack);
								return semanticError;
							}
							break;

						case PTDivide: // E / E
							if (exeDividePow (&xInit, item, &opFirst, &opSecond, I_DIV) == semanticError)
							{	
								stackDestroy (stack);
								return semanticError;
							}
							break;

						case PTPow: // E ^ E
							if (exeDividePow (&xInit, item, &opFirst, &opSecond, I_POW) == semanticError)
							{	
								stackDestroy (stack);
								return semanticError;
							}
							break;

						case PTEqual: // E == E
							if (exeRelation (&xInit, item, &opFirst, &opSecond, I_EQ) == semanticError)
							{	
								stackDestroy (stack);
								return semanticError;
							}
							break;

						case PTNonEqual: // E != E
							if (exeRelation (&xInit, item, &opFirst, &opSecond, I_NEQ) == semanticError)
							{	
								stackDestroy (stack);
								return semanticError;
							}
							break;

						case PTLess: // E < E
							if (exeRelation (&xInit, item, &opFirst, &opSecond, I_LESS) == semanticError)
							{	
								stackDestroy (stack);
								return semanticError;
							}
							break;

						case PTGreater: // E > E
							if (exeRelation (&xInit, item, &opFirst, &opSecond, I_GREATER) == semanticError)
							{	
								stackDestroy (stack);
								return semanticError;
							}
							break;

						case PTLessEqual: // E <= E
							if (exeRelation (&xInit, item, &opFirst, &opSecond, I_LE) == semanticError)
							{	
								stackDestroy (stack);
								return semanticError;
							}
							break;

						case PTGreaterEqual: // E >= E
							if (exeRelation (&xInit, item, &opFirst, &opSecond, I_GE) == semanticError)
							{	
								stackDestroy (stack);
								return semanticError;
							}
							break;


						default: // E neco_jineho E
							stackDestroy (stack);
							return syntaxError; 

					} // switch (tmp.operator)

					stackPush (stack, PTIdentifierNonTerm, *item); // vysledek dam na zasobnik

					break;

			default: // tohle nenastane nikdy, ale co kdyby :o)
					return otherError;
					
		} // switch (PT [topTermSymbol->operator][tokenType])
		//printStack (stack);
		topTermSymbol = stackTopTerminal (stack);

	} while (!isDollar (tokenType) || !isDollar (topTermSymbol->operator));
	
	stackDestroy (stack);
	return noError;
} // switch (PT [topTermSymbol->operator][tokenType])


static inline tError exeRelation (TSItemValue * xInit, TSItem ** item, PSAItem_t * opFirst, PSAItem_t * opSecond, unsigned instruction)
{
	xInit->intValue = 0;
	*item = generator ('@', tInt, *xInit); // vysledek operace

	if ((opFirst->operand)->type == tStr && (opSecond->operand)->type == tStr) // str op str
	{
		TapeInsert (instruction, opFirst->operand, opSecond->operand, *item);		
	}	
	else if (opFirst->operand->type == tInt && opSecond->operand->type == tInt) // int op int
	{
		TapeInsert (instruction, opFirst->operand, opSecond->operand, *item);			
	}
	else if (opFirst->operand->type == tDouble && opSecond->operand->type == tDouble) // double op double
	{
		TapeInsert (instruction, opFirst->operand, opSecond->operand, *item);			
	}
	else if (opFirst->operand->type == tInt && opSecond->operand->type == tDouble) // int op double
	{
		TSItem * tmpIntToDouble = generator ('@', tDouble, *xInit); // vytvoreni promenne
		TapeInsert (I_INT_TO_DOUBLE, opFirst->operand, NULL, tmpIntToDouble); // prevod int na double
		TapeInsert (instruction, tmpIntToDouble, opSecond->operand, *item);
	}
	else if (opFirst->operand->type == tDouble && opSecond->operand->type == tInt) // double op int
	{
		TSItem * tmpIntToDouble = generator ('@', tDouble, *xInit); // vytvoreni promenne
		TapeInsert (I_INT_TO_DOUBLE, opSecond->operand, NULL, tmpIntToDouble); // prevod int na double
		TapeInsert (instruction, opFirst->operand, tmpIntToDouble, *item);
	}
	else // vsechny ostatni kombinace jsou semanticky spatne
		return semanticError;

	return noError;
} // static inline tError exeRelation

static inline tError exePlusMinusMul (TSItemValue * xInit, TSItem ** item, PSAItem_t * opFirst, PSAItem_t * opSecond, unsigned instruction)
{
	if (instruction == I_ADD && (opFirst->operand)->type == tStr && (opSecond->operand)->type == tStr) // str + str
	{
		xInit->stringValue = NULL;
		*item = generator ('@', tStr, *xInit);
		TapeInsert (instruction, opFirst->operand, opSecond->operand, *item);		
	}	
	else if (opFirst->operand->type == tInt && opSecond->operand->type == tInt) // int op int
	{
		xInit->intValue = 0;
		*item = generator ('@', tInt, *xInit);
		TapeInsert (instruction, opFirst->operand, opSecond->operand, *item);			
	}
	else if (opFirst->operand->type == tDouble && opSecond->operand->type == tDouble) // double op double
	{
		xInit->doubleValue = 0.0;
		*item = generator ('@', tDouble, *xInit);
		TapeInsert (instruction, opFirst->operand, opSecond->operand, *item);			
	}
	else if (opFirst->operand->type == tInt && opSecond->operand->type == tDouble) // int op double
	{
		xInit->doubleValue = 0.0;
		*item = generator ('@', tDouble, *xInit);
		TSItem * tmpIntToDouble = generator ('@', tDouble, *xInit);
		TapeInsert (I_INT_TO_DOUBLE, opFirst->operand, NULL, tmpIntToDouble);
		TapeInsert (instruction, tmpIntToDouble, opSecond->operand, *item);
	}
	else if (opFirst->operand->type == tDouble && opSecond->operand->type == tInt) // double op int
	{
		xInit->doubleValue = 0.0;
		*item = generator ('@', tDouble, *xInit);
		TSItem * tmpIntToDouble = generator ('@', tDouble, *xInit);
		TapeInsert (I_INT_TO_DOUBLE, opSecond->operand, NULL, tmpIntToDouble);
		TapeInsert (instruction, opFirst->operand, tmpIntToDouble, *item);
	}
	else // vsechny ostatni kombinace jsou semanticky spatne
		return semanticError;	


	return noError;
} // static inline tError exePlusMinusMul

static inline tError exeDividePow (TSItemValue * xInit, TSItem ** item, PSAItem_t * opFirst, PSAItem_t * opSecond, unsigned instruction)
{
	xInit->doubleValue = 0.0;
	*item = generator ('@', tDouble, *xInit); // vysledek operace

	if (opFirst->operand->type == tInt && opSecond->operand->type == tInt) // int op int
	{
		TSItem * tmpIntToDouble = generator ('@', tDouble, *xInit);
		TapeInsert (I_INT_TO_DOUBLE, opFirst->operand, NULL, tmpIntToDouble);
		TSItem * tmpIntToDouble2 = generator ('@', tDouble, *xInit);
		TapeInsert (I_INT_TO_DOUBLE, opSecond->operand, NULL, tmpIntToDouble2);
		TapeInsert (instruction, tmpIntToDouble, tmpIntToDouble2, *item);			
	}
	else if (opFirst->operand->type == tDouble && opSecond->operand->type == tDouble) // double op double
	{
		TapeInsert (instruction, opFirst->operand, opSecond->operand, *item);			
	}
	else if (opFirst->operand->type == tInt && opSecond->operand->type == tDouble) // int op double
	{
		TSItem * tmpIntToDouble = generator ('@', tDouble, *xInit);
		TapeInsert (I_INT_TO_DOUBLE, opFirst->operand, NULL, tmpIntToDouble);
		TapeInsert (instruction, tmpIntToDouble, opSecond->operand, *item);
	}
	else if (opFirst->operand->type == tDouble && opSecond->operand->type == tInt) // double op double
	{
		TSItem * tmpIntToDouble = generator ('@', tDouble, *xInit);
		TapeInsert (I_INT_TO_DOUBLE, opSecond->operand, NULL, tmpIntToDouble);
		TapeInsert (instruction, opFirst->operand, tmpIntToDouble, *item);
	}
	else // vsechny ostatni kombinace jsou semanticky spatne
		return semanticError;

	return noError;
} // static inline tError exeDividePow

static inline bool isDollar (PTItem tokenType)
{
	if (tokenType == PTDollar)
		return true;
	else
		return false;
} // static bool isDollar

static bool isTerminal (PTItem tokenType)
{
	if (tokenType <= PTDollar) // dle PTItem enumu
		return true;
	else
		return false;
} // static bool isTerminal

static PTItem relevantToken (tToken tokenType)
{
	switch (tokenType)
	{
		case tOpPlus:				return PTPlus;
		case tOpMinus:				return PTMinus;
		case tOpMultiply:			return PTMultiply;
		case tOpDivide:				return PTDivide;
		case tOpPow:				return PTPow;
		case tOpEqual:				return PTEqual;
		case tOpNonEqual:			return PTNonEqual;
		case tOpLess:				return PTLess;
		case tOpGreater:			return PTGreater;
		case tOpLessEqual:			return PTLessEqual;
		case tOpGreaterEqual:		return PTGreaterEqual;
		case tLeftParenthesis:		return PTLeftParenthesis;
		case tRightParenthesis:		return PTRightParenthesis;
		case tDollar:				return PTDollar;

		case tIdentifier:
		case tLiteralInt:
		case tLiteralDouble:
		case tLiteralString:		return PTIdentifier; 

		case tInvalidToken:			return PTInvalid;

		default:					return PTError;
	}
} // static PTItem relevantToken


static PTStack_t stackInit (void)
{
	PTStack_t stack = (PTStack_t)malloc (1 * sizeof(struct PTStack));
	if (stack == NULL)
		exit (otherError); // chybna alokace

	stack->size = STACK_SIZE_INIT;
	stack->count = 0;

	stack->item = (PSAItem_t *)malloc (STACK_SIZE_INIT * sizeof(struct PSAItem));
	if (stack->item == NULL)
		exit (otherError); // chybna alokace
	
	stackPush (stack, PTDollar, NULL); // prvni hodnota zasobniku = $
	
	return stack;
} // static PTStack_t stackInit (void)

static void stackDestroy (PTStack_t stack)
{
	free (stack->item);
	free (stack);
	stack = NULL;

	return ;
} // static void stackDestroy (PTStack_t stack)


static inline void stackPush (PTStack_t stack, PTItem operator, TSItem * operand)
{
	if (stack->count >= stack->size) // zvetseni zasobniku
	{
		stack->size += STACK_SIZE_INIT;
		stack->item = (PSAItem_t *)realloc (stack->item, stack->size * sizeof(struct PSAItem));
		if (stack->item == NULL)
			exit (otherError); // chybna alokace
	}
	stack->item [stack->count].operator = operator;
	stack->item [stack->count].operand = (operand != NULL ? operand : NULL);
	stack->count++;

	return ;
} // static inline void stackPush (PTStack_t stack, PTItem operator, TSItem * operand

static inline PSAItem_t stackPop (PTStack_t stack)
{
	stack->count--;
	return stack->item [stack->count];
}

static inline PSAItem_t * stackTopTerminal (PTStack_t stack)
{
	if (isTerminal (stack->item[stack->count - 1].operator))
		return &(stack->item[stack->count - 1]);
	else
		return &(stack->item[stack->count - 2]);
} // static inline PSAItem_t * stackTopTerminal (PTStack_t stack)


/*
static void printStack (PTStack_t stack)
{
	const char * transArray [] = {"==", "!=", "<", ">", "<=", ">=", "+", "-", "*", "/", "^", "(", ")", "i", "$", "E", "Lt", "Gt", "Eq", "Emp", "Err"};       
							
	printf("Na zasobniku je: ");
	for (int i = 0; i < stack->count; i++)
		printf("%s ", transArray[stack->item[i].operator]);
	printf("\n");
} // static void printStack (PTStack_t stack)
*/
