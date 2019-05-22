/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
-----------------------------------------------
symbolTable.c

This file contains functions of symbol table.
-----------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#include "symbolTable.h"

/* this function gets a symbol label and returns true if it already exists in the symbol table, false otherwise */
boolean symbolExists(Symbol* head , char* label)
{
	Symbol* curr;
	for(curr = head; curr != NULL; curr = curr->next)
	{
		if (strcmp(curr->label,label) == 0)
			return TRUE;
	}
	return FALSE;
}

/* this function inserts a symbol to table */
void insertSymbol(Symbol** pHead,char* label, int address, Mark mark)
{
	Symbol* newSymb = (Symbol*)malloc(sizeof(Symbol));
	newSymb->label = (char*)malloc(strlen(label)+1);
	strcpy(newSymb->label,label);
	newSymb->address = address;
	newSymb->mark = mark;

	if (*pHead == NULL)           /* symbol table is empty */
		*pHead = newSymb;
	else                          /* symbol table contains symbols */
	{
		Symbol* curr;
		for(curr = *pHead; curr->next != NULL; curr = curr->next);
		curr->next = newSymb;
	}
	newSymb->next = NULL;			
}

/* this function returns the address of the label */
int getLabelAddress(Symbol* head , char* label)
{
	int result;
	Symbol* curr;
	for(curr = head; curr != NULL; curr = curr->next)
		if (strcmp(curr->label,label) == 0)
			result = curr->address;
	return result;
}

/* this function returns the mark of a symbol with specific label name */
Mark getLabelMark(Symbol* head , char* label)
{
	Mark result;
	Symbol* curr;
	for(curr = head; curr != NULL; curr = curr->next)
		if (strcmp(curr->label,label) == 0)
			result = curr->mark;
	return result;
}

/* this function returns true if symbol table contains entry symbols */
boolean hasEntrySymbols(Symbol* head)
{
	Symbol* curr;
	for(curr = head; curr != NULL; curr = curr->next)
		if(curr->mark == ENTRY)
			return TRUE;
	return FALSE;
}
