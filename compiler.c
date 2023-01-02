/* Projekt: IFJ07
 * Subor: compiler.c
 * Tym: xbendl00, xdanko00, xduric00, xdvora14, xkopri05
 */

#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"

int main(int argc, char ** argv){

    tError returnValue;

    if (argc != 2)
        return otherError;
    if ((fr = fopen(argv[1], "r")) == NULL)
        return otherError;
	
    initTS (&TSItemRoot);
    TapeInit();

    returnValue = parser();

    if(returnValue == noError)
        returnValue = interpretation();

    // uvolnovat pamat

    TapeDestroy();
    deallocTS (&TSItemRoot);

	//const char * errArray [] = {"noError", "lexError", "syntaxError", "semanticError", "interpretError", "otherError"};
	//printf("\nChyba: %s\n", errArray [returnValue]);

    fclose(fr);
    return returnValue;
}
