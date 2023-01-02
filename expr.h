/* Projekt:	IFJ07
 * Soubor:	expr.h
 * Team:	xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis:	Vyhodnocovani aritmeticky vyrazu, NE vycislovani
 * Autor:	xdvora14
 */


#ifndef EXPR_H
#define EXPR_H

#include "avl.h"
#include "tape.h"
#include "parser.h"
#include "scanner.h"
#include "stdbool.h"

#define STACK_SIZE_INIT 32 // pocatecni velikost zasobniku

typedef enum
{
 
	// symboly PT
	PTEqual,
	PTNonEqual,
	PTLess,
	PTGreater,
	PTLessEqual,
	PTGreaterEqual, 
	PTPlus,
	PTMinus,
	PTMultiply,
	PTDivide,
	PTPow,
	PTLeftParenthesis,  // znak (
	PTRightParenthesis, // znak )
	PTIdentifier, // identifikator jeste jako terminalni symbol == i
	PTDollar,

	PTIdentifierNonTerm, // identifikator jiz jako neterminalni symbol == E


	// ridici znaky PT
	PTLt,  // znak <
	PTGt,  // znak >
	PTEq,  // znak =
	PTEmp, // prazdna bunka v PT
	
	PTInvalid, // neplatny token
	PTError // token nepatrici do vyrazu
 
} PTItem;


// precedencni tabulka
const unsigned PT [15][15] = 
{
/*			 0		1		2		3		4		5		6		7		8		9		10		11		12		13		14		*/
/*			 ==		!=		<		>		<=		>=		+		-		*		/		^		(		)		i		$		*/
/* == */	{PTGt,	PTGt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTGt,	PTLt,	PTGt	},
/* != */	{PTGt,	PTGt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTGt,	PTLt,	PTGt	},
/* <  */	{PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTGt,	PTLt,	PTGt	},
/* >  */	{PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTGt,	PTLt,	PTGt	},
/* <= */	{PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTGt,	PTLt,	PTGt	},
/* >= */	{PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTGt,	PTLt,	PTGt	},
/* +  */	{PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTLt,	PTLt,	PTLt,	PTLt,	PTGt,	PTLt,	PTGt	},
/* -  */	{PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTLt,	PTLt,	PTLt,	PTLt,	PTGt,	PTLt,	PTGt	},
/* *  */	{PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTLt,	PTLt,	PTGt,	PTLt,	PTGt	},
/* /  */	{PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTLt,	PTLt,	PTGt,	PTLt,	PTGt	},
/* ^  */	{PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTLt,	PTLt,	PTGt,	PTLt,	PTGt	},
/* (  */	{PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTEq,	PTLt,	PTEmp	},
/* )  */	{PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTEmp,	PTGt,	PTEmp,	PTGt	},
/* i  */	{PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTGt,	PTEmp,	PTGt,	PTEmp,	PTGt	},
/* $  */	{PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTLt,	PTEmp,	PTLt,	PTEmp	}
}; 

typedef struct PSAItem
{
	PTItem operator;	// operatory a ridici znaky
	TSItem * operand;	// ukazatel do tabulky symbolu
} PSAItem_t;			// typ polozky zasobniku


typedef struct PTStack
{
	PSAItem_t * item;	// pole polozek z PTItem
	size_t size;		// velikost zasobniku
	size_t count;		// aktualni zaplneni zasobniku
} *PTStack_t;			// ukazatel na zasobnik



tError expressionParser (TSItem ** item);
/*
 *  Funkce starajici se o vyhodnocovani vyrazu pomoci Precedencni tabulky, dale jen PT.
 *	V PT mam nadefinovanou prioritu operatoru a take pomoci ni lze zjistit korektnost vyrazu.
 *	Zpracovava nasledujici operatory: 
 *										- aritmeticke: +, -, *, /, ^
 *										- relacni: ==, !=, <, >, <=, >=
 *										- zavorky: (, )
 * 
 *  @param item Ukazatel do tabulky symbolu.
 *  @return Chybove hlaseni z enumu tError.
 */


static PTStack_t stackInit (void);
/*
 *  Inicializace zasobniku pro praci s vyrazy.
 *	Zasobnik se inicializuje na velikost STACK_SIZE_INIT a na jeho vrchol se da pocatecni hodnota $.
 *	
 *  @param void
 *  @return Ukazatel na zasobnik.
 */


static void stackDestroy (PTStack_t stack);
/*
 *  Destrukce zasobniku.
 *	
 *  @param stack Ukazatel na zasobnik.
 *  @return void
 */


static inline void stackPush (PTStack_t stack, PTItem operator, TSItem * operand);
/*
 *  Inline funkce davajici na vrchol zasobniku jednu polozku.
 *	Pokud je zasobnik zaplnen, tak je rozsiren o STACK_SIZE_INIT polozek.
 *	
 *  @param stack Ukazatel na zasobnik.
 *	@param operator Druh operatoru, o jaky se jedna, napr. +, -, i, E atd.
 *	@param operand Ukazatel do TS. Pokud neni operator i nebo E, pak je operand roven NULL.
 *  @return void.
 */


static inline PSAItem_t stackPop (PTStack_t stack);
/*
 *  Inline funkce odebirajici polozku z vrcholu zasobniku.
 *	
 *  @param stack Ukazatel na zasobnik.
 *  @return Polozka z vrcholu zasobniku.
 */


static inline PSAItem_t * stackTopTerminal (PTStack_t stack);
/*
 *  Inline funkce zjistujici, jaky je nejvyssi terminalni symbol na zasobniku.
 *	Vzdy to bude bud vrchol zasobniku nebo druhy nejvyssi prvek.
 *	
 *  @param stack Ukazatel na zasobnik.
 *  @return Ukazatel na nejvyssi terminalni symbol na zasobniku.
 */


static inline tError exePlusMinusMul (TSItemValue * xInit, TSItem ** item, PSAItem_t * opFirst, PSAItem_t * opSecond, unsigned instruction);
/*
 *  Inline funkce vyhodnocujici aritmeticke operace +, -, * a konkatenaci testovych retezcu.
 *	
 *  @param xInit Hodnota identifikatoru nebo konstanty - definovana jako union
 *	@param item Ukazatel do TS, kam budu ukladat vysledek operace.
 *	@param opFirst Prvni operand vyrazu.
 *	@param opSecond Druhy operand vyrazu.
 *	@param instruction Polozka z intrukci sady Instruction.
 *  @return Chybove hlaseni z enumu tError.
 */


static inline tError exeDividePow (TSItemValue * xInit, TSItem ** item, PSAItem_t * opFirst, PSAItem_t * opSecond, unsigned instruction);
/*
 *  Inline funkce vyhodnocujici aritmeticke operace /, ^.
 *	
 *  @param xInit Hodnota identifikatoru nebo konstanty - definovana jako union
 *	@param item Ukazatel do TS, kam budu ukladat vysledek operace.
 *	@param opFirst Prvni operand vyrazu.
 *	@param opSecond Druhy operand vyrazu.
 *	@param instruction Polozka z intrukci sady Instruction.
 *  @return Chybove hlaseni z enumu tError.
 */


static inline tError exeRelation (TSItemValue * xInit, TSItem ** item, PSAItem_t * opFirst, PSAItem_t * opSecond, unsigned instruction);
/*
 *  Inline funkce vyhodnocujici relacni operace ==, !=, <, >, <=, >=.
 *	
 *  @param xInit Hodnota identifikatoru nebo konstanty - definovana jako union
 *	@param item Ukazatel do TS, kam budu ukladat vysledek operace.
 *	@param opFirst Prvni operand vyrazu.
 *	@param opSecond Druhy operand vyrazu.
 *	@param instruction Polozka z intrukci sady Instruction.
 *  @return Chybove hlaseni z enumu tError.
 */


static PTItem relevantToken (tToken tokenType);
/*
 *  Funkce zjistujici, zda-li muze byt token pouzit ve vyrazu, tzn. je z enumu PTItem.
 *	Nasledne token prelozi na polozku z PTItem.
 *	
 *  @param tokenType Typ tokenu, ktery mi prijde z funkce getNextToken.
 *  @return Polozka z enumu PTItem.
 */


static inline bool isDollar (PTItem tokenType);
/*
 *  Inline funkce zjistujici, zda-li je token dolar $.
 *	
 *  @param tokenType Typ tokenu z enumu PTItem.
 *  @return True nebo False.
 */


static bool isTerminal (PTItem tokenType);
/*
 *  Funkce zjistujici, zda-li je token terminalni symbol.
 *	
 *  @param tokenType Typ tokenu z enumu PTItem.
 *  @return True nebo False.
 */

/*
static void printStack (PTStack_t stack);
*/
#endif
