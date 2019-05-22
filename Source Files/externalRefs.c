/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
-----------------------------------------------------------
externalRefs.c

This is a file that contains external references functions.  
-----------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "externalRefs.h" 

/* This function inserts an external reference to a list */
void insertExternalReference(ExternalRef** pHead, char* label, int address)
{
	/* memory allocation */
	ExternalRef* newExternalRef = (ExternalRef*)malloc(sizeof(ExternalRef));
	if (!newExternalRef)
	{
		printf("memory allocation failed\n");
		exit(ERROR_MEMORY);
	}

	/* update fields */
	newExternalRef->label = (char*)malloc(strlen(label)+1);
	strcpy(newExternalRef->label,label);
	newExternalRef->address = address;
	
	if (*pHead == NULL)
	{ 
		*pHead = newExternalRef;
	}
	else
	{
		ExternalRef* curr;
		for(curr = *pHead; curr->next != NULL; curr = curr->next);
		curr->next = newExternalRef;
	}
	newExternalRef->next = NULL;   /* marks the end of the list. */
}

/* This function returns true if the external references list is empty, false otherwise */
boolean isEmpty(ExternalRef* head)
{
	if (head == NULL)
		return TRUE;
	return FALSE;		
}
