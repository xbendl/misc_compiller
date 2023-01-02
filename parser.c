/* Projekt: IFJ07
 * Subor: parser.c
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 * Popis: Syntakticky analyzator.
 * Autor: xdanko00
 */
     
#include "parser.h"
#include "tape.h"

/* funkcia nonterminalu <PROGRAM>
 ***********************************
 * <PROGRAM> -> <DECLARATION> <BODY>
 */

tError program(tToken token, TSItem *pTokenData){
    
    tError returnValue;
    
    returnValue = declaration(token,pTokenData);
    //pokial deklaracna cast OK -> analyza tela
    if(returnValue == noError){
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        returnValue = body(token,pTokenData);
        if(returnValue == noError){
            if((token = getNextToken(&pTokenData))== tInvalidToken)
                return lexError;
            if(token != tEOF)
                return syntaxError;
        }
        return returnValue;
    }
    else 
        return returnValue;

}

/* funkcia nonterminalu <DECLARATION>
 *****************************************
 * <DECLARATION> -> int ID ; <DECLARATION>
 * <DECLARATION> -> double ID ; <DECLARATION>
 * <DECLARATION> -> string ID ; <DECLARATION>
 * <DECLARATION> -> {
 */
 
tError declaration(tToken token, TSItem *pTokenData){


    switch (token) {
    case tTypeInt:
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tIdentifier)
            return syntaxError;
        if(pTokenData->type == tNone)
            pTokenData->type = tInt;
        else
            return semanticError;
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tSemicollon)
            return syntaxError;
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        return declaration(token,pTokenData);
        break;
    case tTypeDouble:
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tIdentifier)
            return syntaxError;
        if(pTokenData->type == tNone)
            pTokenData->type = tDouble;
        else
            return semanticError;
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tSemicollon)
            return syntaxError;
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        return declaration(token,pTokenData);
        break;
    case tTypeString:
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tIdentifier)
            return syntaxError;
        if(pTokenData->type == tNone)
            pTokenData->type = tStr;
        else
            return semanticError;
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tSemicollon)
            return syntaxError;
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        return declaration(token,pTokenData);
        break;
    case tBlockStart:
        return noError;
        break;
    default:
        return syntaxError;
        break;
    }

}

/* funkcia nonterminalu <BODY>
 ******************************
 * <BODY> -> <STATEMENT> <BODY>
 * <BODY> -> } 
 */
 
tError body(tToken token, TSItem *pTokenData){
    tError returnValue;
    
    //printf("som v body() - ");printToken(token);
    
    if(token == tBlockEnd)
        return noError;
    else {
        returnValue = statement(token,pTokenData);
        if(returnValue == noError){
            if((token = getNextToken(&pTokenData))== tInvalidToken)
                return lexError;
            return body(token,pTokenData);
        }
        else
            return returnValue;
    }
}

/* funkcia nonterminalu <STATEMENT>
 ***********************************
 * <STATEMENT> -> sort ID ;
 * <STATEMENT> -> ID = <vyraz> ;
 * <STATEMENT> -> if <vyraz> <BODYORSTAT> else <BODYORSTAT>
 * <STATEMENT> -> while <vyraz> <BODYORSTAT>
 * <STATEMENT> -> read ID <IDS>
 * <STATEMENT> -> write <vyraz> <EXPRESSIONS>
 */
 
tError statement(tToken token, TSItem *pTokenData){
    tError returnValue;
    TSItem *expressionItem;
    TSItem *workItem;
    TapeItem *goto1;
    TapeItem *goto2;
    TapeItem *label;
    TSItemValue initValue;
   
    //printf("som v statement() - ");printToken(token);
    
    switch (token) {
    case tSort:
        if((token = getNextToken(&workItem))== tInvalidToken)
            return lexError;
        if(token != tIdentifier)
            return syntaxError;
        if(workItem->type != tStr)
            return semanticError;
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tSemicollon)
            return syntaxError;
        // --generovanie kodu--
        TapeInsert(I_SORT,workItem,NULL,workItem);
        //---------------------
        return noError;
        break;
    case tIdentifier:
        if(pTokenData->type == tNone)
            return semanticError;
        workItem = pTokenData;
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tOpAssignment)
            return syntaxError;
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tDollar)
            return syntaxError;
        returnValue = expressionParser(&expressionItem);
        if(returnValue == noError){
            if(expressionItem->type != workItem->type){
                if((expressionItem->type == tInt)&&(workItem->type ==
                    tDouble)){
                    // pretypovanie z Int na Double
                    initValue.doubleValue = 0.0;
                    pTokenData = generator('@', tDouble, initValue);
                    // --generovanie kodu--
                    TapeInsert(I_INT_TO_DOUBLE,expressionItem,NULL,pTokenData);
                    //---------------------
                    expressionItem = pTokenData;
                }
                else 
                    return semanticError;           
            }
            if((token = getNextToken(&pTokenData))== tInvalidToken)
                return lexError;
            if(token != tSemicollon)
                return syntaxError;
            // --generovanie kodu--
            TapeInsert(I_ASSIGN,expressionItem,NULL,workItem);
            //---------------------
            return noError;
        }
        else
            return returnValue;
        break;
    case tIf:
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tDollar)
            return syntaxError;
        returnValue = expressionParser(&expressionItem);
        if(returnValue == noError){
            if(expressionItem->type != tInt)
                return semanticError;
            // --generovanie kodu--
            initValue.intValue = 0;
            workItem = generator('@', tInt, initValue);
            TapeInsert(I_NOT,expressionItem,NULL,workItem);
            goto1 = TapeInsert(I_GOTO,workItem,NULL,NULL);
            //---------------------
            if((token = getNextToken(&pTokenData))== tInvalidToken)
                return lexError;
            returnValue = bodyorstat(token,pTokenData);
            if(returnValue == noError){
                if((token = getNextToken(&pTokenData))== tInvalidToken)
                    return lexError;
                if(token != tElse)
                    return syntaxError;
                // --generovanie kodu--
                goto2 = TapeInsert(I_GOTO,NULL,NULL,NULL);
                goto1->dest = (TSItem *) TapeInsert(I_LAB,NULL,NULL,NULL);
                //---------------------
                if((token = getNextToken(&pTokenData))== tInvalidToken)
                    return lexError;
                returnValue = bodyorstat(token,pTokenData);
                // --generovanie kodu--
                goto2->dest = (TSItem *) TapeInsert(I_LAB,NULL,NULL,NULL);
                //---------------------
                return returnValue;
            }
            else
                return returnValue;
        }
        else
            return returnValue;
        break;
    case tWhile:
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tDollar)
            return syntaxError;
        // --generovanie kodu--
        label = TapeInsert(I_LAB,NULL,NULL,NULL);
        // --generovanie kodu--
        returnValue = expressionParser(&expressionItem);
        if(returnValue == noError){
            if(expressionItem->type != tInt)
                return semanticError;
            // --generovanie kodu--
            initValue.intValue = 0;
            workItem = generator('@', tInt, initValue);
            TapeInsert(I_NOT,expressionItem,NULL,workItem);
            goto1 = TapeInsert(I_GOTO,workItem,NULL,NULL);
            //---------------------
            if((token = getNextToken(&pTokenData))== tInvalidToken)
                return lexError;
            returnValue = bodyorstat(token,pTokenData);
            // --generovanie kodu--
            TapeInsert(I_GOTO,NULL,NULL,(TSItem*) label);
            goto1->dest = (TSItem *) TapeInsert(I_LAB,NULL,NULL,NULL);
            //---------------------
            return returnValue;
        }
        else
            return returnValue;
        break;
    case tRead:
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tIdentifier)
            return syntaxError;
        if(pTokenData->type == tNone){
            return semanticError;
        }
        // --generovanie kodu--
        TapeInsert(I_READ,NULL,NULL,pTokenData);
        //---------------------
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        return ids(token,pTokenData);
        break;
    case tWrite:
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tDollar)
            return syntaxError;
        returnValue = expressionParser(&expressionItem);
        if(returnValue == noError){
            // --generovanie kodu--
            TapeInsert(I_WRITE,NULL,NULL,expressionItem);
            //---------------------
            if((token = getNextToken(&pTokenData))== tInvalidToken)
                return lexError;
            return expressions(token,pTokenData);
        }
        else
            return returnValue;
        break;
    default:
        return syntaxError;
        break;
    }
}

/* funkcia nonterminalu <BODYORSTAT>
 ***********************************
 * <BODYORSTAT> -> { <BODY>
 * <BODYORSTAT> -> <STATEMENT>
 */
 
tError bodyorstat(tToken token, TSItem *pTokenData){
    
    //printf("som v bodyorstat() - ");printToken(token);
    
    if(token == tBlockStart){
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        return body(token,pTokenData);
    }
    else {
        return statement(token,pTokenData);
    }
}

/* funkcia nonterminalu <IDS>
 ***********************************
 * <IDS> -> , ID <IDS>
 * <IDS> -> ;
 */
 
tError ids(tToken token, TSItem *pTokenData){

    //printf("som v ids() - ");printToken(token);
    
    switch (token) {
    case tComma:
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tIdentifier)
            return syntaxError;
        if(pTokenData->type == tNone)
            return semanticError;
        // --generovanie kodu--
        TapeInsert(I_READ,NULL,NULL,pTokenData);
        //---------------------
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        return ids(token,pTokenData);
        break;
    case tSemicollon:
        return noError;
        break;
    default:
        return syntaxError;
        break;
    }
    
}

/* funkcia nonterminalu <EXPRESSIONS>
 ***********************************
 * <EXPRESSIONS> -> , <vyraz> <EXPRESSIONS>
 * <EXPRESSIONS> -> ;
 */

tError expressions(tToken token, TSItem *pTokenData){
    tError returnValue;
    TSItem * expressionItem;

    //printf("som v expressions() - ");printToken(token);
    
    switch (token) {
    case tComma:
        if((token = getNextToken(&pTokenData))== tInvalidToken)
            return lexError;
        if(token != tDollar)
            return syntaxError;
        returnValue = expressionParser(&expressionItem);
        if(returnValue == noError){
            // --generovanie kodu--
            TapeInsert(I_WRITE,NULL,NULL,expressionItem);
            //---------------------
            if((token = getNextToken(&pTokenData))== tInvalidToken)
                return lexError;
            return expressions(token,pTokenData);
        }
        else
            return returnValue;
        break;
    case tSemicollon:
        return noError;
        break;
    default:
        return syntaxError;
        break;
    }
    
}

/* funkcia spustajuca syntakticku analyzu
 */

tError parser()
{

    tToken token;
    TSItem *pTokenData;
    
    if((token = getNextToken(&pTokenData))== tInvalidToken)
        return lexError;
    return program(token,pTokenData);
}

// veci, ktore treba vymazat


void printToken(tToken token){
    switch (token) {
    case tIdentifier:
        //printf("mam token tIdentifier\n");
        break;
    case tTypeInt:
        //printf("mam token tTypeInt\n");
        break;
    case tTypeDouble:
        //printf("mam token tTypeDouble\n");
        break;
    case tTypeString:
        //printf("mam token tTypeString\n");
        break;
    case tIf:
        //printf("mam token tIf\n");
        break;
    case tElse:
        //printf("mam token tElse\n");
        break;
    case tWhile:
        //printf("mam token tWhile\n");
        break;
    case tRead:
        //printf("mam token tRead\n");
        break;
    case tWrite:
        //printf("mam token tWrite\n");
        break;
    case tSort:
        //printf("mam token tSort\n");
        break;
    case tEOF:
        //printf("mam token tEOF\n");
        break;
    case tDollar:
        //printf("mam token tDollar\n");
        break;
    case tComma:
        //printf("mam token tComma\n");
        break;
    case tSemicollon:
        //printf("mam token tSemicollon\n");
        break;
    case tBlockStart:
        //printf("mam token tBlockStart\n");
        break;
    case tBlockEnd:
        //printf("mam token tBlockEnd\n");
        break;
    default:
        //printf("mam nieco ine :)- %d\n", token);
        break;
    }
}
